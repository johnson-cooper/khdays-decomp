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
#include <stdio.h>
#include <math.h>
#include <gs_gp.h>
#include <gs_psm.h>
#include <gif_tags.h>


extern unsigned char kh_ds_io[], kh_ds_pal[], kh_ds_oam[];
extern unsigned char *kh_nitro_view_ptr(int view, uint32_t ofs);
extern uint32_t kh_nitro_view_banks(int view);
enum { VIEW_BG, VIEW_OBJ, VIEW_SUB_BG, VIEW_SUB_OBJ };

#define IO16(o) (*(volatile u16 *)(kh_ds_io + (o)))
#define IO32(o) (*(volatile u32 *)(kh_ds_io + (o)))

/* ------------------------------------------------------------ GS VRAM plan */

/* Per engine: 4 BG character textures + 2 OBJ textures (the OBJ characters once as PSMT4 for
 * 16-colour sprites, once as PSMT8 for 256-colour ones: the GS swizzles the two formats
 * differently, so one upload cannot serve both), each 128x512 PSMT8-sized = 64 KiB, enough for
 * 1024 8bpp tiles; then one page of 16-colour CLUTs per engine and the 256-colour CLUTs.
 * Engine A then B.  Word addresses (GS VRAM / 4), block (64-word) aligned. */
#define TEXSZ_WORDS (65536 / 4)
#define SLOTS       6              /* BG0..BG3, OBJ 4bpp, OBJ 8bpp */
/* The OBJ atlases hold the whole 128 KiB OBJ area (1D mapping with a 128- or 256-byte boundary
 * reaches past 64 KiB: the pause menu's buttons sit at 66 KiB): 128 tiles per 8-pixel column,
 * 1024 rows; 8bpp 2048 tiles = 128x1024 PSMT8, 4bpp 4096 tiles = 256x1024 PSMT4, 128 KiB each.
 * BG atlases: 64 tiles per column, 128x512, 64 KiB. */
#define OBJ_SZ_WORDS (2 * TEXSZ_WORDS)
#define ENG_WORDS    (4 * TEXSZ_WORDS + 2 * OBJ_SZ_WORDS)
#define CLUT_WORDS  (512 / 4)
static u32 g_base;                 /* words */
static int g_ready;
#define GS_VRAM_WORDS (4u * 1024u * 1024u / 4u)

static u32 tex_addr(int eng, int slot)
{
    return g_base + (u32)eng * ENG_WORDS + (slot < 4 ? (u32)slot * TEXSZ_WORDS : 4 * TEXSZ_WORDS + (u32)(slot - 4) * OBJ_SZ_WORDS);
}
/* tiles per atlas column, as a shift: 6 for BG atlases, 7 while OBJs are drawn */
static int g_colshift = 6;
/* CLUT sets: 0 = the palettes as they are, 1 = brightened (BLDCNT brightness increase) */
#define CLUT4_WORDS (4 * 2048)       /* one PSMCT16 page per engine and set: BG palettes 0-15, OBJ 0-15 */
#define CLUT8_WORDS (8 * 256)        /* 16x16 PSMCT16 CLUT: eng x set x {BG, OBJ} */
static int g_clut_set;               /* the set the layer being drawn uses */
static u32 clut4_page(int eng, int set) { return g_base + 2 * ENG_WORDS + (u32)(eng * 2 + set) * 2048; }
/* CBP (in 64-word blocks) of a CLUT: 8bpp = the 256-colour CLUT, 4bpp = palette `pal`'s own block */
static u32 clut_cbp(int eng, int obj, int bpp8, int pal)
{
    if (bpp8)
        return (g_base + 2 * ENG_WORDS + CLUT4_WORDS + (u32)((eng * 2 + g_clut_set) * 2 + obj) * 256) / 64;
    return clut4_page(eng, g_clut_set) / 64 + (u32)obj * 16 + (u32)pal;
}
/* one 256 KiB region for bitmap BGs (up to 512x256 direct colour), page aligned; reused by every
 * bitmap BG because each is uploaded right before it is drawn and the GS keeps packet order */
#define BITMAP_WORDS (256 * 1024 / 4)
static u32 bitmap_addr(void) { return (g_base + 2 * ENG_WORDS + CLUT4_WORDS + CLUT8_WORDS + 2047) & ~2047u; }
/* display capture: a 256x192 PSMCT16 image (12 pages) right after the bitmap region */
#define CAPTURE_WORDS (256 * 192 / 2)
static u32 capture_addr(void) { return bitmap_addr() + BITMAP_WORDS; }
/* extended palettes: one 256-colour CLUT per engine, reloaded before each use (TEX0.CLD copies
 * a CLUT into the GS when TEX0 is written, so the block can take the next palette right after) */
static u32 extclut_addr(int eng) { return capture_addr() + CAPTURE_WORDS + (u32)eng * 256; }
u32 kh_ds2d_vram_words(void) { return capture_addr() - g_base + CAPTURE_WORDS + 2 * 256; }

/* Prove the fixed 2D reservation before the first frame uses it.  All addresses are GS words;
 * this catches overlap with framebuffer/Z/font space, overlap between 2D slots, arithmetic
 * wraparound and any end beyond the physical 4 MiB GS VRAM. */
static u32 vram_span(const char *name, u32 start, u32 words, u32 cursor)
{
    if (start < cursor)
        kh_panic("GS VRAM overlap at %s: 0x%x < previous end 0x%x",
                 name, (unsigned)start, (unsigned)cursor);
    if (start > GS_VRAM_WORDS || words > GS_VRAM_WORDS - start)
        kh_panic("GS VRAM range %s exceeds 4 MiB: 0x%x + 0x%x words",
                 name, (unsigned)start, (unsigned)words);
    return start + words;
}

static u32 validate_vram_layout(void)
{
    u32 fixed_end = (kh_gs_texpool_base() + 3u) / 4u;
    u32 cursor = g_base, end;
    int eng, slot;

    if (g_base < fixed_end)
        kh_panic("2D GS VRAM overlaps fixed framebuffer/Z/font area: 0x%x < 0x%x",
                 (unsigned)g_base, (unsigned)fixed_end);
    if (g_base & 2047u)
        kh_panic("2D GS VRAM base 0x%x is not page aligned", (unsigned)g_base);

    for (eng = 0; eng < 2; eng++) {
        for (slot = 0; slot < SLOTS; slot++) {
            u32 words = slot < 4 ? TEXSZ_WORDS : OBJ_SZ_WORDS;
            char name[20];
            snprintf(name, sizeof name, "eng%d texture%d", eng, slot);
            cursor = vram_span(name, tex_addr(eng, slot), words, cursor);
        }
    }
    cursor = vram_span("4bpp CLUTs", g_base + 2 * ENG_WORDS, CLUT4_WORDS, cursor);
    cursor = vram_span("8bpp CLUTs", g_base + 2 * ENG_WORDS + CLUT4_WORDS, CLUT8_WORDS, cursor);
    cursor = vram_span("bitmap scratch", bitmap_addr(), BITMAP_WORDS, cursor);
    cursor = vram_span("display capture", capture_addr(), CAPTURE_WORDS, cursor);
    cursor = vram_span("extended CLUT A", extclut_addr(0), 256, cursor);
    cursor = vram_span("extended CLUT B", extclut_addr(1), 256, cursor);

    end = g_base + kh_ds2d_vram_words();
    if (end < g_base || end != cursor || end > GS_VRAM_WORDS)
        kh_panic("2D GS VRAM end mismatch: calculated 0x%x, spans 0x%x, limit 0x%x",
                 (unsigned)end, (unsigned)cursor, (unsigned)GS_VRAM_WORDS);
    return end;
}

/* ---------------------------------------------------------------- output */

typedef struct Rect { float x, y, sx, sy; int w, h; } Rect;   /* screen origin, scale per DS pixel */

static inline void ad(KhGsPacket *p, u64 reg, u64 v) { kh_gs_packet_q(p, v, reg); }

static inline int gx(const Rect *r, float x) { return (int)((KH_GS_OFS + r->x + x * r->sx) * 16.0f); }
static inline int gy(const Rect *r, float y) { return (int)((KH_GS_OFS + r->y + y * r->sy) * 16.0f); }

/* The part of the DS screen the layer being drawn may cover (DS pixels, x1/y1 exclusive): the
 * whole screen, or one rectangle of a window region (draw_engine). */
typedef struct ClipRect { int x0, y0, x1, y1; } ClipRect;
static ClipRect g_clip = { 0, 0, 256, 192 };

/* SCISSOR for DS rectangle [x0, x1) x [y0, y1) of screen rectangle r; 0 if nothing is left */
static int scissor_for(const Rect *r, int x0, int y0, int x1, int y1, u64 *out)
{
    int sx0, sy0, sx1, sy1;
    if (x0 < g_clip.x0) x0 = g_clip.x0;
    if (y0 < g_clip.y0) y0 = g_clip.y0;
    if (x1 > g_clip.x1) x1 = g_clip.x1;
    if (y1 > g_clip.y1) y1 = g_clip.y1;
    if (x0 >= x1 || y0 >= y1)
        return 0;
    /* the GS fills a pixel when its (integer) coordinate lies in [start, end) of a primitive, so
     * at a non-integer scale the rows/columns of DS [a, b) are ceil(a)..ceil(b)-1, in 1/16 pixel
     * steps as gx/gy produce them: cut cells the same way, or a row at a cell edge is drawn by
     * neither cell's layers (a line of backdrop colour across the screen) */
    sx0 = (gx(r, (float)x0) - (KH_GS_OFS << 4) + 15) >> 4;
    sy0 = (gy(r, (float)y0) - (KH_GS_OFS << 4) + 15) >> 4;
    sx1 = ((gx(r, (float)x1) - (KH_GS_OFS << 4) + 15) >> 4) - 1;
    sy1 = ((gy(r, (float)y1) - (KH_GS_OFS << 4) + 15) >> 4) - 1;
    if (sx0 > sx1 || sy0 > sy1)
        return 0;
    *out = GS_SET_SCISSOR(sx0, sx1, sy0, sy1);
    return 1;
}

