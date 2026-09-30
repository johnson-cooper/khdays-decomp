/* DS 3D textures -> GS textures, with a GS VRAM residency cache.
 *
 * A DS texture is named by TEXIMAGE_PARAM (VRAM offset, size, format, colour-0 transparency) and
 * PLTT_BASE.  Its texels live in the texture-image VRAM view, its palette in the texture-palette
 * view (nitro_gx.c).  Conversion avoids touching texels wherever the GS can take them as they are:
 *
 *   format           GS texture             texels                       CLUT (PSMCT32, CSM1)
 *   1 A3I5           PSMT8                  as is                        256: [a<<5|i] = pal[i], alpha a
 *   2 4-colour       PSMT4                  2 bpp expanded to 4 bpp      16 (4 used)
 *   3 16-colour      PSMT4                  as is                        16
 *   4 256-colour     PSMT8                  as is                        256
 *   5 4x4 compressed PSMCT32                decoded once, then cached    -
 *   6 A5I3           PSMT8                  as is                        256: [a<<3|i] = pal[i], alpha a
 *   7 direct         PSMCT16                as is (A1BGR5 == GS 16-bit)  -
 *
 * Residency: entries carry GS address, PSM, size, CLUT address, the texture/palette VRAM
 * generation they were built from, and the last frame they were used.  Space is allocated linearly
 * from the 3D texture region; when it is full, everything not used in the current frame is dropped
 * and the region is compacted by starting over (textures used this frame are re-uploaded).  Any
 * reload of texture VRAM by the game bumps the generation and invalidates the cache.
 */
#include "platform/kh_platform.h"
#include "platform/ps2/ps2_gs.h"
#include "nitro_internal.h"

#include <string.h>
#include <gs_gp.h>
#include <gs_psm.h>

enum { VIEW_TEX = 4, VIEW_TEXPLTT = 5 };

typedef struct TexEntry {
    u32 key0, key1;          /* teximage (without texcoord mode) | pltt | colour-0 flag */
    u32 gen;
    u32 tbp, cbp;            /* GS block addresses (64 words) */
    u8 psm, tbw, tw, th;
    u32 last_frame;
    int has_clut;
} TexEntry;

#define MAX_ENTRIES 512
static TexEntry g_ent[MAX_ENTRIES];
static int g_nent;
static u32 g_region_base, g_region_blocks, g_next_block;   /* in 256-byte blocks */
static u32 g_gen = 1, g_frame;
static u32 g_used_bytes;

uint32_t kh_tex3d_used(void) { return g_used_bytes; }

void kh_tex3d_init(uint32_t base_bytes, uint32_t size_bytes)
{
    g_region_base = (base_bytes + 255) / 256;
    g_region_blocks = size_bytes / 256;
    g_next_block = 0;
    g_nent = 0;
    KH_INFO("tex", "3D texture cache: %u KiB of GS VRAM", (unsigned)(size_bytes / 1024));
}

void kh_tex3d_invalidate(void) { g_gen++; }
void kh_tex3d_new_frame(void) { g_frame++; }

static int log2i(int v) { int n = 0; while ((1 << n) < v) n++; return n; }

static u32 alloc_blocks(u32 n)
{
    u32 b;
    if (g_next_block + n > g_region_blocks) {
        /* full: start over; entries used this frame are rebuilt on demand */
        g_nent = 0;
        g_next_block = 0;
        g_used_bytes = 0;
        if (n > g_region_blocks)
            return 0xffffffffu;
    }
    b = g_next_block;
    g_next_block += (n + 31) & ~31u;       /* keep 8 KiB (page) alignment for later entries */
    g_used_bytes = g_next_block * 256;
    return g_region_base + b;
}

static u32 rgb555_to_32(u16 c, u32 a)
{
    u32 r = (c & 31) * 255 / 31, g = ((c >> 5) & 31) * 255 / 31, b = ((c >> 10) & 31) * 255 / 31;
    return r | (g << 8) | (b << 16) | (a << 24);
}

static inline int csm1(int i) { return (i & ~0x18) | ((i & 0x08) << 1) | ((i & 0x10) >> 1); }

/* scratch for converted data (kept until the frame packet has been sent) */
#define SCRATCH_BYTES (512 * 1024)
static u8 g_scratch[SCRATCH_BYTES] __attribute__((aligned(64)));
static u32 g_scratch_used;

void kh_tex3d_frame_sent(void) { g_scratch_used = 0; }

