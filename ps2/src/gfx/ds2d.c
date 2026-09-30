/* DS 2D engines -> GS: the 2D compositor.
 *
 * Reads the DS display state the game produced this frame (kh_ds_io registers, kh_ds_vram banks
 * through the BG/OBJ views, kh_ds_pal, kh_ds_oam) and draws each engine's screen with native GS
 * primitives into a caller-chosen rectangle of the frame buffer:
 *
 *   characters  uploaded raw, no EE conversion: a BG's (or the OBJ area's) character data becomes
 *               a PSMT4/PSMT8 texture 128 texels wide made of 16 "tile columns" 8 texels wide,
 *               64 tiles each (tile t -> u = (t / 64) * 8, v = (t % 64) * 8).  Each column is one
 *               IMAGE transfer of 64 consecutive tiles, sent by reference from kh_ds_vram.
 *   palettes    DS BGR555 is GS PSMCT16 bit for bit except alpha: CLUTs are the DS palettes with
 *               bit 15 set on opaque entries (index 0 of a 16- or 256-colour palette stays
 *               transparent), stored in CSM1 order so a 4bpp tile picks its palette with CSA.
 *   BG text     one SPRITE per visible tile (flips by swapped UVs), clipped by the scissor.
 *   OBJ         one SPRITE per 8x8 tile of each sprite, 1D/2D character mapping, flips.
 *   order       DS priority: for priority 3..0, BGs of that priority (BG3..BG0), then OBJs.
 *   brightness  master brightness as a blended full-rectangle quad.
 *
 * Not yet: affine / extended (bitmap, 256x16) BGs, affine OBJs, windows, mosaic, colour special
 * effects other than master brightness, extended palettes.  Each is logged once when used.
 */
#include "platform/kh_platform.h"
#include "platform/ps2/ps2_gs.h"

#include <string.h>
#include <gs_gp.h>
#include <gs_psm.h>
#include <gif_tags.h>


extern unsigned char kh_ds_io[], kh_ds_pal[], kh_ds_oam[];
extern unsigned char *kh_nitro_view_ptr(int view, uint32_t ofs);
enum { VIEW_BG, VIEW_OBJ, VIEW_SUB_BG, VIEW_SUB_OBJ };

#define IO16(o) (*(volatile u16 *)(kh_ds_io + (o)))
#define IO32(o) (*(volatile u32 *)(kh_ds_io + (o)))

/* ------------------------------------------------------------ GS VRAM plan */

/* Per engine: 4 BG character textures + 1 OBJ texture (each 128x512 PSMT8-sized = 64 KiB,
 * enough for 1024 8bpp tiles) and 4 CLUTs (BG/OBJ x 4bpp/8bpp flavours).  Engine A then B.
 * Word addresses (GS VRAM / 4), block (64-word) aligned. */
#define TEXSZ_WORDS (65536 / 4)
#define CLUT_WORDS  (512 / 4)
static u32 g_base;                 /* words */
static int g_ready;

static u32 tex_addr(int eng, int slot) { return g_base + (u32)(eng * 5 + slot) * TEXSZ_WORDS; }
static u32 clut_addr(int eng, int which) { return g_base + 10 * TEXSZ_WORDS + (u32)(eng * 4 + which) * 256; }
u32 kh_ds2d_vram_words(void) { return 10 * TEXSZ_WORDS + 8 * 256; }

/* ---------------------------------------------------------------- output */

typedef struct Rect { float x, y, sx, sy; int w, h; } Rect;   /* screen origin, scale per DS pixel */

static inline void ad(KhGsPacket *p, u64 reg, u64 v) { kh_gs_packet_q(p, v, reg); }

static inline int gx(const Rect *r, float x) { return (int)((KH_GS_OFS + r->x + x * r->sx) * 16.0f); }
static inline int gy(const Rect *r, float y) { return (int)((KH_GS_OFS + r->y + y * r->sy) * 16.0f); }

/* ------------------------------------------------------------ palettes */

/* CSM1 order for a 256-entry 16-bit CLUT: swap bits 3 and 4 of the index. */
static inline int csm1(int i) { return (i & ~0x18) | ((i & 0x08) << 1) | ((i & 0x10) >> 1); }

static u16 g_clut[2][4][256] __attribute__((aligned(64)));   /* eng, {BG4,BG8,OBJ4,OBJ8} */