/* ------------------------------------------------------------ palettes */

/* CLUTs.  DS BGR555 is GS PSMCT16 bit for bit except alpha: entries get bit 15 when opaque (index
 * 0 of a 16- or 256-colour palette stays transparent).
 *   256 colours: one 16x16 CSM1 CLUT (index bits 3 and 4 swapped in the image).
 *   16 colours: every palette in its own 64-word block, used with CSA 0.  For a 4-bit texture,
 *   TEX0.CLD loads the 16 entries at CBP (an 8x2 rectangle) to CLUT position CSA, so CSA does not
 *   select a palette out of a bigger CLUT: CBP has to point at the palette itself.  (Using CSA did
 *   that wrong: every 4bpp tile and sprite was drawn with palette 0.)  The 32 palettes of an engine
 *   (BG 0-15 in blocks 0-15, OBJ in 16-31) fill one PSMCT16 page, uploaded as one 64x64 image:
 *   palette p sits at the origin of block p, whose place in the page is k_blk16[p]. */
static inline int csm1(int i) { return (i & ~0x18) | ((i & 0x08) << 1) | ((i & 0x10) >> 1); }

/* PSMCT16 page: 4x8 blocks of 16x8 pixels; block number -> (column, row) */
static const u8 k_blk16_col[32] = { 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3,
                                    0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3 };
static const u8 k_blk16_row[32] = { 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3,
                                    4, 5, 4, 5, 6, 7, 6, 7, 4, 5, 4, 5, 6, 7, 6, 7 };

static u16 g_clut8[2][2][2][256] __attribute__((aligned(64)));    /* eng, set, {BG, OBJ} */
static u16 g_clut4[2][2][64 * 64] __attribute__((aligned(64)));   /* eng, set: one page image */

/* DS brightness increase: each component c -> c + (31 - c) * EVY / 16 */
static inline u16 brighten(u16 c, int evy)
{
    int r = c & 31, g = (c >> 5) & 31, b = (c >> 10) & 31;
    r += ((31 - r) * evy) >> 4;
    g += ((31 - g) * evy) >> 4;
    b += ((31 - b) * evy) >> 4;
    return (u16)(r | g << 5 | b << 10);
}

/* Build and upload CLUT set `set` of an engine (set 1: brightened by evy). */
static void build_cluts(KhGsPacket *p, int eng, int set, int evy)
{
    const u16 *bg = (const u16 *)(kh_ds_pal + eng * 0x400);
    int i, obj, saved = g_clut_set;
    u16 *c4 = g_clut4[eng][set];
    g_clut_set = set;
    for (obj = 0; obj < 2; obj++) {
        const u16 *pal = bg + obj * 256;
        u16 *c8 = g_clut8[eng][set][obj];
        for (i = 0; i < 256; i++) {
            u16 c = pal[i] & 0x7fff;
            int blk = obj * 16 + (i >> 4), j = i & 15;
            if (set)
                c = brighten(c, evy);
            c8[csm1(i)] = (u16)(c | (i ? 0x8000 : 0));
            /* 8x2 at the block origin: entries 0-7 on row 0, 8-15 on row 1 */
            c4[(k_blk16_row[blk] * 8 + (j >> 3)) * 64 + k_blk16_col[blk] * 16 + (j & 7)] = (u16)(c | (j ? 0x8000 : 0));
        }
        kh_gs_upload_ref(p, c8, (int)clut_cbp(eng, obj, 1, 0), 1, GS_PSM_16, 0, 0, 16, 16);
    }
    kh_gs_upload_ref(p, c4, (int)(clut4_page(eng, set) / 64), 1, GS_PSM_16, 0, 0, 64, 64);
    g_clut_set = saved;
    KH_PROF_ADD(KH_PC_CLUTS, 3);
}

/* TEX0 for a character texture whose CLUT is at block `cbp` */
static void set_tex0(KhGsPacket *p, u32 tbp_words, int tbw, int psm, int tw, int th, u32 cbp)
{
    kh_gs_packet_ad_begin(p, 1);
    ad(p, GS_REG_TEX0_1, GS_SET_TEX0(tbp_words / 64, tbw, psm, tw, th, 1, 0, cbp, GS_PSM_16, 0, 0, 1));
}

/* Extended palettes (DISPCNT bit 30 BG, bit 31 OBJ): 256-colour BGs and OBJs pick one of 16
 * palettes of 256 colours (BG: per screen entry, in the BG's slot; OBJ: OAM palette field).  The
 * palette is converted into a frame-lifetime buffer and uploaded into the engine's ext CLUT block
 * right before the TEX0 that uses it. */
#define EXT_POOL 96
static u16 g_extbuf[EXT_POOL][256] __attribute__((aligned(64)));
static int g_extn;
static u32 g_ext_last[2];               /* key of the palette in each engine's block (0: none) */

#if KH_PS2_DEBUG
/* Development: write 1 to kh_ds2d_dbg (PINE / debugger) to log every OBJ drawn in the next frame */
volatile u32 kh_ds2d_dbg;
static int g_dbg_frame;
#endif
void kh_ds2d_frame_begin(void)
{
    g_extn = 0;
    g_ext_last[0] = g_ext_last[1] = 0;
#if KH_PS2_DEBUG
    g_dbg_frame = kh_ds2d_dbg & 1;
    kh_ds2d_dbg &= ~1u;
#endif
}

#if KH_PS2_DEBUG
/* Sparse transition trace: Engine B is the lower DS screen in the title flow.  Log only state
 * changes, so a hardware log can place a failure before/after the New Game register rewrite
 * without turning real-device logging into a per-frame timing problem. */
static void trace_engine_b_state(void)
{
    static u32 last_dispcnt = 0xffffffffu;
    static u32 last_bg = 0xffffffffu;
    static u32 last_banks = 0xffffffffu;
    u32 dispcnt = IO32(0x1000);
    u32 bg = (u32)IO16(0x100a) | ((u32)IO16(0x100e) << 16);
    u32 banks = kh_nitro_view_banks(VIEW_SUB_BG) |
                (kh_nitro_view_banks(VIEW_SUB_OBJ) << 16);
    if (dispcnt != last_dispcnt || bg != last_bg || banks != last_banks) {
        KH_INFO("engB", "DISPCNT=%08x BG1CNT=%04x BG3CNT=%04x banks BG=%03x OBJ=%03x",
                (unsigned)dispcnt, (unsigned)(bg & 0xffff), (unsigned)(bg >> 16),
                (unsigned)(banks & 0xffff), (unsigned)(banks >> 16));
        last_dispcnt = dispcnt;
        last_bg = bg;
        last_banks = banks;
    }
}
#endif

static u32 ext_clut(KhGsPacket *p, int eng, int obj, int slot, int pal)
{
    /* nitro_gx.c views: BGEXT 6, OBJEXT 7, SUB_BGEXT 8, SUB_OBJEXT 9 */
    int view = obj ? (eng ? 9 : 7) : (eng ? 8 : 6);
    u32 key = 0x80000000u | (u32)obj << 16 | (u32)slot << 8 | (u32)pal << 1 | (u32)g_clut_set;
    const u16 *src = (const u16 *)kh_nitro_view_ptr(view, (obj ? 0 : (u32)slot * 0x2000) + (u32)pal * 0x200);
    u16 *buf;
    int i;
    if (!src || g_extn >= EXT_POOL) {
        KH_UNIMPLEMENTED_ONCE("ds2d: extended palette not mapped / pool full (standard palette used)");
        return clut_cbp(eng, obj, 1, 0);
    }
    if (g_ext_last[eng] == key)
        return extclut_addr(eng) / 64;
    buf = g_extbuf[g_extn++];
    for (i = 0; i < 256; i++) {
        u16 c = src[i] & 0x7fff;
        if (g_clut_set) {
            u16 bld = IO16((eng ? 0x1000 : 0) + 0x50);
            int evy = IO16((eng ? 0x1000 : 0) + 0x54) & 31;
            (void)bld;
            c = brighten(c, evy > 16 ? 16 : evy);
        }
        buf[csm1(i)] = (u16)(c | (i ? 0x8000 : 0));
    }
    kh_gs_upload_ref(p, buf, (int)(extclut_addr(eng) / 64), 1, GS_PSM_16, 0, 0, 16, 16);
    g_ext_last[eng] = key;
    KH_PROF_ADD(KH_PC_CLUTS, 1);
    return extclut_addr(eng) / 64;
}

/* CLUT for an OBJ: an extended palette for 256-colour sprites when OBJ extended palettes are on */
static u32 obj_cbp(KhGsPacket *p, int eng, int bpp8, int pal)
{
    if (bpp8 && (IO32(eng ? 0x1000 : 0) & 0x80000000u))
        return ext_clut(p, eng, 1, 0, pal);
    return clut_cbp(eng, 1, bpp8, pal);
}

/* ------------------------------------------------------------ characters */

/* Upload `ntiles` tiles starting at character offset `ofs` of `view` into texture slot. */
static void upload_chars_ex(KhGsPacket *p, int view, u32 ofs, int ntiles, int bpp8, u32 tbp_words, int colshift,
                            int tbw, int maxtiles)
{
    int tsz = bpp8 ? 64 : 32, per = 1 << colshift;
    int col;
    if (ntiles > maxtiles)
        ntiles = maxtiles;
    for (col = 0; col * per < ntiles; col++) {
        int n = ntiles - col * per > per ? per : ntiles - col * per;
        const u8 *src = kh_nitro_view_ptr(view, ofs + (u32)col * (u32)per * (u32)tsz);
        if (!src)
            return;
        kh_gs_upload_ref(p, src, (int)(tbp_words / 64), tbw, bpp8 ? GS_PSM_8 : GS_PSM_4, col * 8, 0, 8, n * 8);
    }
}