static void *scratch(u32 n)
{
    void *p;
    n = (n + 63) & ~63u;
    if (g_scratch_used + n > SCRATCH_BYTES)
        return NULL;
    p = g_scratch + g_scratch_used;
    g_scratch_used += n;
    return p;
}

/* 4x4 compressed: per 4x4 block a 32-bit texel word (2 bits/texel) in the texture slot and a 16-bit
 * palette-index word in the "index" region (slot 1 for textures in slot 0, slot 1 upper half for
 * slot 2), decoded to RGBA32. */
static u32 *decode_4x4(u32 ofs, int w, int h, u32 pltt_ofs)
{
    u32 *out = scratch((u32)w * (u32)h * 4);
    int bx, by, x, y;
    u32 idx_ofs = 0x20000 + ((ofs & 0x1ffff) >> 1) + ((ofs >= 0x40000) ? 0x10000 : 0);
    if (!out)
        return NULL;
    for (by = 0; by < h / 4; by++) {
        for (bx = 0; bx < w / 4; bx++) {
            u32 blk = (u32)(by * (w / 4) + bx);
            const u8 *tp = kh_nitro_view_ptr(VIEW_TEX, ofs + blk * 4);
            const u8 *ip = kh_nitro_view_ptr(VIEW_TEX, idx_ofs + blk * 2);
            u32 texels, pal[4];
            u16 idx;
            const u16 *pp;
            int mode;
            if (!tp || !ip)
                return out;
            texels = tp[0] | (tp[1] << 8) | ((u32)tp[2] << 16) | ((u32)tp[3] << 24);
            idx = (u16)(ip[0] | (ip[1] << 8));
            mode = idx >> 14;
            pp = (const u16 *)kh_nitro_view_ptr(VIEW_TEXPLTT, pltt_ofs + (u32)(idx & 0x3fff) * 4);
            if (!pp)
                return out;
            pal[0] = rgb555_to_32(pp[0], 0x80);
            pal[1] = rgb555_to_32(pp[1], 0x80);
            switch (mode) {
            case 0: pal[2] = rgb555_to_32(pp[2], 0x80); pal[3] = 0; break;
            case 1: {
                u16 c0 = pp[0], c1 = pp[1];
                int r = ((c0 & 31) + (c1 & 31)) / 2, g = (((c0 >> 5) & 31) + ((c1 >> 5) & 31)) / 2,
                    b = (((c0 >> 10) & 31) + ((c1 >> 10) & 31)) / 2;
                pal[2] = rgb555_to_32((u16)(r | g << 5 | b << 10), 0x80);
                pal[3] = 0;
                break;
            }
            case 2: pal[2] = rgb555_to_32(pp[2], 0x80); pal[3] = rgb555_to_32(pp[3], 0x80); break;
            default: {
                u16 c0 = pp[0], c1 = pp[1];
                int r2 = ((c0 & 31) * 5 + (c1 & 31) * 3) / 8, g2 = (((c0 >> 5) & 31) * 5 + ((c1 >> 5) & 31) * 3) / 8,
                    b2 = (((c0 >> 10) & 31) * 5 + ((c1 >> 10) & 31) * 3) / 8;
                int r3 = ((c0 & 31) * 3 + (c1 & 31) * 5) / 8, g3 = (((c0 >> 5) & 31) * 3 + ((c1 >> 5) & 31) * 5) / 8,
                    b3 = (((c0 >> 10) & 31) * 3 + ((c1 >> 10) & 31) * 5) / 8;
                pal[2] = rgb555_to_32((u16)(r2 | g2 << 5 | b2 << 10), 0x80);
                pal[3] = rgb555_to_32((u16)(r3 | g3 << 5 | b3 << 10), 0x80);
                break;
            }
            }
            for (y = 0; y < 4; y++)
                for (x = 0; x < 4; x++)
                    out[(by * 4 + y) * w + bx * 4 + x] = pal[(texels >> ((y * 4 + x) * 2)) & 3];
        }
    }
    return out;
}