static void build_cluts(KhGsPacket *p, int eng)
{
    const u16 *bg = (const u16 *)(kh_ds_pal + eng * 0x400);
    const u16 *obj = bg + 256;
    int i;
    for (i = 0; i < 256; i++) {
        int d = csm1(i);
        u16 cb = bg[i] & 0x7fff, co = obj[i] & 0x7fff;
        g_clut[eng][0][d] = (u16)(cb | ((i & 15) ? 0x8000 : 0));
        g_clut[eng][1][d] = (u16)(cb | (i ? 0x8000 : 0));
        g_clut[eng][2][d] = (u16)(co | ((i & 15) ? 0x8000 : 0));
        g_clut[eng][3][d] = (u16)(co | (i ? 0x8000 : 0));
    }
    for (i = 0; i < 4; i++)
        kh_gs_upload_ref(p, g_clut[eng][i], (int)(clut_addr(eng, i) / 64), 1, GS_PSM_16, 0, 0, 16, 16);
}

/* ------------------------------------------------------------ characters */

/* Upload `ntiles` tiles starting at character offset `ofs` of `view` into texture slot. */
static void upload_chars(KhGsPacket *p, int view, u32 ofs, int ntiles, int bpp8, u32 tbp_words)
{
    int tsz = bpp8 ? 64 : 32;
    int col;
    if (ntiles > 1024)
        ntiles = 1024;
    for (col = 0; col * 64 < ntiles; col++) {
        int n = ntiles - col * 64 > 64 ? 64 : ntiles - col * 64;
        const u8 *src = kh_nitro_view_ptr(view, ofs + (u32)col * 64 * tsz);
        if (!src)
            return;
        kh_gs_upload_ref(p, src, (int)(tbp_words / 64), 2, bpp8 ? GS_PSM_8 : GS_PSM_4, col * 8, 0, 8, n * 8);
    }
}

static void set_texture(KhGsPacket *p, u32 tbp_words, int bpp8, u32 cbp_words, int csa)
{
    kh_gs_packet_ad_begin(p, 2);
    ad(p, GS_REG_TEX0_1, GS_SET_TEX0(tbp_words / 64, 2, bpp8 ? GS_PSM_8 : GS_PSM_4, 7, 9, 1, 0,
                                     cbp_words / 64, GS_PSM_16, 0, csa, 1));
    ad(p, GS_REG_TEX1_1, GS_SET_TEX1(0, 0, 0, 0, 0, 0, 0));
}

/* one 8x8 tile sprite (packed UV/XYZ2 pairs in an open REGLIST) */
static inline void tile_sprite(KhGsPacket *p, const Rect *r, int dx, int dy, int t, int hf, int vf)
{
    int u0 = (t >> 6) * 8, v0 = (t & 63) * 8;
    int u1 = u0 + 8, v1 = v0 + 8;
    if (hf) { int s = u0; u0 = u1; u1 = s; }
    if (vf) { int s = v0; v0 = v1; v1 = s; }
    kh_gs_packet_q(p, KH_PK_UV_LO(u0 << 4, v0 << 4), 0);
    kh_gs_packet_q(p, KH_PK_XYZ2_LO(gx(r, (float)dx), gy(r, (float)dy)), KH_PK_XYZ2_HI(0));
    kh_gs_packet_q(p, KH_PK_UV_LO(u1 << 4, v1 << 4), 0);
    kh_gs_packet_q(p, KH_PK_XYZ2_LO(gx(r, (float)(dx + 8)), gy(r, (float)(dy + 8))), KH_PK_XYZ2_HI(0));
}

#define SPRITE_REGS ((u64)GIF_REG_UV | ((u64)GIF_REG_XYZ2 << 4) | ((u64)GIF_REG_UV << 8) | ((u64)GIF_REG_XYZ2 << 12))

/* --------------------------------------------------------------- BG text */

typedef struct BgInfo { int enabled, prio, bpp8; u16 cnt; u32 char_ofs, scr_ofs; int maxtile; } BgInfo;