static void upload_chars(KhGsPacket *p, int view, u32 ofs, int ntiles, int bpp8, u32 tbp_words)
{
    upload_chars_ex(p, view, ofs, ntiles, bpp8, tbp_words, 6, 2, 1024);
}

static void obj_texture(KhGsPacket *p, int eng, int bpp8, u32 cbp);
static void set_texture(KhGsPacket *p, u32 tbp_words, int bpp8, u32 cbp)
{
    set_tex0(p, tbp_words, 2, bpp8 ? GS_PSM_8 : GS_PSM_4, 7, 9, cbp);
    kh_gs_packet_ad_begin(p, 1);
    ad(p, GS_REG_TEX1_1, GS_SET_TEX1(0, 0, 0, 0, 0, 0, 0));
}

static void obj_texture(KhGsPacket *p, int eng, int bpp8, u32 cbp)
{
    set_tex0(p, tex_addr(eng, bpp8 ? 5 : 4), bpp8 ? 2 : 4, bpp8 ? GS_PSM_8 : GS_PSM_4, bpp8 ? 7 : 8, 10, cbp);
    kh_gs_packet_ad_begin(p, 1);
    ad(p, GS_REG_TEX1_1, GS_SET_TEX1(0, 0, 0, 0, 0, 0, 0));
}

/* UV is 14 bits of 12.4 texels: the far edge of the last tile row of a 1024-row atlas (1024.0)
 * would wrap to 0 and sample the whole column backwards - clamp to 1023.9375 */
#define UV14(v) ((v) > 0x3fff ? 0x3fff : (v))

/* One 8x8 tile sprite in an open REGLIST (UV, XYZ2, UV, XYZ2: two qwords).  x0/y0/x1/y1 are GS
 * 12.4 screen coordinates. */
static inline void tile_sprite_xy(KhGsPacket *p, int x0, int y0, int x1, int y1, int t, int hf, int vf)
{
    int u0 = (t >> g_colshift) * 8, v0 = (t & ((1 << g_colshift) - 1)) * 8;
    int u1 = u0 + 8, v1 = v0 + 8;
    if (hf) { int s = u0; u0 = u1; u1 = s; }
    if (vf) { int s = v0; v0 = v1; v1 = s; }
    kh_gs_packet_q(p, GS_SET_UV(UV14(u0 << 4), UV14(v0 << 4)), GS_SET_XYZ(x0, y0, 0));
    kh_gs_packet_q(p, GS_SET_UV(UV14(u1 << 4), UV14(v1 << 4)), GS_SET_XYZ(x1, y1, 0));
}

static inline void tile_sprite(KhGsPacket *p, const Rect *r, int dx, int dy, int t, int hf, int vf)
{
    tile_sprite_xy(p, gx(r, (float)dx), gy(r, (float)dy), gx(r, (float)(dx + 8)), gy(r, (float)(dy + 8)), t, hf, vf);
}

#define SPRITE_REGS ((u64)GIF_REG_UV | ((u64)GIF_REG_XYZ2 << 4) | ((u64)GIF_REG_UV << 8) | ((u64)GIF_REG_XYZ2 << 12))
#define SPRITE_TAG(n) GIF_SET_TAG((n), 1, 0, 0, GIF_FLG_REGLIST, 4)

/* --------------------------------------------------------------- BG text */

typedef struct BgInfo { int enabled, prio, bpp8; u16 cnt; u32 char_ofs, scr_ofs; int maxtile; } BgInfo;

/* One pass over the visible 33x25 screen entries: tiles are bucketed by palette (4bpp tiles pick
 * their palette with TEX0.CSA, so each palette in use costs one TEX0 and one GIF tag), screen
 * coordinates come from per-column / per-row tables. */
static void draw_text_bg(KhGsPacket *p, const Rect *r, int eng, int bg, const BgInfo *b)
{
    enum { COLS = 33, ROWS = 25 };
    static u16 ent[COLS * ROWS], pos[COLS * ROWS];
    int view = eng ? VIEW_SUB_BG : VIEW_BG;
    int size = b->cnt >> 14;
    int mw = (size & 1) ? 64 : 32, mh = (size & 2) ? 64 : 32;
    int hofs = IO16((eng ? 0x1010 : 0x10) + bg * 4) & 0x1ff;
    int vofs = IO16((eng ? 0x1012 : 0x12) + bg * 4) & 0x1ff;
    const u16 *scr = (const u16 *)kh_nitro_view_ptr(view, b->scr_ofs);
    int xs[COLS + 1], ys[ROWS + 1];
    int count[16], first[17];
    int extpal = b->bpp8 && (IO32(eng ? 0x1000 : 0) & 0x40000000u);
    int slot = (bg < 2 && (b->cnt & 0x2000)) ? bg + 2 : bg;   /* BG0/BG1 may use slot 2/3 */
    int bypal = !b->bpp8 || extpal;                            /* tiles pick a palette */
    int tx, ty, i, n = 0, npal = bypal ? 16 : 1, pal;
    if (!scr)
        return;
    for (tx = 0; tx <= COLS; tx++)
        xs[tx] = gx(r, (float)(tx * 8 - (hofs & 7)));
    for (ty = 0; ty <= ROWS; ty++)
        ys[ty] = gy(r, (float)(ty * 8 - (vofs & 7)));

    /* gather (bucket counts first, then a stable fill) */
    memset(count, 0, sizeof count);
    for (ty = 0; ty < ROWS; ty++) {
        int my = ((vofs >> 3) + ty) & (mh - 1);
        const u16 *row = scr + ((mw == 64) ? (my >> 5) * 2048 : (my >> 5) * 1024) + (my & 31) * 32;
        int py = ty * 8 - (vofs & 7);
        if (py + 8 <= g_clip.y0 || py >= g_clip.y1)
            continue;
        for (tx = 0; tx < COLS; tx++) {
            int mx = ((hofs >> 3) + tx) & (mw - 1), px = tx * 8 - (hofs & 7);
            u16 e;
            if (px + 8 <= g_clip.x0 || px >= g_clip.x1)
                continue;
            e = row[(mx >> 5) * 1024 + (mx & 31)];
            if ((e & 0x3ff) > b->maxtile)
                continue;
            ent[n] = e;
            pos[n] = (u16)(ty << 8 | tx);
            count[bypal ? e >> 12 : 0]++;
            n++;
        }
    }
    if (!n)
        return;
    {
        static u16 sent[COLS * ROWS], spos[COLS * ROWS];
        int at[16];
        first[0] = 0;
        for (pal = 0; pal < 16; pal++)
            first[pal + 1] = first[pal] + count[pal];
        memcpy(at, first, sizeof at);
        for (i = 0; i < n; i++) {
            int k = at[bypal ? ent[i] >> 12 : 0]++;
            sent[k] = ent[i];
            spos[k] = pos[i];
        }
        if (p->len + 8 + (u32)npal * 16 + (u32)n * 2 > p->cap)
            return;
        set_texture(p, tex_addr(eng, bg), b->bpp8, clut_cbp(eng, 0, b->bpp8, 0));
        for (pal = 0; pal < npal; pal++) {
            int c = count[pal];
            if (!c)
                continue;
            if (!b->bpp8)
                set_tex0(p, tex_addr(eng, bg), 2, GS_PSM_4, 7, 9, clut_cbp(eng, 0, 0, pal));
            else if (extpal)
                set_tex0(p, tex_addr(eng, bg), 2, GS_PSM_8, 7, 9, ext_clut(p, eng, 0, slot, pal));
            kh_gs_packet_q(p, SPRITE_TAG(c), SPRITE_REGS);
            for (i = first[pal]; i < first[pal] + c; i++) {
                u16 e = sent[i];
                int px = spos[i] & 0xff, py = spos[i] >> 8;
                tile_sprite_xy(p, xs[px], ys[py], xs[px + 1], ys[py + 1], e & 0x3ff, (e >> 10) & 1, (e >> 11) & 1);
            }
        }
        KH_PROF_ADD(KH_PC_SPRITES, n);
    }
}

/* -------------------------------------------------- colour special effects */

static int g_abe;   /* alpha blending on for the layer being drawn (PRIM.ABE) */

/* BLDCNT for one layer (bit 0-3 BG0-3, 4 OBJ): textures are MODULATEd by RGBAQ, so the effect
 * is the vertex colour/alpha plus the GS blend.  Alpha blending: fragment alpha EVA*8,
 * (Cs - Cd) * As + Cd, exact when EVA + EVB = 16 (the usual fade); darken: colour * (16 - EVY) / 16.
 * Transparent texels keep alpha 0 and stay discarded by the alpha test. */
static void layer_effect(KhGsPacket *p, int eng, int bit, int effects_on)
{
    u16 bld = IO16((eng ? 0x1000 : 0) + 0x50), alpha = IO16((eng ? 0x1000 : 0) + 0x52);
    int evy = IO16((eng ? 0x1000 : 0) + 0x54) & 31, mode = (bld >> 6) & 3;
    int c = 0x80, a = 0x80;

    g_abe = 0;
    g_clut_set = 0;
    if (effects_on && (bld & (1u << bit))) {
        if (mode == 1) {
            int eva = alpha & 31;
            a = (eva > 16 ? 16 : eva) * 8;
            g_abe = 1;
        } else if (mode == 3) {
            c = 0x80 * (16 - (evy > 16 ? 16 : evy)) / 16;
        } else if (mode == 2 && evy) {
            g_clut_set = 1;           /* the brightened CLUTs (built in draw_engine) */
        }
    }
    kh_gs_packet_ad_begin(p, 3);
    ad(p, GS_REG_RGBAQ, GS_SET_RGBAQ(c, c, c, a, 0x3f800000));
    ad(p, GS_REG_ALPHA_1, GS_SET_ALPHA(0, 1, 0, 1, 0));            /* (Cs - Cd) * As + Cd */
    ad(p, GS_REG_PRIM, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 1, 0, g_abe, 0, 1, 0, 0));
}

