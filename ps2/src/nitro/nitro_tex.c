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
extern uint32_t kh_nitro_view_banks(int view);

typedef struct TexEntry {
    u32 key0, key1;          /* teximage (without texcoord mode) | pltt | colour-0 flag */
    u32 gen;
    u32 tbp, cbp;            /* GS block addresses (64 words) */
    u8 psm, tbw, tw, th;
    u32 last_frame;
    int has_clut;
    u32 sum;                 /* checksum of the DS texels + palette the entry was built from */
    u32 fullsum;             /* the same over every word (sum samples large textures) */
    u32 pgsig;               /* VRAM write generations of the pages it was built from */
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
    g_next_block += n;                     /* footprints are exact (gs_footprint): no padding */
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

/* GS memory is swizzled: a page (8 KiB) is a grid of 32 blocks whose numbering is not row-major,
 * so a texture smaller than a page occupies a scattered set of a page's blocks - a 128x32 PSMT8
 * texture uses blocks 0-7 and 16-23.  Sizing textures linearly put the CLUT on blocks 16-19, over
 * texture columns 64-95 (the dashes across the title's "358/2 Days").  Footprint = highest block
 * touched + 1 for a texture inside one page, whole pages otherwise.
 *   PSMCT32 / PSMT8: 8 x 4 blocks per page; PSMCT16 / PSMT4: 4 x 8 blocks per page. */
static const u8 k_blk_8x4[4][8] = {
    { 0, 1, 4, 5, 16, 17, 20, 21 }, { 2, 3, 6, 7, 18, 19, 22, 23 },
    { 8, 9, 12, 13, 24, 25, 28, 29 }, { 10, 11, 14, 15, 26, 27, 30, 31 } };
static const u8 k_blk_4x8[8][4] = {
    { 0, 2, 8, 10 }, { 1, 3, 9, 11 }, { 4, 6, 12, 14 }, { 5, 7, 13, 15 },
    { 16, 18, 24, 26 }, { 17, 19, 25, 27 }, { 20, 22, 28, 30 }, { 21, 23, 29, 31 } };

/* blocks a w x h texture of psm occupies from its TBP0, and the TBW it is stored with */
static u32 gs_footprint(int psm, int w, int h, int *tbw_out)
{
    int pw, ph, bw, bh, wide, bx, by, nbx, nby, maxb = 0;
    switch (psm) {
    case GS_PSM_32: pw = 64;  ph = 32;  bw = 8;  bh = 8;  wide = 1; break;
    case GS_PSM_16: pw = 64;  ph = 64;  bw = 16; bh = 8;  wide = 0; break;
    case GS_PSM_8:  pw = 128; ph = 64;  bw = 16; bh = 16; wide = 1; break;
    default:        pw = 128; ph = 128; bw = 32; bh = 16; wide = 0; break;   /* PSMT4 */
    }
    if (w > pw || h > ph) {                  /* whole pages, TBW a multiple of the page width */
        int npx = (w + pw - 1) / pw, npy = (h + ph - 1) / ph;
        *tbw_out = npx * pw / 64;
        return (u32)(npx * npy) * 32u;
    }
    *tbw_out = pw / 64;
    nbx = (w + bw - 1) / bw;
    nby = (h + bh - 1) / bh;
    for (by = 0; by < nby; by++)
        for (bx = 0; bx < nbx; bx++) {
            int b = wide ? k_blk_8x4[by][bx] : k_blk_4x8[by][bx];
            if (b > maxb)
                maxb = b;
        }
    return (u32)maxb + 1;
}

/* Mirrored repeat (TEXIMAGE_PARAM bits 18/19 with repeat bits 16/17): the GS repeats but cannot
 * mirror, so the texture is stored with its mirror image beside / below it, twice as wide / high,
 * and repeated over that size - the DS pattern exactly.  Done once per cache miss. */
static void *mirror_texels(const void *src, int w, int h, int bpp, int ms, int mt)
{
    int W = ms ? 2 * w : w, H = mt ? 2 * h : h, x, y;
    u8 *out = scratch((u32)W * (u32)H * (u32)bpp / 8u);
    const u8 *in = src;
    if (!out)
        return NULL;
    for (y = 0; y < H; y++) {
        int sy = y < h ? y : 2 * h - 1 - y;
        for (x = 0; x < W; x++) {
            int sx = x < w ? x : 2 * w - 1 - x;
            if (bpp == 4) {
                int i = sy * w + sx, o = y * W + x;
                u8 t = (u8)((in[i >> 1] >> ((i & 1) * 4)) & 15);
                if (o & 1)
                    out[o >> 1] = (u8)((out[o >> 1] & 0x0f) | (t << 4));
                else
                    out[o >> 1] = (u8)((out[o >> 1] & 0xf0) | t);
            } else {
                int n = bpp / 8;
                memcpy(out + ((u32)y * (u32)W + (u32)x) * (u32)n, in + ((u32)sy * (u32)w + (u32)sx) * (u32)n, (size_t)n);
            }
        }
    }
    return out;
}

/* Converts and uploads a texture the cache does not hold; NULL if it cannot be used.  ms / mt:
 * mirrored repeat in s / t (the stored texture is doubled in that direction). */
static TexEntry *tex_miss(KhGsPacket *p, int fmt, int w, int h, int c0t, u32 ofs, u32 pofs, u32 k0, u32 k1,
                          int ms, int mt)
{
    const u8 *tex = kh_nitro_view_ptr(VIEW_TEX, ofs);
    const u16 *pal = (const u16 *)kh_nitro_view_ptr(VIEW_TEXPLTT, pofs);
    u32 bytes, blocks, tbp, cbp = 0;
    int tbw, ncl = 0, i, psm, bpp;
    void *src = (void *)tex;
    u32 *clut = NULL;
    TexEntry *e;

    if (!tex)
        return NULL;
    switch (fmt) {
    case 1: case 4: case 6: psm = GS_PSM_8; bpp = 8; ncl = 256; break;
    case 2: case 3: psm = GS_PSM_4; bpp = 4; ncl = 16; break;
    case 5: psm = GS_PSM_32; bpp = 32; break;
    default: psm = GS_PSM_16; bpp = 16; break;
    }
    if (ncl && !pal)
        return NULL;
    bytes = gs_footprint(psm, ms ? 2 * w : w, mt ? 2 * h : h, &tbw) * 256u;
    blocks = bytes / 256 + (ncl ? 4 : 0);       /* + the CLUT (16x16 PSMCT32: 4 blocks) */
    tbp = alloc_blocks(blocks);
    if (tbp == 0xffffffffu)
        return NULL;
    if (ncl)
        cbp = tbp + (bytes + 255) / 256;

    if (fmt == 2) {                           /* 2 bpp -> 4 bpp */
        u8 *o = scratch((u32)w * (u32)h / 2);
        int n;
        if (!o)
            return NULL;
        for (n = 0; n < w * h / 4; n++) {
            u8 b = tex[n];
            o[n * 2] = (u8)((b & 3) | ((b >> 2) & 3) << 4);
            o[n * 2 + 1] = (u8)(((b >> 4) & 3) | ((b >> 6) & 3) << 4);
        }
        src = o;
    } else if (fmt == 5) {
        src = decode_4x4(ofs, w, h, pofs);
        if (!src)
            return NULL;
    }
    if (ms || mt) {
        src = mirror_texels(src, w, h, bpp, ms, mt);
        if (!src)
            return NULL;
        if (ms) w *= 2;
        if (mt) h *= 2;
    }
    if (ncl) {
        clut = scratch(1024);
        if (!clut)
            return NULL;
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
        KH_PROF_ADD(KH_PC_CLUTS, 1);
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
    return e;
}

/* Checksum of what a texture is built from: its texels (every word for textures up to 4 KiB,
 * otherwise 256 words spread over the data) and its palette.  Texture VRAM also changes without
 * GX_LoadTex - VRAM-transfer tasks, writes through the LCDC mapping (eye textures are filled after
 * the model first draws with them: the cache kept them black) - so a cached texture is re-checked
 * against its source on its first use in each frame. */
/* sum of the write generations of the VRAM pages under [ofs, ofs + bytes) of a view */
static u32 page_sig(int view, u32 ofs, u32 bytes)
{
    u32 sig = 0, o;
    for (o = ofs & ~0x3fffu; o < ofs + bytes; o += 0x4000) {
        u32 v = kh_nitro_view_to_vram(view, o);
        if (v != 0xffffffffu && (v >> 14) < KH_VRAM_PAGES)
            sig += kh_vram_page_gen[v >> 14] * ((v >> 14) + 1);
    }
    return sig;
}

static u32 texture_sum_ex(int fmt, int w, int h, u32 ofs, u32 pofs, int full);
static u32 texture_sum(int fmt, int w, int h, u32 ofs, u32 pofs) { return texture_sum_ex(fmt, w, h, ofs, pofs, 0); }

static u32 texture_pages(int fmt, int w, int h, u32 ofs, u32 pofs)
{
    static const u8 k_bpp[8] = { 0, 8, 2, 4, 8, 2, 8, 16 };
    u32 bytes = (u32)w * (u32)h * k_bpp[fmt] / 8u;
    return page_sig(VIEW_TEX, ofs, bytes) + page_sig(VIEW_TEXPLTT, pofs, 512) * 7u;
}

static u32 texture_sum_ex(int fmt, int w, int h, u32 ofs, u32 pofs, int full)
{
    static const u8 k_bpp[8] = { 0, 8, 2, 4, 8, 2, 8, 16 };
    u32 bytes = (u32)w * (u32)h * k_bpp[fmt] / 8u, words = bytes / 4, step, i, sum = 0x811c9dc5u;
    u32 ncol = fmt == 1 ? 32 : fmt == 6 ? 8 : fmt == 2 ? 4 : fmt == 3 ? 16 : fmt == 4 ? 256 : fmt == 5 ? 0 : 0;
    const u8 *t = kh_nitro_view_ptr(VIEW_TEX, ofs), *tend = kh_nitro_view_ptr(VIEW_TEX, ofs + bytes - 4);
    const u8 *pl = ncol ? kh_nitro_view_ptr(VIEW_TEXPLTT, pofs) : NULL;
    if (!t)
        return 0;
    step = (full || words <= 1024) ? 1 : words / 256;
    if (tend == t + bytes - 4) {                 /* contiguous (one bank): read directly */
        const u32 *w32 = (const u32 *)t;
        for (i = 0; i < words; i += step)
            sum = (sum ^ w32[i]) * 0x01000193u;
    } else {
        for (i = 0; i < words; i += step) {
            const u8 *q = kh_nitro_view_ptr(VIEW_TEX, ofs + i * 4);
            if (q)
                sum = (sum ^ *(const u32 *)q) * 0x01000193u;
        }
    }
    if (fmt == 5) {                              /* 4x4: the palette-index words and the palette */
        u32 idx_ofs = 0x20000 + ((ofs & 0x1ffff) >> 1) + ((ofs >= 0x40000) ? 0x10000 : 0);
        const u8 *ix = kh_nitro_view_ptr(VIEW_TEX, idx_ofs);
        for (i = 0; ix && i < bytes / 8 && i < 1024; i += 4)
            sum = (sum ^ *(const u32 *)(ix + i)) * 0x01000193u;
        pl = kh_nitro_view_ptr(VIEW_TEXPLTT, pofs);
        ncol = 64;
    }
    for (i = 0; pl && i < ncol / 2; i++)
        sum = (sum ^ ((const u32 *)pl)[i]) * 0x01000193u;
    return sum;
}

/* Returns 1 and fills TEX0 for the texture, 0 if it cannot be used.  *w_out / *h_out: the size
 * the GS texture coordinates are normalised to (doubled in a mirrored direction). */
int kh_tex3d_bind(KhGsPacket *p, u32 teximage, u32 pltt, u64 *tex0_out, int *w_out, int *h_out)
{
    int fmt = (int)((teximage >> 26) & 7);
    int w = 8 << ((teximage >> 20) & 7), h = 8 << ((teximage >> 23) & 7);
    int c0t = (int)((teximage >> 29) & 1);
    int ms = ((teximage >> 16) & 1) && ((teximage >> 18) & 1) && w < 1024;
    int mt = ((teximage >> 17) & 1) && ((teximage >> 19) & 1) && h < 1024;
    u32 ofs = (teximage & 0xffff) << 3;
    u32 pofs = pltt << (fmt == 2 ? 3 : 4);
    /* key: what the stored texels depend on - not the repeat bits, nor a mirror bit without repeat
     * (bits 16-19), nor the texcoord transform mode (30-31) */
    u32 k0 = (teximage & 0x3ff0ffffu) | ((u32)ms << 18) | ((u32)mt << 19), k1 = pltt;
    TexEntry *e = NULL;
    int i;

    if (fmt == 0)
        return 0;
    for (i = 0; i < g_nent; i++)
        if (g_ent[i].key0 == k0 && g_ent[i].key1 == k1 && g_ent[i].gen == g_gen) {
            e = &g_ent[i];
            break;
        }
    /* first use this frame: a sampled check, and a full one if its VRAM pages were written (a
     * texture reloaded at the same place can differ only in words the sample skips: the pause
     * menu's buttons kept the previous screen's digits) */
    if (e && e->last_frame != g_frame) {
        u32 pg = texture_pages(fmt, w, h, ofs, pofs);
        int changed = texture_sum(fmt, w, h, ofs, pofs) != e->sum;
        if (!changed && pg != e->pgsig)
            changed = texture_sum_ex(fmt, w, h, ofs, pofs, 1) != e->fullsum;
        e->pgsig = pg;
        if (!changed)
            goto bound;
    } else if (e) {
        goto bound;
    }
    if (e) {
        KH_PROF_ADD(KH_PC_TEXSTALE, 1);
        e->gen = 0;                              /* the source changed: build it again */
        e = NULL;
    }
    if (!e) {
        KH_PROF_ADD(KH_PC_TEXMISS, 1);
        kh_prof_begin(KH_PROF_TEX);
        e = tex_miss(p, fmt, w, h, c0t, ofs, pofs, k0, k1, ms, mt);
        if (e) {
            e->sum = texture_sum(fmt, w, h, ofs, pofs);
            e->fullsum = texture_sum_ex(fmt, w, h, ofs, pofs, 1);
            e->pgsig = texture_pages(fmt, w, h, ofs, pofs);
        }
        kh_prof_end(KH_PROF_TEX);
        if (!e) {
            static u32 warned;
            if (!(warned & (1u << fmt))) {
                warned |= 1u << fmt;
                KH_WARN("tex", "cannot bind fmt %d %dx%d teximage %08x pltt %04x", fmt, w, h, (unsigned)teximage,
                        (unsigned)pltt);
            }
        }
        if (!e)
            return 0;
    }
bound:
    e->last_frame = g_frame;
    *tex0_out = GS_SET_TEX0(e->tbp, e->tbw, e->psm, e->tw, e->th, 1, 0,
                            e->cbp, GS_PSM_32, 0, 0, e->has_clut ? 1 : 0);
    *w_out = 1 << e->tw;
    *h_out = 1 << e->th;
    return 1;
}