static void draw_text_bg(KhGsPacket *p, const Rect *r, int eng, int bg, const BgInfo *b)
{
    int view = eng ? VIEW_SUB_BG : VIEW_BG;
    int size = b->cnt >> 14;
    int mw = (size & 1) ? 64 : 32, mh = (size & 2) ? 64 : 32;
    int hofs = IO16((eng ? 0x1010 : 0x10) + bg * 4) & 0x1ff;
    int vofs = IO16((eng ? 0x1012 : 0x12) + bg * 4) & 0x1ff;
    const u16 *scr = (const u16 *)kh_nitro_view_ptr(view, b->scr_ofs);
    int ty, tx, count = 0, start;
    if (!scr)
        return;
    set_texture(p, tex_addr(eng, bg), b->bpp8, clut_addr(eng, b->bpp8), 0);

    /* palette changes per tile for 4bpp: group by palette with one texture state per group */
    for (int pal = 0; pal < (b->bpp8 ? 1 : 16); pal++) {
        start = (int)p->len;
        count = 0;
        if (!b->bpp8) {
            kh_gs_packet_ad_begin(p, 1);
            ad(p, GS_REG_TEX0_1, GS_SET_TEX0(tex_addr(eng, bg) / 64, 2, GS_PSM_4, 7, 9, 1, 0,
                                             clut_addr(eng, 0) / 64, GS_PSM_16, 0, pal, 1));
        }
        start = (int)p->len;
        kh_gs_packet_q(p, 0, 0);    /* GIFtag placeholder */
        for (ty = 0; ty <= 24; ty++) {
            int my = ((vofs >> 3) + ty) % mh;
            for (tx = 0; tx <= 32; tx++) {
                int mx = ((hofs >> 3) + tx) % mw;
                int blk = (mx >> 5) + ((mw == 64) ? (my >> 5) * 2 : (my >> 5));
                u16 e = scr[blk * 1024 + (my & 31) * 32 + (mx & 31)];
                int t = e & 0x3ff;
                if (!b->bpp8 && (e >> 12) != pal)
                    continue;
                if (t > b->maxtile)
                    continue;
                if (p->len + 8 > p->cap)
                    break;
                tile_sprite(p, r, tx * 8 - (hofs & 7), ty * 8 - (vofs & 7), t, (e >> 10) & 1, (e >> 11) & 1);
                count++;
            }
        }
        if (count) {
            u64 *tag = (u64 *)p->base + start * 2;
            tag[0] = GIF_SET_TAG(count, 1, 0, 0, GIF_FLG_PACKED, 4);
            tag[1] = SPRITE_REGS;
            kh_prof_count(1, (u32)count * 2, (u32)count * 2);
        } else {
            p->len = (u32)start;        /* nothing drawn with this palette */
        }
    }
}

/* ------------------------------------------------------------------ OBJ */

static const u8 k_obj_w[3][4] = { { 8, 16, 32, 64 }, { 16, 32, 32, 64 }, { 8, 8, 16, 32 } };
static const u8 k_obj_h[3][4] = { { 8, 16, 32, 64 }, { 8, 8, 16, 32 }, { 16, 32, 32, 64 } };

static void draw_objs(KhGsPacket *p, const Rect *r, int eng, int prio, int obj_bpp8_tiles)
{
    const u16 *oam = (const u16 *)(kh_ds_oam + eng * 0x400);
    u32 dispcnt = IO32(eng ? 0x1000 : 0);
    int map1d = (dispcnt >> 4) & 1;
    int shift1d = (dispcnt >> 20) & 3;     /* 1D boundary: 32 << shift bytes per tile unit */
    int i;
    (void)obj_bpp8_tiles;
    for (i = 127; i >= 0; i--) {
        u16 a0 = oam[i * 4], a1 = oam[i * 4 + 1], a2 = oam[i * 4 + 2];
        int shape = a0 >> 14, sz = a1 >> 14, w, h, x, y, bpp8, hf, vf, tile, pal, row, col, base;
        if (((a2 >> 10) & 3) != prio)
            continue;
        if (a0 & 0x100) {                      /* affine */
            KH_UNIMPLEMENTED_ONCE("ds2d: affine OBJ");
            continue;
        }
        if (a0 & 0x200)                        /* disabled */
            continue;
        if (shape == 3)
            continue;
        if (((a0 >> 10) & 3) >= 2) {           /* window / bitmap OBJ */
            KH_UNIMPLEMENTED_ONCE("ds2d: window/bitmap OBJ");
            continue;
        }
        w = k_obj_w[shape][sz];
        h = k_obj_h[shape][sz];
        y = a0 & 0xff;
        if (y >= 192)
            y -= 256;
        x = a1 & 0x1ff;
        if (x >= 256)
            x -= 512;
        bpp8 = (a0 >> 13) & 1;
        hf = (a1 >> 12) & 1;
        vf = (a1 >> 13) & 1;
        tile = a2 & 0x3ff;
        pal = a2 >> 12;

        /* tile index in our atlas: units of 32 bytes (4bpp tile); 8bpp textures hold 64-byte
         * tiles, so the atlas index is the byte offset / tile size */
        set_texture(p, tex_addr(eng, 4), bpp8, clut_addr(eng, bpp8 ? 3 : 2), bpp8 ? 0 : pal);
        {
            int n = (w / 8) * (h / 8), start = (int)p->len;
            kh_gs_packet_q(p, GIF_SET_TAG(n, 1, 0, 0, GIF_FLG_PACKED, 4), SPRITE_REGS);
            for (row = 0; row < h / 8; row++) {
                for (col = 0; col < w / 8; col++) {
                    int sr = vf ? h / 8 - 1 - row : row, sc = hf ? w / 8 - 1 - col : col;
                    u32 byteofs;
                    if (map1d)
                        byteofs = ((u32)tile << (5 + shift1d)) + (u32)(sr * (w / 8) + sc) * (bpp8 ? 64 : 32);
                    else
                        byteofs = (u32)tile * 32 + (u32)sr * 32 * 32 + (u32)sc * (bpp8 ? 64 : 32);
                    base = (int)(byteofs / (bpp8 ? 64 : 32));
                    tile_sprite(p, r, x + col * 8, y + row * 8, base, hf, vf);
                }
            }
            (void)start;
            kh_prof_count(1, (u32)n * 2, (u32)n * 2);
        }
    }
}