/* --------------------------------------------------------- bitmap BGs */

static int log2i(int v) { int n = 0; while ((1 << n) < v) n++; return n; }

/* An extended BG in one of its bitmap forms (BGxCNT bit 7 set: bit 2 clear = 256 colours through
 * the BG palette, set = direct colour with bit 15 = opaque), drawn through its affine transform:
 * texture (u, v) = reference + (PA x + PB y, PC x + PD y) for screen pixel (x, y). */
static void draw_bitmap_bg(KhGsPacket *p, const Rect *r, int eng, int bg, u16 cnt)
{
    static const short k_w[4] = { 128, 256, 512, 512 }, k_h[4] = { 128, 256, 256, 512 };
    int direct = (cnt >> 2) & 1, size = cnt >> 14;
    int W = k_w[size], H = k_h[size], bpp = direct ? 2 : 1;
    u32 io = (eng ? 0x1000 : 0) + (bg == 2 ? 0x20 : 0x30), rows, row, chunk;
    u32 base = ((cnt >> 8) & 0x1f) * 0x4000;
    s32 pa = (s16)IO16(io), pb = (s16)IO16(io + 2), pc = (s16)IO16(io + 4), pd = (s16)IO16(io + 6);
    s32 xr = (s32)(IO32(io + 8) << 4) >> 4, yr = (s32)(IO32(io + 12) << 4) >> 4;
    int psm = direct ? GS_PSM_16 : GS_PSM_8, i;
    static const float k_sx[4] = { 0, 256, 0, 256 }, k_sy[4] = { 0, 0, 192, 192 };

    if (W * H * bpp > 256 * 1024) {
        KH_UNIMPLEMENTED_ONCE("ds2d: bitmap BG larger than 256 KiB");
        return;
    }
    /* upload in pieces of at most 16 KiB of rows, each through the bank mapping; rows past the
     * mapped VRAM (a 256x256 bitmap at 0x8000 of a 128 KiB bank: the movie player shows only the
     * first 160) are left as they are - the DS would read unmapped space there */
    chunk = 16384 / (W * bpp);
    for (row = 0; row < (u32)H; row += rows) {
        const u8 *src = kh_nitro_view_ptr(eng ? VIEW_SUB_BG : VIEW_BG, base + row * W * bpp);
        rows = (u32)H - row < chunk ? (u32)H - row : chunk;
        if (!src)
            break;
        kh_gs_upload_ref(p, src, (int)(bitmap_addr() / 64), W / 64, psm, 0, (int)row, W, (int)rows);
    }
    set_tex0(p, bitmap_addr(), W / 64, psm, log2i(W), log2i(H), clut_cbp(eng, 0, 1, 0));
    kh_gs_packet_ad_begin(p, 3);
    ad(p, GS_REG_TEX1_1, GS_SET_TEX1(0, 0, 0, 0, 0, 0, 0));
    ad(p, GS_REG_CLAMP_1, (cnt & 0x2000) ? GS_SET_CLAMP(0, 0, 0, 0, 0, 0)
                                         : GS_SET_CLAMP(1, 1, 0, 0, 0, 0));
    ad(p, GS_REG_PRIM, GS_SET_PRIM(GS_PRIM_TRIANGLE_STRIP, 0, 1, 0, g_abe, 0, 0, 0, 0));   /* ST */
    kh_gs_packet_ad_begin(p, 8);
    if (cnt & 0x2000) {
        /* wrapping: the whole screen, texture coordinates repeat */
        for (i = 0; i < 4; i++) {
            float u = ((float)xr + pa * k_sx[i] + pb * k_sy[i]) / 256.0f;
            float v = ((float)yr + pc * k_sx[i] + pd * k_sy[i]) / 256.0f;
            union { float f; u32 u; } s, t;
            s.f = u / (float)W;
            t.f = v / (float)H;
            ad(p, GS_REG_ST, (u64)s.u | ((u64)t.u << 32));
            ad(p, GS_REG_XYZ2, GS_SET_XYZ(gx(r, k_sx[i]), gy(r, k_sy[i]), 0));
        }
    } else {
        /* not wrapping: transparent outside the bitmap, so draw the bitmap's own quad, its
         * corners mapped to the screen through the inverse transform (the movie player places
         * its 160-line frames 16 lines down this way) */
        float fa = pa / 256.0f, fb = pb / 256.0f, fc = pc / 256.0f, fd = pd / 256.0f;
        float det = fa * fd - fb * fc, X = xr / 256.0f, Y = yr / 256.0f;
        static const float k_u[4] = { 0, 1, 0, 1 }, k_v[4] = { 0, 0, 1, 1 };
        if (det == 0.0f)
            det = 1.0f;
        for (i = 0; i < 4; i++) {
            float du = k_u[i] * W - X, dv = k_v[i] * H - Y;
            float sx = (fd * du - fb * dv) / det, sy = (fa * dv - fc * du) / det;
            union { float f; u32 u; } s, t;
            s.f = k_u[i];
            t.f = k_v[i];
            ad(p, GS_REG_ST, (u64)s.u | ((u64)t.u << 32));
            ad(p, GS_REG_XYZ2, GS_SET_XYZ(gx(r, sx), gy(r, sy), 0));
        }
    }
    /* back to the sprite/UV state the tile layers use */
    kh_gs_packet_ad_begin(p, 2);
    ad(p, GS_REG_CLAMP_1, GS_SET_CLAMP(0, 0, 0, 0, 0, 0));
    ad(p, GS_REG_PRIM, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 1, 0, g_abe, 0, 1, 0, 0));
    KH_PROF_ADD(KH_PC_SPRITES, 1);
}

/* An affine BG (8-bit map entries: tile only) or an extended affine BG in its tiled form (16-bit
 * entries: tile, flips, palette), both 256-colour tiles in an N x N tile map (N = 16 << size).
 * The DS samples BG point  (X, Y) + (PA x + PB y, PC x + PD y)  for screen pixel (x, y)
 * (BGxX/BGxY 20.8, PA..PD 8.8), wrapping around the map when BGxCNT bit 13 is set, transparent
 * outside it otherwise.  Here every map tile the screen can see is drawn as two textured
 * triangles at its inverse-mapped screen position; the screen-rectangle scissor does the rest. */