/* Returns 1 and fills TEX0/CLAMP register values for the texture, 0 if it cannot be used. */
int kh_tex3d_bind(KhGsPacket *p, u32 teximage, u32 pltt, u64 *tex0_out, int *w_out, int *h_out)
{
    int fmt = (int)((teximage >> 26) & 7);
    int w = 8 << ((teximage >> 20) & 7), h = 8 << ((teximage >> 23) & 7);
    int c0t = (int)((teximage >> 29) & 1);
    u32 ofs = (teximage & 0xffff) << 3;
    u32 pofs = pltt << (fmt == 2 ? 3 : 4);
    u32 k0 = teximage & 0x3fffffffu, k1 = pltt;
    TexEntry *e = NULL;
    int i, psm, bpp;

    if (fmt == 0)
        return 0;
    for (i = 0; i < g_nent; i++)
        if (g_ent[i].key0 == k0 && g_ent[i].key1 == k1 && g_ent[i].gen == g_gen) {
            e = &g_ent[i];
            break;
        }
    if (!e) {
        const u8 *tex = kh_nitro_view_ptr(VIEW_TEX, ofs);
        const u16 *pal = (const u16 *)kh_nitro_view_ptr(VIEW_TEXPLTT, pofs);
        u32 bytes, blocks, tbp, cbp = 0;
        int tbw, ncl = 0;
        void *src = (void *)tex;
        u32 *clut = NULL;

        if (!tex)
            return 0;
        switch (fmt) {
        case 1: case 4: case 6: psm = GS_PSM_8; bpp = 8; ncl = 256; break;
        case 2: case 3: psm = GS_PSM_4; bpp = 4; ncl = 16; break;
        case 5: psm = GS_PSM_32; bpp = 32; break;
        default: psm = GS_PSM_16; bpp = 16; break;
        }
        if (ncl && !pal)
            return 0;
        tbw = (w + 63) / 64;
        if (bpp <= 8 && tbw < 2)
            tbw = 2;
        bytes = (u32)tbw * 64u * (u32)h * (u32)bpp / 8u;
        blocks = (bytes + 255) / 256 + (ncl ? 4 : 0);
        tbp = alloc_blocks(blocks);
        if (tbp == 0xffffffffu)
            return 0;
        if (ncl)
            cbp = tbp + (bytes + 255) / 256;

        if (fmt == 2) {                           /* 2 bpp -> 4 bpp */
            u8 *o = scratch((u32)w * (u32)h / 2);
            int n;
            if (!o)
                return 0;
            for (n = 0; n < w * h / 4; n++) {
                u8 b = tex[n];
                o[n * 2] = (u8)((b & 3) | ((b >> 2) & 3) << 4);
                o[n * 2 + 1] = (u8)(((b >> 4) & 3) | ((b >> 6) & 3) << 4);
            }
            src = o;
        } else if (fmt == 5) {
            src = decode_4x4(ofs, w, h, pofs);
            if (!src)
                return 0;
        }
        if (ncl) {
            clut = scratch(1024);
            if (!clut)
                return 0;
            memset(clut, 0, 1024);
            for (i = 0; i < ncl; i++) {
                u32 c;
                if (fmt == 1)
                    c = rgb555_to_32(pal[i & 31], ((i >> 5) * 0x80 + 3) / 7);
                else if (fmt == 6)
                    c = rgb555_to_32(pal[i & 7], ((i >> 3) * 0x80 + 15) / 31);
                else if (fmt == 2 && i >= 4)
                    c = 0;
                else
                    c = rgb555_to_32(pal[i], (c0t && i == 0) ? 0 : 0x80);
                clut[ncl == 256 ? csm1(i) : i] = c;
            }
        }
        /* texels and CLUT go by reference; the scratch buffers live until the frame is sent */
        FlushCache(0);
        kh_gs_upload_ref(p, src, (int)tbp, tbw, psm, 0, 0, w, h);
        if (ncl)
            kh_gs_upload_ref(p, clut, (int)cbp, 1, GS_PSM_32, 0, 0, ncl == 256 ? 16 : 8, ncl == 256 ? 16 : 2);
        kh_gs_packet_ad_begin(p, 1);
        kh_gs_packet_q(p, 0, GS_REG_TEXFLUSH);

        if (g_nent >= MAX_ENTRIES)
            g_nent = 0;
        e = &g_ent[g_nent++];
        e->key0 = k0;
        e->key1 = k1;
        e->gen = g_gen;
        e->tbp = tbp;
        e->cbp = cbp;
        e->psm = (u8)psm;
        e->tbw = (u8)tbw;
        e->tw = (u8)log2i(w);
        e->th = (u8)log2i(h);
        e->has_clut = ncl != 0;
    }
    e->last_frame = g_frame;
    if (teximage & (3u << 18))
        KH_UNIMPLEMENTED_ONCE("tex: mirrored repeat (drawn as plain repeat)");
    *tex0_out = GS_SET_TEX0(e->tbp, e->tbw, e->psm, e->tw, e->th, 1, 0,
                            e->cbp, GS_PSM_32, 0, 0, e->has_clut ? 1 : 0);
    *w_out = w;
    *h_out = h;
    return 1;
}