/* highest OBJ character offset any visible sprite uses, in bytes */
static u32 obj_char_extent(int eng)
{
    const u16 *oam = (const u16 *)(kh_ds_oam + eng * 0x400);
    u32 dispcnt = IO32(eng ? 0x1000 : 0);
    int map1d = (dispcnt >> 4) & 1, shift1d = (dispcnt >> 20) & 3;
    u32 ext = 0;
    int i;
    for (i = 0; i < 128; i++) {
        u16 a0 = oam[i * 4], a1 = oam[i * 4 + 1], a2 = oam[i * 4 + 2];
        int shape = a0 >> 14, sz = a1 >> 14;
        u32 end;
        if ((a0 & 0x300) == 0x200 || shape == 3)
            continue;
        end = map1d ? ((u32)(a2 & 0x3ff) << (5 + shift1d)) + (u32)k_obj_w[shape][sz] * k_obj_h[shape][sz]
                    : (u32)(a2 & 0x3ff) * 32 + 32 * 32 * (k_obj_h[shape][sz] / 8);
        if ((a0 >> 13) & 1)
            end += (u32)k_obj_w[shape][sz] * k_obj_h[shape][sz] / 2;
        if (end > ext)
            ext = end;
    }
    return ext > 0x10000 ? 0x10000 : ext;
}

/* ---------------------------------------------------------------- engine */

static void draw_engine(KhGsPacket *p, const Rect *r, int eng)
{
    u32 dispcnt = IO32(eng ? 0x1000 : 0);
    int mode = (dispcnt >> 16) & 3, bgmode = dispcnt & 7;
    BgInfo bg[4];
    int i, prio;

    kh_gs_packet_ad_begin(p, 2);
    ad(p, GS_REG_SCISSOR_1, GS_SET_SCISSOR((int)r->x, (int)(r->x + r->w - 1), (int)r->y, (int)(r->y + r->h - 1)));
    ad(p, GS_REG_TEST_1, GS_SET_TEST(1, 6, 0, 0, 0, 0, 1, 1));     /* alpha > 0, no Z */

    if (mode != 1 || (dispcnt & 0x80))     /* off, VRAM/main-memory display, or forced blank */
        return;

    build_cluts(p, eng);
    memset(bg, 0, sizeof bg);
    for (i = 0; i < 4; i++) {
        u16 cnt = IO16((eng ? 0x1008 : 0x8) + i * 2);
        int text = (i < 2) || (i == 2 && (bgmode == 0 || bgmode == 1 || bgmode == 3)) || (i == 3 && bgmode == 0);
        if (!(dispcnt & (0x100u << i)))
            continue;
        if (i == 0 && !eng && (dispcnt & 8))
            continue;                    /* BG0 shows the 3D scene: drawn by the 3D renderer */
        if (!text) {
            KH_UNIMPLEMENTED_ONCE("ds2d: affine/extended BG");
            continue;
        }
        bg[i].enabled = 1;
        bg[i].cnt = cnt;
        bg[i].prio = cnt & 3;
        bg[i].bpp8 = (cnt >> 7) & 1;
        bg[i].char_ofs = ((cnt >> 2) & 0xf) * 0x4000 + (eng ? 0 : ((dispcnt >> 24) & 7) * 0x10000);
        bg[i].scr_ofs = ((cnt >> 8) & 0x1f) * 0x800 + (eng ? 0 : ((dispcnt >> 27) & 7) * 0x10000);
        bg[i].maxtile = 1023;
        upload_chars(p, eng ? VIEW_SUB_BG : VIEW_BG, bg[i].char_ofs, 1024, bg[i].bpp8, tex_addr(eng, i));
    }
    if (dispcnt & 0x1000) {
        u32 ext = obj_char_extent(eng);
        int n4 = (int)(ext / 32), n8 = (int)(ext / 64);
        /* one atlas; 4bpp sprites index 32-byte tiles, 8bpp 64-byte tiles: upload as 4bpp, and
         * 8bpp sprites read the same bytes through a PSMT8 view of their own slot when used */
        if (n4)
            upload_chars(p, eng ? VIEW_SUB_OBJ : VIEW_OBJ, 0, n4, 0, tex_addr(eng, 4));
        (void)n8;
    }

    for (prio = 3; prio >= 0; prio--) {
        for (i = 3; i >= 0; i--)
            if (bg[i].enabled && bg[i].prio == prio)
                draw_text_bg(p, r, eng, i, &bg[i]);
        if (dispcnt & 0x1000)
            draw_objs(p, r, eng, prio, 0);
    }
}