#define AFFINE_BG_MAX_TILES 2048
static void draw_affine_bg(KhGsPacket *p, const Rect *r, int eng, int bg, const BgInfo *b, int extended)
{
    static const float k_cx[4] = { 0, 256, 0, 256 }, k_cy[4] = { 0, 0, 192, 192 };
    u32 io = (eng ? 0x1000 : 0) + (bg == 2 ? 0x20 : 0x30);
    int N = 16 << (b->cnt >> 14), wrap = (b->cnt >> 13) & 1;
    float pa = (float)(s16)IO16(io) / 256.0f, pb = (float)(s16)IO16(io + 2) / 256.0f;
    float pc = (float)(s16)IO16(io + 4) / 256.0f, pd = (float)(s16)IO16(io + 6) / 256.0f;
    float X = (float)((s32)(IO32(io + 8) << 4) >> 4) / 256.0f, Y = (float)((s32)(IO32(io + 12) << 4) >> 4) / 256.0f;
    float det = pa * pd - pb * pc, ia, ib, ic, id, minx = 1e9f, miny = 1e9f, maxx = -1e9f, maxy = -1e9f;
    int view = eng ? VIEW_SUB_BG : VIEW_BG, tx0, ty0, tx1, ty1, tx, ty, k, n = 0;
    const u8 *map8 = NULL;
    const u16 *map16 = NULL;
    u32 tagpos;

    if (det == 0.0f)
        return;
    if (extended)
        map16 = (const u16 *)kh_nitro_view_ptr(view, b->scr_ofs);
    else
        map8 = kh_nitro_view_ptr(view, b->scr_ofs);
    if (!map8 && !map16)
        return;
    ia = pd / det; ib = -pb / det; ic = -pc / det; id = pa / det;
    for (k = 0; k < 4; k++) {            /* the BG region under the screen */
        float bx = X + pa * k_cx[k] + pb * k_cy[k], by = Y + pc * k_cx[k] + pd * k_cy[k];
        if (bx < minx) minx = bx;
        if (bx > maxx) maxx = bx;
        if (by < miny) miny = by;
        if (by > maxy) maxy = by;
    }
    tx0 = (int)floorf(minx / 8.0f); tx1 = (int)floorf(maxx / 8.0f);
    ty0 = (int)floorf(miny / 8.0f); ty1 = (int)floorf(maxy / 8.0f);
    if (!wrap) {
        if (tx0 < 0) tx0 = 0;
        if (ty0 < 0) ty0 = 0;
        if (tx1 > N - 1) tx1 = N - 1;
        if (ty1 > N - 1) ty1 = N - 1;
    }
    if (tx1 < tx0 || ty1 < ty0)
        return;
    if ((tx1 - tx0 + 1) * (ty1 - ty0 + 1) > AFFINE_BG_MAX_TILES) {
        KH_UNIMPLEMENTED_ONCE("ds2d: affine BG zoomed out beyond the tile budget");
        return;
    }
    if (p->len + 8 + (u32)(tx1 - tx0 + 1) * (u32)(ty1 - ty0 + 1) * 6 > p->cap)
        return;
    if (extended && (IO32(eng ? 0x1000 : 0) & 0x40000000u))
        KH_UNIMPLEMENTED_ONCE("ds2d: BG extended palettes (drawn with the standard palette)");
    set_texture(p, tex_addr(eng, bg), 1, clut_cbp(eng, 0, 1, 0));
    tagpos = p->len;
    kh_gs_packet_q(p, 0, 0);
    for (ty = ty0; ty <= ty1; ty++) {
        for (tx = tx0; tx <= tx1; tx++) {
            static const u8 k_tri[6] = { 0, 1, 2, 1, 3, 2 };   /* corners: 0 TL, 1 TR, 2 BL, 3 BR */
            int mx = tx & (N - 1), my = ty & (N - 1), t, hf = 0, vf = 0, u0, v0;
            int Xs[4], Ys[4], U[4], V[4];
            if (map16) {
                u16 e = map16[my * N + mx];
                t = e & 0x3ff;
                hf = (e >> 10) & 1;
                vf = (e >> 11) & 1;
            } else {
                t = map8[my * N + mx];
            }
            u0 = (t >> 6) * 8;
            v0 = (t & 63) * 8;
            for (k = 0; k < 4; k++) {
                float bx = (float)(tx * 8 + (k & 1) * 8) - X, by = (float)(ty * 8 + (k >> 1) * 8) - Y;
                Xs[k] = gx(r, ia * bx + ib * by);
                Ys[k] = gy(r, ic * bx + id * by);
                U[k] = (u0 + (((k & 1) ^ hf) * 8)) << 4;
                V[k] = (v0 + (((k >> 1) ^ vf) * 8)) << 4;
            }
            for (k = 0; k < 6; k += 2) {
                int c0 = k_tri[k], c1 = k_tri[k + 1];
                kh_gs_packet_q(p, GS_SET_UV(U[c0], V[c0]), GS_SET_XYZ(Xs[c0], Ys[c0], 0));
                kh_gs_packet_q(p, GS_SET_UV(U[c1], V[c1]), GS_SET_XYZ(Xs[c1], Ys[c1], 0));
            }
            n++;
        }
    }
    {
        u64 *tag = (u64 *)p->base + tagpos * 2;
        tag[0] = GIF_SET_TAG(n * 6, 1, 1, GS_SET_PRIM(GS_PRIM_TRIANGLE, 0, 1, 0, g_abe, 0, 1, 0, 0), GIF_FLG_REGLIST, 2);
        tag[1] = (u64)GIF_REG_UV | ((u64)GIF_REG_XYZ2 << 4);
    }
    kh_gs_packet_ad_begin(p, 1);
    ad(p, GS_REG_PRIM, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 1, 0, g_abe, 0, 1, 0, 0));
    KH_PROF_ADD(KH_PC_SPRITES, n);
}

/* ------------------------------------------------------------------ OBJ */

static const u8 k_obj_w[3][4] = { { 8, 16, 32, 64 }, { 16, 32, 32, 64 }, { 8, 8, 16, 32 } };
static const u8 k_obj_h[3][4] = { { 8, 16, 32, 64 }, { 8, 8, 16, 32 }, { 16, 32, 32, 64 } };

/* character atlas tile of sprite tile (row, col) */
static inline int obj_tile(int tile, int row, int col, int w, int bpp8, int map1d, int shift1d)
{
    u32 byteofs;
    if (map1d)
        byteofs = ((u32)tile << (5 + shift1d)) + (u32)(row * (w / 8) + col) * (bpp8 ? 64 : 32);
    else
        byteofs = (u32)tile * 32 + (u32)row * 32 * 32 + (u32)col * (bpp8 ? 64 : 32);
    return (int)(byteofs / (bpp8 ? 64 : 32));
}

/* An affine OBJ.  The DS maps every screen pixel of the sprite's box (w x h, or 2w x 2h with the
 * double-size bit) back into the sprite through the matrix of affine group n (PA PB PC PD, 8.8):
 *     texel = M x (pixel - box centre) + (w/2, h/2),
 * texels outside w x h are transparent, nothing is drawn outside the box.  Here the inverse
 * matrix maps each 8x8 tile of the sprite forward to the screen; the tile is drawn as two textured
 * triangles (UVs = its place in the character atlas) and the scissor is the box, so the DS clip is
 * exact (the box is axis-aligned).  Flip bits do not exist in affine mode (bits 9-13 = n). */
static void draw_affine_obj(KhGsPacket *p, const Rect *r, int eng, const u16 *oam, u16 a0, u16 a1, u16 a2,
                            int map1d, int shift1d)
{
    int shape = a0 >> 14, sz = a1 >> 14, dbl = (a0 >> 9) & 1;
    int w = k_obj_w[shape][sz], h = k_obj_h[shape][sz], bw = dbl ? 2 * w : w, bh = dbl ? 2 * h : h;
    int x = a1 & 0x1ff, y = a0 & 0xff, n = (a1 >> 9) & 31, bpp8 = (a0 >> 13) & 1;
    int tile = a2 & 0x3ff, pal = a2 >> 12, row, col;
    u64 box, clip;
    float pa = (float)(s16)oam[(n * 4 + 0) * 4 + 3] / 256.0f, pb = (float)(s16)oam[(n * 4 + 1) * 4 + 3] / 256.0f;
    float pc = (float)(s16)oam[(n * 4 + 2) * 4 + 3] / 256.0f, pd = (float)(s16)oam[(n * 4 + 3) * 4 + 3] / 256.0f;
    float det = pa * pd - pb * pc, ia, ib, ic, id, cx, cy;
    int ntiles = (w / 8) * (h / 8);

    if (det == 0.0f)
        return;                           /* every pixel samples one texel row/column: degenerate */
    if (y >= 192)
        y -= 256;
    if (x >= 256)
        x -= 512;
    ia = pd / det; ib = -pb / det; ic = -pc / det; id = pa / det;
    cx = (float)x + (float)bw * 0.5f;
    cy = (float)y + (float)bh * 0.5f;

    /* scissor: the box, inside the clip rectangle */
    if (!scissor_for(r, x, y, x + bw, y + bh, &box) || !scissor_for(r, 0, 0, 256, 192, &clip))
        return;
    if (p->len + 8 + (u32)ntiles * 6 > p->cap)
        return;

    obj_texture(p, eng, bpp8, obj_cbp(p, eng, bpp8, pal));
    kh_gs_packet_ad_begin(p, 1);
    ad(p, GS_REG_SCISSOR_1, box);
    /* triangles, flat colour (the layer's RGBAQ), UV coordinates, blending as the layer */
    kh_gs_packet_q(p, GIF_SET_TAG(ntiles * 6, 1, 1, GS_SET_PRIM(GS_PRIM_TRIANGLE, 0, 1, 0, g_abe, 0, 1, 0, 0),
                                  GIF_FLG_REGLIST, 2),
                   (u64)GIF_REG_UV | ((u64)GIF_REG_XYZ2 << 4));
    for (row = 0; row < h / 8; row++) {
        for (col = 0; col < w / 8; col++) {
            static const u8 k_tri[6] = { 0, 1, 2, 1, 3, 2 };   /* corners: 0 TL, 1 TR, 2 BL, 3 BR */
            int t = obj_tile(tile, row, col, w, bpp8, map1d, shift1d);
            int u0 = (t >> 7) * 8, v0 = (t & 127) * 8, k;
            int X[4], Y[4], U[4], V[4];
            for (k = 0; k < 4; k++) {
                float tx = (float)(col * 8 + (k & 1) * 8) - (float)w * 0.5f;
                float ty = (float)(row * 8 + (k >> 1) * 8) - (float)h * 0.5f;
                X[k] = gx(r, cx + ia * tx + ib * ty);
                Y[k] = gy(r, cy + ic * tx + id * ty);
                U[k] = UV14((u0 + (k & 1) * 8) << 4);
                V[k] = UV14((v0 + (k >> 1) * 8) << 4);
            }
            for (k = 0; k < 6; k += 2) {
                int c0 = k_tri[k], c1 = k_tri[k + 1];
                kh_gs_packet_q(p, GS_SET_UV(U[c0], V[c0]), GS_SET_XYZ(X[c0], Y[c0], 0));
                kh_gs_packet_q(p, GS_SET_UV(U[c1], V[c1]), GS_SET_XYZ(X[c1], Y[c1], 0));
            }
        }
    }
    /* back to the clip scissor and the tile layers' PRIM */
    kh_gs_packet_ad_begin(p, 2);
    ad(p, GS_REG_SCISSOR_1, clip);
    ad(p, GS_REG_PRIM, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 1, 0, g_abe, 0, 1, 0, 0));
    KH_PROF_ADD(KH_PC_SPRITES, ntiles);
}

/* Bitmap OBJs (OBJ mode 3): direct-colour sprites, bit 15 = opaque, read straight from OBJ VRAM.
 * DISPCNT bit 6 clear: the 2D layout, a bitmap 128 (bit 5 clear) or 256 (bit 5 set) pixels wide
 * where the character number is the 8x8 cell (x = n % cols, y = n / cols); the rows a sprite
 * needs are uploaded once per pass into a 256-row image.  Bit 6 set: the 1D layout, w x h pixels
 * from character n << (7 + DISPCNT bit 22), uploaded per sprite.  The movie player shows every
 * other frame this way (a grid of 64x64 bitmap OBJs). */
#define OBJBMP_ADDR() (bitmap_addr() + BITMAP_WORDS / 2)
typedef struct ObjBmpPass { u32 rows[8]; u32 cursor; } ObjBmpPass;

static void draw_bitmap_obj(KhGsPacket *p, const Rect *r, int eng, ObjBmpPass *pass, u16 a0, u16 a1, u16 a2)
{
    u32 dispcnt = IO32(eng ? 0x1000 : 0);
    int shape = a0 >> 14, sz = a1 >> 14, w = k_obj_w[shape][sz], h = k_obj_h[shape][sz];
    int x = a1 & 0x1ff, y = a0 & 0xff, tile = a2 & 0x3ff, alpha = a2 >> 12;
    int view = eng ? VIEW_SUB_OBJ : VIEW_OBJ, u0, v0, u1, v1, row;
    u32 tbp;
    int tbw, tw, th;

    if (alpha == 0)
        return;
    if (alpha != 15)
        KH_UNIMPLEMENTED_ONCE("ds2d: translucent bitmap OBJ (drawn opaque)");
    if (y >= 192)
        y -= 256;
    if (x >= 256)
        x -= 512;
    if (dispcnt & 0x40) {                                       /* 1D */
        u32 src = (u32)tile << (7 + ((dispcnt >> 22) & 1)), bytes = (u32)w * h * 2;
        const u8 *s;
        if (pass->cursor + 64 * 2 * (u32)h > BITMAP_WORDS * 4 / 2)
            return;
        tbp = OBJBMP_ADDR() + pass->cursor / 4;
        for (row = 0; row < h; row += 64) {
            int n = h - row < 64 ? h - row : 64;
            s = kh_nitro_view_ptr(view, src + (u32)row * w * 2);
            if (!s)
                return;
            kh_gs_upload_ref(p, s, (int)(tbp / 64), 1, GS_PSM_16, 0, row, w, n);
        }
        (void)bytes;
        pass->cursor += (64 * 2 * (u32)h + 8191) & ~8191u;
        tbw = 1; tw = 6; th = log2i(h);
        u0 = 0; v0 = 0;
    } else {                                                    /* 2D, 128 or 256 wide */
        int wide = (dispcnt >> 5) & 1, cols = wide ? 32 : 16, W = cols * 8;
        u0 = (tile % cols) * 8;
        v0 = (tile / cols) * 8;
        tbp = OBJBMP_ADDR();
        for (row = v0; row < v0 + h && row < 256; row++) {
            int end = row;
            if (pass->rows[row >> 5] & (1u << (row & 31)))
                continue;
            while (end < v0 + h && end < 256 && end - row < 16384 / (W * 2) &&
                   !(pass->rows[end >> 5] & (1u << (end & 31)))) {
                pass->rows[end >> 5] |= 1u << (end & 31);
                end++;
            }
            {
                const u8 *s = kh_nitro_view_ptr(view, (u32)row * W * 2);
                if (!s)
                    return;
                kh_gs_upload_ref(p, s, (int)(tbp / 64), W / 64, GS_PSM_16, 0, row, W, end - row);
            }
            row = end - 1;
        }
        tbw = W / 64; tw = log2i(W); th = 8;
    }
    u1 = u0 + w;
    v1 = v0 + h;
    if (a1 & 0x1000) { int t = u0; u0 = u1; u1 = t; }
    if (a1 & 0x2000) { int t = v0; v0 = v1; v1 = t; }
    set_tex0(p, tbp, tbw, GS_PSM_16, tw, th, 0);
    kh_gs_packet_ad_begin(p, 1);
    ad(p, GS_REG_TEX1_1, GS_SET_TEX1(0, 0, 0, 0, 0, 0, 0));
    kh_gs_packet_q(p, SPRITE_TAG(1), SPRITE_REGS);
    kh_gs_packet_q(p, GS_SET_UV(u0 << 4, v0 << 4), GS_SET_XYZ(gx(r, (float)x), gy(r, (float)y), 0));
    kh_gs_packet_q(p, GS_SET_UV(u1 << 4, v1 << 4), GS_SET_XYZ(gx(r, (float)(x + w)), gy(r, (float)(y + h)), 0));
    KH_PROF_ADD(KH_PC_SPRITES, 1);
}

static void draw_objs(KhGsPacket *p, const Rect *r, int eng, int prio, int obj_bpp8_tiles)
{
    ObjBmpPass bmp;
    const u16 *oam = (const u16 *)(kh_ds_oam + eng * 0x400);
    u32 dispcnt = IO32(eng ? 0x1000 : 0);
    int map1d = (dispcnt >> 4) & 1;
    int shift1d = (dispcnt >> 20) & 3;     /* 1D boundary: 32 << shift bytes per tile unit */
    int i;
    (void)obj_bpp8_tiles;
    memset(&bmp, 0, sizeof bmp);
    for (i = 127; i >= 0; i--) {
        u16 a0 = oam[i * 4], a1 = oam[i * 4 + 1], a2 = oam[i * 4 + 2];
        int shape = a0 >> 14, sz = a1 >> 14, w, h, x, y, bpp8, hf, vf, tile, pal, row, col, base;
        if (((a2 >> 10) & 3) != prio)
            continue;
        if (a0 & 0x100) {                      /* affine (bit 9: double-size box) */
            if (((a0 >> 10) & 3) >= 2) {
                KH_UNIMPLEMENTED_ONCE("ds2d: affine window/bitmap OBJ");
                continue;
            }
            if (shape != 3)
                draw_affine_obj(p, r, eng, oam, a0, a1, a2, map1d, shift1d);
            continue;
        }
        if (a0 & 0x200)                        /* disabled */
            continue;
        if (shape == 3)
            continue;
        if (((a0 >> 10) & 3) == 3) {           /* bitmap OBJ */
            draw_bitmap_obj(p, r, eng, &bmp, a0, a1, a2);
            continue;
        }
        if (((a0 >> 10) & 3) == 2) {           /* OBJ window */
            KH_UNIMPLEMENTED_ONCE("ds2d: window OBJ");
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
        obj_texture(p, eng, bpp8, obj_cbp(p, eng, bpp8, pal));
        {
            int n = (w / 8) * (h / 8), start = (int)p->len;
#if KH_PS2_DEBUG
            if (g_dbg_frame)
                KH_INFO("ds2d", "obj eng %d #%d prio %d at %d,%d %dx%d %s tile %d pal %d map1d %d shift %d tex %x",
                        eng, i, prio, x, y, w, h, bpp8 ? "8bpp" : "4bpp", tile, pal, map1d, shift1d,
                        (unsigned)tex_addr(eng, bpp8 ? 5 : 4));
#endif
            if (p->len + 4 + (u32)n * 2 > p->cap)
                return;
            kh_gs_packet_q(p, SPRITE_TAG(n), SPRITE_REGS);
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
            KH_PROF_ADD(KH_PC_SPRITES, n);
        }
    }
}

/* highest OBJ character offset any visible sprite uses, in bytes */
static u32 obj_char_extent(int eng, int *any8)
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
        if ((a0 & 0x300) == 0x200 || shape == 3 || ((a0 >> 10) & 3) == 3)
            continue;
        end = map1d ? ((u32)(a2 & 0x3ff) << (5 + shift1d)) + (u32)k_obj_w[shape][sz] * k_obj_h[shape][sz]
                    : (u32)(a2 & 0x3ff) * 32 + 32 * 32 * (k_obj_h[shape][sz] / 8);
        if ((a0 >> 13) & 1) {
            end += (u32)k_obj_w[shape][sz] * k_obj_h[shape][sz] / 2;
            *any8 = 1;
        }
        if (end > ext)
            ext = end;
    }
    return ext > 0x20000 ? 0x20000 : ext;
}

/* ---------------------------------------------------------------- engine */

/* DS windows.  WIN0 and WIN1 are rectangles (WINxH: x1 << 8 | x2, WINxV: y1 << 8 | y2, x2/y2
 * exclusive; x1 > x2 wraps around the screen edge); a pixel belongs to WIN0 if inside it, else to
 * WIN1, else to the outside region.  WININ/WINOUT give each region the layers shown in it (bits
 * 0-4: BG0-3, OBJ) and whether colour effects apply there (bit 5).  The screen is cut along the
 * window edges into cells, each cell takes its region's bits, and cells with equal bits next to
 * each other on a row are merged.  (The OBJ window - sprites in mode 2 shaping a region - is
 * per-pixel and is not drawn.) */
typedef struct WinRect { ClipRect c; u8 mask; } WinRect;
#define MAX_WIN_RECTS 32

static int win_inside(int v, int a, int b) { return a <= b ? (v >= a && v < b) : (v >= a || v < b); }