static void draw_brightness(KhGsPacket *p, const Rect *r, int eng)
{
    u16 v = IO16(eng ? 0x106c : 0x6c);
    int mode = v >> 14, f = v & 0x1f, a, c;
    if (!mode || !f || mode == 3)
        return;
    if (f > 16)
        f = 16;
    a = f * 8;                               /* 0..128 */
    c = mode == 1 ? 0xff : 0x00;
    kh_gs_packet_ad_begin(p, 6);
    ad(p, GS_REG_TEST_1, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 1));
    ad(p, GS_REG_ALPHA_1, GS_SET_ALPHA(0, 1, 2, 1, 0));   /* (Cs - Cd) * As + Cd */
    ad(p, GS_REG_PRIM, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 0, 0, 1, 0, 0, 0, 0));
    ad(p, GS_REG_RGBAQ, GS_SET_RGBAQ(c, c, c, a, 0x3f800000));
    ad(p, GS_REG_XYZ2, GS_SET_XYZ(gx(r, 0), gy(r, 0), 0));
    ad(p, GS_REG_XYZ2, GS_SET_XYZ(gx(r, 256), gy(r, 192), 0));
}

void kh_ds2d_init(void)
{
    if (g_ready)
        return;
    g_base = (kh_gs_texpool_base() / 4 + 2047) & ~2047u;   /* page aligned */
    g_ready = 1;
    {
        extern void kh_tex3d_init(uint32_t base_bytes, uint32_t size_bytes);
        uint32_t tex_base = (g_base + kh_ds2d_vram_words()) * 4;
        kh_tex3d_init(tex_base, 4u * 1024u * 1024u - tex_base);
    }
    KH_INFO("ds2d", "2D compositor: %u KiB of GS VRAM at word 0x%x", (unsigned)(kh_ds2d_vram_words() * 4 / 1024),
            (unsigned)g_base);
}

/* Draw engine `eng` (0 = A, 1 = B) into the output rectangle (x, y, w, h). */
void kh_ds2d_draw(int eng, int x, int y, int w, int h)
{
    KhGsPacket *p = kh_gs_frame_packet();
    Rect r;
    kh_ds2d_init();
    r.x = (float)x;
    r.y = (float)y;
    r.w = w;
    r.h = h;
    r.sx = (float)w / 256.0f;
    r.sy = (float)h / 192.0f;

    kh_gs_packet_ad_begin(p, 4);
    ad(p, GS_REG_PRIM, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 1, 0, 0, 0, 1, 0, 0));
    ad(p, GS_REG_RGBAQ, GS_SET_RGBAQ(0x80, 0x80, 0x80, 0x80, 0x3f800000));
    ad(p, GS_REG_TEXA, GS_SET_TEXA(0, 1, 0x80));
    ad(p, GS_REG_CLAMP_1, GS_SET_CLAMP(0, 0, 0, 0, 0, 0));
    draw_engine(p, &r, eng);
    draw_brightness(p, &r, eng);

    /* back to full-screen state for whoever draws next */
    kh_gs_packet_ad_begin(p, 2);
    ad(p, GS_REG_SCISSOR_1, GS_SET_SCISSOR(0, kh_video_width() - 1, 0, kh_video_height() - 1));
    ad(p, GS_REG_TEST_1, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 2));
}