static int window_rects(int eng, WinRect *out)
{
    u32 dispcnt = IO32(eng ? 0x1000 : 0);
    u32 io = eng ? 0x1000 : 0;
    int on[2] = { (int)(dispcnt >> 13) & 1, (int)(dispcnt >> 14) & 1 };
    int wx1[2], wx2[2], wy1[2], wy2[2], xs[6], ys[6], nx = 0, ny = 0, i, j, k, n = 0;
    u16 winin = IO16(io + 0x48), winout = IO16(io + 0x4a);

    if (!on[0] && !on[1]) {
        if (dispcnt & 0x8000)
            KH_UNIMPLEMENTED_ONCE("ds2d: OBJ window");
        out[0].c.x0 = 0; out[0].c.y0 = 0; out[0].c.x1 = 256; out[0].c.y1 = 192;
        out[0].mask = 0x3f;
        return 1;
    }
    if (dispcnt & 0x8000)
        KH_UNIMPLEMENTED_ONCE("ds2d: OBJ window");
    xs[nx++] = 0; xs[nx++] = 256;
    ys[ny++] = 0; ys[ny++] = 192;
    for (k = 0; k < 2; k++) {
        u16 h = IO16(io + 0x40 + k * 2), v = IO16(io + 0x44 + k * 2);
        wx1[k] = h >> 8; wx2[k] = h & 0xff;
        wy1[k] = v >> 8; wy2[k] = v & 0xff;
        if (!on[k])
            continue;
        xs[nx++] = wx1[k]; xs[nx++] = wx2[k];
        ys[ny++] = wy1[k] > 192 ? 192 : wy1[k]; ys[ny++] = wy2[k] > 192 ? 192 : wy2[k];
    }
    /* sort the cut positions (tiny insertion sorts), drop duplicates */
    for (i = 1; i < nx; i++) for (j = i; j > 0 && xs[j] < xs[j - 1]; j--) { int t = xs[j]; xs[j] = xs[j - 1]; xs[j - 1] = t; }
    for (i = 1; i < ny; i++) for (j = i; j > 0 && ys[j] < ys[j - 1]; j--) { int t = ys[j]; ys[j] = ys[j - 1]; ys[j - 1] = t; }
    for (j = 0; j + 1 < ny; j++) {
        if (ys[j] == ys[j + 1])
            continue;
        for (i = 0; i + 1 < nx; i++) {
            int x = xs[i], y = ys[j];
            u8 mask;
            if (xs[i] == xs[i + 1])
                continue;
            if (on[0] && win_inside(x, wx1[0], wx2[0]) && win_inside(y, wy1[0], wy2[0]))
                mask = winin & 0x3f;
            else if (on[1] && win_inside(x, wx1[1], wx2[1]) && win_inside(y, wy1[1], wy2[1]))
                mask = (winin >> 8) & 0x3f;
            else
                mask = winout & 0x3f;
            if (n && out[n - 1].mask == mask && out[n - 1].c.y0 == y && out[n - 1].c.x1 == x) {
                out[n - 1].c.x1 = xs[i + 1];        /* extends the cell on its left */
                continue;
            }
            if (n == MAX_WIN_RECTS)
                return n;
            out[n].c.x0 = x; out[n].c.x1 = xs[i + 1];
            out[n].c.y0 = y; out[n].c.y1 = ys[j + 1];
            out[n].mask = mask;
            n++;
        }
    }
    return n;
}

/* The 3D layer: the geometry front end's triangles (nitro_ge.c), then back to the state the 2D
 * layers draw with (the 3D renderer leaves full-screen state behind). */
static void draw_3d_layer(KhGsPacket *p, const Rect *r)
{
    extern void kh_ge_render(int x, int y, int w, int h);
    u64 sc;
    kh_ge_render((int)r->x, (int)r->y, r->w, r->h);
    if (!scissor_for(r, 0, 0, 256, 192, &sc))
        sc = GS_SET_SCISSOR((int)r->x, (int)(r->x + r->w - 1), (int)r->y, (int)(r->y + r->h - 1));
    kh_gs_packet_ad_begin(p, 4);
    ad(p, GS_REG_SCISSOR_1, sc);
    ad(p, GS_REG_TEST_1, GS_SET_TEST(1, 6, 0, 0, 0, 0, 1, 1));     /* alpha > 0, no Z */
    ad(p, GS_REG_CLAMP_1, GS_SET_CLAMP(0, 0, 0, 0, 0, 0));
    ad(p, GS_REG_TEXA, GS_SET_TEXA(0, 1, 0x80));
}

/* Restrict drawing to window rectangle w (and the screen) */
static void set_clip(KhGsPacket *p, const Rect *r, const ClipRect *c)
{
    u64 sc;
    g_clip.x0 = 0; g_clip.y0 = 0; g_clip.x1 = 256; g_clip.y1 = 192;
    if (!scissor_for(r, c->x0, c->y0, c->x1, c->y1, &sc))
        return;
    g_clip = *c;
    kh_gs_packet_ad_begin(p, 1);
    ad(p, GS_REG_SCISSOR_1, sc);
}

/* ------------------------------------------------------- display capture */

/* DISPCAPCNT (engine A, 0x04000064): bit 31 enable, 29-30 source (0 A, 1 B, 2-3 A and B blended),
 * 24 source A (0 the graphics screen = 2D + 3D, 1 the 3D layer only), 16-17 the VRAM bank written,
 * EVA 0-4, EVB 8-12.  The game's cutscene blur / cross-fade (Gfx_SetupBlendCapture) captures the
 * screen blended with the previous capture into bank C and *displays bank C* (VRAM display mode):
 * the picture is a feedback of the frames before.  On the GS: the capture source is drawn as the
 * graphics screen would be, the previous capture is blended over it with FIX alpha EVB, and the
 * result is copied (scaled to 256x192 PSMCT16 - DS captures are 15-bit) into the capture image
 * for the next frame.  Returns 1 when engine A shows the bank it captures to. */
static int g_capture_valid;

static int capture_shown(int eng, u32 dispcnt, u32 *cap_out)
{
    u32 cap = IO32(0x64);
    if (eng || ((dispcnt >> 16) & 3) != 2 || !(cap >> 31) || ((cap >> 16) & 3) != ((dispcnt >> 18) & 3))
        return 0;
    *cap_out = cap;
    return 1;
}

/* the display mode the compositor draws: VRAM display of the captured bank draws the source */
static int effective_mode(int eng, u32 dispcnt)
{
    u32 cap;
    return capture_shown(eng, dispcnt, &cap) ? 1 : (int)((dispcnt >> 16) & 3);
}

static void capture_blend_and_store(KhGsPacket *p, const Rect *r, u32 cap)
{
    extern uint64_t kh_gs_frame_tex0(void);
    extern uint64_t kh_gs_frame_value(uint32_t fbmsk);
    extern u64 kh_gs_zbuf_value(int mask_writes);
    int src = (int)((cap >> 29) & 3), evb = (int)((cap >> 8) & 31);
    u32 cbp = capture_addr();
    int x0 = (int)r->x, y0 = (int)r->y, x1 = (int)(r->x + r->w), y1 = (int)(r->y + r->h);

    if (evb > 16)
        evb = 16;
    if (src >= 2 && g_capture_valid && evb) {
        /* framebuffer = A * (16 - EVB) / 16 + previous capture * EVB / 16 */
        kh_gs_packet_ad_begin(p, 9);
        ad(p, GS_REG_TEXFLUSH, 0);
        ad(p, GS_REG_TEX0_1, GS_SET_TEX0(cbp / 64, 4, GS_PSM_16, 8, 8, 1, 0, 0, 0, 0, 0, 0));
        ad(p, GS_REG_TEX1_1, GS_SET_TEX1(0, 0, 1, 1, 0, 0, 0));                 /* bilinear */
        ad(p, GS_REG_TEXA, GS_SET_TEXA(0x80, 0, 0x80));
        ad(p, GS_REG_TEST_1, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 1));
        ad(p, GS_REG_ALPHA_1, GS_SET_ALPHA(0, 1, 2, 1, evb * 8));              /* (Cs - Cd) * FIX + Cd */
        ad(p, GS_REG_PRIM, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 1, 0, 1, 0, 1, 0, 0));
        ad(p, GS_REG_RGBAQ, GS_SET_RGBAQ(0x80, 0x80, 0x80, 0x80, 0x3f800000));
        ad(p, GS_REG_UV, GS_SET_UV(0, 0));
        kh_gs_packet_ad_begin(p, 3);
        ad(p, GS_REG_XYZ2, GS_SET_XYZ((KH_GS_OFS + x0) << 4, (KH_GS_OFS + y0) << 4, 0));
        ad(p, GS_REG_UV, GS_SET_UV(256 << 4, 192 << 4));
        ad(p, GS_REG_XYZ2, GS_SET_XYZ((KH_GS_OFS + x1) << 4, (KH_GS_OFS + y1) << 4, 0));
    }
    /* store: the screen rectangle -> the capture image, through a sprite reading the frame */
    kh_gs_packet_ad_begin(p, 12);
    ad(p, GS_REG_TEXFLUSH, 0);
    ad(p, GS_REG_FRAME_1, GS_SET_FRAME(cbp / 2048, 4, GS_PSM_16, 0));
    ad(p, GS_REG_ZBUF_1, kh_gs_zbuf_value(1));
    ad(p, GS_REG_SCISSOR_1, GS_SET_SCISSOR(0, 255, 0, 191));
    ad(p, GS_REG_TEST_1, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 1));
    ad(p, GS_REG_TEX0_1, kh_gs_frame_tex0());
    ad(p, GS_REG_TEX1_1, GS_SET_TEX1(0, 0, 1, 1, 0, 0, 0));
    ad(p, GS_REG_PRIM, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 1, 0, 0, 0, 1, 0, 0));
    ad(p, GS_REG_RGBAQ, GS_SET_RGBAQ(0x80, 0x80, 0x80, 0x80, 0x3f800000));
    ad(p, GS_REG_UV, GS_SET_UV(x0 << 4, y0 << 4));
    ad(p, GS_REG_XYZ2, GS_SET_XYZ(KH_GS_OFS << 4, KH_GS_OFS << 4, 0));
    ad(p, GS_REG_UV, GS_SET_UV(x1 << 4, y1 << 4));
    kh_gs_packet_ad_begin(p, 6);
    ad(p, GS_REG_XYZ2, GS_SET_XYZ((KH_GS_OFS + 256) << 4, (KH_GS_OFS + 192) << 4, 0));
    ad(p, GS_REG_TEXFLUSH, 0);
    ad(p, GS_REG_FRAME_1, kh_gs_frame_value(0));
    ad(p, GS_REG_ZBUF_1, kh_gs_zbuf_value(0));
    ad(p, GS_REG_SCISSOR_1, GS_SET_SCISSOR(x0, x1 - 1, y0, y1 - 1));
    ad(p, GS_REG_TEXA, GS_SET_TEXA(0, 1, 0x80));
    g_capture_valid = 1;
}

static void draw_engine(KhGsPacket *p, const Rect *r, int eng)
{
    WinRect win[MAX_WIN_RECTS];
    int nwin, w;
    u32 dispcnt = IO32(eng ? 0x1000 : 0);
    int mode = (dispcnt >> 16) & 3, bgmode = dispcnt & 7;
    BgInfo bg[4];
    int i, prio;

    kh_gs_packet_ad_begin(p, 2);
    ad(p, GS_REG_SCISSOR_1, GS_SET_SCISSOR((int)r->x, (int)(r->x + r->w - 1), (int)r->y, (int)(r->y + r->h - 1)));
    ad(p, GS_REG_TEST_1, GS_SET_TEST(1, 6, 0, 0, 0, 0, 1, 1));     /* alpha > 0, no Z */

    mode = effective_mode(eng, dispcnt);
    if (mode != 1 || (dispcnt & 0x80))     /* off, VRAM/main-memory display, or forced blank */
        return;

    build_cluts(p, eng, 0, 0);
    {
        u16 bld = IO16((eng ? 0x1000 : 0) + 0x50);
        int evy = IO16((eng ? 0x1000 : 0) + 0x54) & 31;
        if (((bld >> 6) & 3) == 2 && evy && (bld & 0x1f))
            build_cluts(p, eng, 1, evy > 16 ? 16 : evy);
    }
    memset(bg, 0, sizeof bg);
    for (i = 0; i < 4; i++) {
        u16 cnt = IO16((eng ? 0x1008 : 0x8) + i * 2);
        int text = (i < 2) || (i == 2 && (bgmode == 0 || bgmode == 1 || bgmode == 3)) || (i == 3 && bgmode == 0);
        if (!(dispcnt & (0x100u << i)))
            continue;
        if (i == 0 && !eng && (dispcnt & 8)) {
            /* BG0 shows the 3D scene, at BG0's priority like any other layer (the game-over
             * screen puts its models in front of its 2D background with priority 0) */
            bg[0].enabled = 5;
            bg[0].prio = cnt & 3;
            continue;
        }
        if (!text) {
            /* extended BGs: BG3 in modes 3-5, BG2 in mode 5; bit 7 selects a bitmap form */
            int extended = (i == 3 && bgmode >= 3 && bgmode <= 5) || (i == 2 && bgmode == 5);
            if (bgmode == 6) {
                KH_UNIMPLEMENTED_ONCE("ds2d: large bitmap BG (mode 6)");
                continue;
            }
            bg[i].cnt = cnt;
            bg[i].prio = cnt & 3;
            if (extended && (cnt & 0x80)) {
                bg[i].enabled = 2;          /* bitmap: uploaded and drawn in priority order */
                continue;
            }
            /* affine (8-bit map) or extended tiled (16-bit map): 256-colour tiles */
            bg[i].enabled = extended ? 4 : 3;
            bg[i].bpp8 = 1;
            bg[i].char_ofs = ((cnt >> 2) & 0xf) * 0x4000 + (eng ? 0 : ((dispcnt >> 24) & 7) * 0x10000);
            bg[i].scr_ofs = ((cnt >> 8) & 0x1f) * 0x800 + (eng ? 0 : ((dispcnt >> 27) & 7) * 0x10000);
            upload_chars(p, eng ? VIEW_SUB_BG : VIEW_BG, bg[i].char_ofs, extended ? 1024 : 256, 1, tex_addr(eng, i));
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
        int any8 = 0;
        u32 ext = obj_char_extent(eng, &any8);
        int n4 = (int)(ext / 32), n8 = (int)((ext + 63) / 64);
        /* 4bpp sprites index 32-byte tiles of the PSMT4 atlas; 8bpp sprites 64-byte tiles of the
         * PSMT8 one (same bytes, uploaded a second time only if an 8bpp sprite is visible) */
        if (n4)
            upload_chars_ex(p, eng ? VIEW_SUB_OBJ : VIEW_OBJ, 0, n4, 0, tex_addr(eng, 4), 7, 4, 4096);
        if (any8 && n8)
            upload_chars_ex(p, eng ? VIEW_SUB_OBJ : VIEW_OBJ, 0, n8, 1, tex_addr(eng, 5), 7, 2, 2048);
    }

    {
        u32 cap;
        if (capture_shown(eng, dispcnt, &cap) && ((cap >> 24) & 1)) {   /* source A = the 3D layer */
            bg[1].enabled = bg[2].enabled = bg[3].enabled = 0;
            dispcnt &= ~0x1000u;
        }
    }
    nwin = window_rects(eng, win);
    for (prio = 3; prio >= 0; prio--) {
        for (i = 3; i >= 0; i--) {
            if (!bg[i].enabled || bg[i].prio != prio)
                continue;
            for (w = 0; w < nwin; w++) {
                if (!(win[w].mask & (1u << i)))
                    continue;
                if (nwin > 1)
                    set_clip(p, r, &win[w].c);
                layer_effect(p, eng, i, win[w].mask & 0x20);
                if (bg[i].enabled == 5) {
                    draw_3d_layer(p, r);
                    break;                  /* (windows are not applied to the 3D layer) */
                }
                if (bg[i].enabled == 2)
                    draw_bitmap_bg(p, r, eng, i, bg[i].cnt);
                else if (bg[i].enabled >= 3)
                    draw_affine_bg(p, r, eng, i, &bg[i], bg[i].enabled == 4);
                else
                    draw_text_bg(p, r, eng, i, &bg[i]);
            }
        }
        if (dispcnt & 0x1000) {
            for (w = 0; w < nwin; w++) {
                if (!(win[w].mask & 0x10))
                    continue;
                if (nwin > 1)
                    set_clip(p, r, &win[w].c);
                layer_effect(p, eng, 4, win[w].mask & 0x20);
                g_colshift = 7;                 /* OBJ atlas geometry */
                draw_objs(p, r, eng, prio, 0);
                g_colshift = 6;
            }
        }
    }
    if (nwin > 1) {
        static const ClipRect k_full = { 0, 0, 256, 192 };
        set_clip(p, r, &k_full);
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

/* The backdrop: where no layer is opaque the DS shows BG palette entry 0 (when the engine displays
 * graphics, mode 1, without forced blank).  Drawn first, under the 3D layer and the 2D layers. */
void kh_ds2d_backdrop(int eng, int x, int y, int w, int h)
{
    KhGsPacket *p = kh_gs_frame_packet();
    u32 dispcnt = IO32(eng ? 0x1000 : 0);
    u16 c;
    int r, g, b;
    Rect rc;

    if (effective_mode(eng, dispcnt) != 1 || (dispcnt & 0x80))
        return;                                   /* off / VRAM display / forced blank: black */
    c = *(const u16 *)(kh_ds_pal + eng * 0x400);
    r = (c & 31) << 3; g = ((c >> 5) & 31) << 3; b = ((c >> 10) & 31) << 3;
    rc.x = (float)x; rc.y = (float)y; rc.w = w; rc.h = h;
    rc.sx = (float)w / 256.0f; rc.sy = (float)h / 192.0f;
    kh_gs_packet_ad_begin(p, 6);
    ad(p, GS_REG_SCISSOR_1, GS_SET_SCISSOR(x, x + w - 1, y, y + h - 1));
    ad(p, GS_REG_TEST_1, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 1));
    ad(p, GS_REG_PRIM, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 0, 0));
    ad(p, GS_REG_RGBAQ, GS_SET_RGBAQ(r, g, b, 0x80, 0x3f800000));
    ad(p, GS_REG_XYZ2, GS_SET_XYZ(gx(&rc, 0), gy(&rc, 0), 0));
    ad(p, GS_REG_XYZ2, GS_SET_XYZ(gx(&rc, 256), gy(&rc, 192), 0));
    kh_gs_packet_ad_begin(p, 1);
    ad(p, GS_REG_SCISSOR_1, GS_SET_SCISSOR(0, kh_video_width() - 1, 0, kh_video_height() - 1));
}

void kh_ds2d_init(void)
{
    u32 end_words;
    if (g_ready)
        return;
    g_base = (kh_gs_texpool_base() / 4 + 2047) & ~2047u;   /* page aligned */
    end_words = validate_vram_layout();
    g_ready = 1;
    {
        extern void kh_tex3d_init(uint32_t base_bytes, uint32_t size_bytes);
        uint32_t tex_base = end_words * 4u;
        kh_tex3d_init(tex_base, (GS_VRAM_WORDS - end_words) * 4u);
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
#if KH_PS2_DEBUG
    if (eng)
        trace_engine_b_state();
#endif
    kh_prof_begin(KH_PROF_R2D);
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
    kh_gs_packet_ad_begin(p, 2);                      /* layer effects leave their own state */
    ad(p, GS_REG_RGBAQ, GS_SET_RGBAQ(0x80, 0x80, 0x80, 0x80, 0x3f800000));
    ad(p, GS_REG_PRIM, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 1, 0, 0, 0, 1, 0, 0));
    {
        u32 cap;
        if (capture_shown(eng, IO32(eng ? 0x1000 : 0), &cap))
            capture_blend_and_store(p, &r, cap);
        else if (!eng)
            g_capture_valid = 0;                 /* the next capture starts without history */
    }
    draw_brightness(p, &r, eng);

    /* back to full-screen state for whoever draws next */
    kh_gs_packet_ad_begin(p, 2);
    ad(p, GS_REG_SCISSOR_1, GS_SET_SCISSOR(0, kh_video_width() - 1, 0, kh_video_height() - 1));
    ad(p, GS_REG_TEST_1, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 2));
    kh_prof_end(KH_PROF_R2D);
}
