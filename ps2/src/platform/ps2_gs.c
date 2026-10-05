/* GS back end: video mode, VRAM layout, frame packets, DMA submission, debug text.
 *
 * Video: interlaced, full-height 640x448 (NTSC) / 640x512 (PAL) framebuffers, FIELD mode (each
 * field reads every other line).  The layouts put a DS screen into as few as 224 lines (two
 * screens stacked): a half-height buffer would give it 112 and drop most of its 192 rows.
 * 16-bit colour and Z keep the VRAM cost of the old 640x224 32-bit buffers (the DS's 2D output is
 * 15-bit; the 3D is dithered).
 *
 * GS VRAM (4 MiB) at NTSC:
 *   2 x framebuffer PSMCT16 640x448   1,146,880
 *   1 x Z buffer   PSMZ16   640x448     573,440
 *   debug font PSMT8 128x128 + CLUT      ~17,000
 *   texture pools                      ~2.3 MiB (ds2d.c, nitro_tex.c)
 *
 * Packets are built in cached EE RAM, written back with FlushCache and sent on the GIF DMA
 * channel (PATH3) in chunks under the 0xFFFF-qword DMA limit.  VU1/PATH1 is added later
 * without changing this interface.
 */
#include "platform/kh_platform.h"
#include "platform/ps2/ps2_gs.h"
#include "ps2_internal.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <kernel.h>
#include <dma.h>
#include <dma_tags.h>
#include <graph.h>
#include <graph_vram.h>
#include <gs_gp.h>
#include <gs_psm.h>
#include <gif_tags.h>

#define GS_VRAM_BYTES (4u * 1024u * 1024u)

extern const unsigned char msx[];  /* 8x8 font from the SDK's libdebug */

#define FB_PSM GS_PSM_16
#define Z_PSM  GS_PSMZ_16

static int g_w = 640, g_h = 448;
static int g_pal;
static int g_fbp[2], g_zbp, g_draw;
static int g_fontbp, g_fontclut;
static int g_flip_pending;
static int g_video_ready;
static uint32_t g_vram_top;         /* first byte above fixed allocations */
static KhGsPacket g_frame_pkt[2];

/* -------------------------------------------------------------- packets */

void kh_gs_packet_init(KhGsPacket *p, uint32_t qwords)
{
    p->base = kh_alloc(qwords * 16, 64, KH_LIFE_GLOBAL, KH_MEM_RENDER);
    if (!p->base)
        kh_panic("GS packet buffer (%u KiB) allocation failed", qwords * 16 / 1024);
    p->cap = qwords;
    p->len = 0;
    p->tag = -1;
}

/* ---- chain mode ---- */
#define KH_DMATAG(qwc, id, addr) ((uint64_t)((qwc) & 0xffff) | ((uint64_t)(id) << 28) | ((uint64_t)(uint32_t)(addr) << 32))
enum { TAG_REFE = 0, TAG_CNT = 1, TAG_REF = 3, TAG_END = 7 };

void kh_gs_chain_begin(KhGsPacket *p)
{
    p->len = 0;
    p->tag = 0;
    p->tag_vif[0] = p->tag_vif[1] = 0;
    p->vu_busy = 0;
    kh_gs_packet_q(p, 0, 0);          /* CNT tag, filled in when closed */
}

/* VIFcodes for a tag whose qwc qwords are GIF data: DIRECT, after a FLUSH if VU1 work is queued */
static uint64_t direct_codes(KhGsPacket *p, uint32_t qwc)
{
    uint32_t v0 = KH_VIF_NOP;
    if (!qwc)
        return 0;                     /* (DIRECT 0 would mean 65536 qwords) */
    if (p->vu_busy) {
        v0 = KH_VIF_FLUSH;
        p->vu_busy = 0;
    }
    return (uint64_t)v0 | ((uint64_t)KH_VIF_DIRECT(qwc) << 32);
}

static void close_tag(KhGsPacket *p, int id)
{
    uint64_t *t = (uint64_t *)p->base + p->tag * 2;
    uint32_t qwc = p->len - p->tag - 1;
    t[0] = KH_DMATAG(qwc, id, 0);
    if (p->tag_vif[1])
        t[1] = (uint64_t)p->tag_vif[0] | ((uint64_t)p->tag_vif[1] << 32);
    else
        t[1] = direct_codes(p, qwc);
    p->tag_vif[0] = p->tag_vif[1] = 0;
}

static void open_tag(KhGsPacket *p)
{
    p->tag = (int32_t)p->len;
    kh_gs_packet_q(p, 0, 0);
}

void kh_gs_chain_ref(KhGsPacket *p, const void *data, uint32_t qwc)
{
    if (p->tag < 0)
        kh_panic("kh_gs_chain_ref on a normal-mode packet");
    if (qwc && (!data || ((uintptr_t)data & 15u)))
        kh_panic("VIF1 REF source %p is not 16-byte aligned (%u qwords)", data, (unsigned)qwc);
    close_tag(p, TAG_CNT);
    while (qwc) {
        uint32_t n = qwc > 0xffff ? 0xffff : qwc;
        kh_gs_packet_q(p, KH_DMATAG(n, TAG_REF, (uintptr_t)data), direct_codes(p, n));
        data = (const uint8_t *)data + n * 16;
        qwc -= n;
    }
    open_tag(p);
}

void kh_gs_chain_vif(KhGsPacket *p, uint32_t vif0, uint32_t vif1)
{
    close_tag(p, TAG_CNT);
    open_tag(p);
    p->tag_vif[0] = vif0;
    p->tag_vif[1] = vif1;
}

void kh_gs_chain_vif_ref(KhGsPacket *p, uint32_t vif0, uint32_t vif1, const void *ref, uint32_t qwc)
{
    if (qwc > 0xffffu)
        kh_panic("VIF1 REF is too large (%u qwords)", (unsigned)qwc);
    if (qwc && (!ref || ((uintptr_t)ref & 15u)))
        kh_panic("VIF1 REF source %p is not 16-byte aligned (%u qwords)", ref, (unsigned)qwc);
    close_tag(p, TAG_CNT);
    kh_gs_packet_q(p, KH_DMATAG(qwc, TAG_REF, (uintptr_t)ref), (uint64_t)vif0 | ((uint64_t)vif1 << 32));
    open_tag(p);
}

void kh_gs_chain_send(KhGsPacket *p)
{
    close_tag(p, TAG_END);
    FlushCache(0);
    dma_channel_wait(DMA_CHANNEL_VIF1, 0);
    dma_channel_send_chain(DMA_CHANNEL_VIF1, p->base, (int)p->len, DMA_FLAG_TRANSFERTAG, 0);
    dma_channel_wait(DMA_CHANNEL_VIF1, 0);
    p->len = 0;
}

/* ------------------------------------------------------------------ VU1 */

/* weak: the platform test ELF links this file without the microprograms */
extern unsigned char kh_vu1_tri_start[] __attribute__((weak)), kh_vu1_tri_end[] __attribute__((weak));

/* Upload the microprograms to VU1 micro memory (MPG through a VIF1 chain) and set up the VIF
 * double buffering the 3D path uses (ps2/src/nitro/nitro_ge.c). */
void kh_vu1_init(void)
{
    static uint64_t chain[2 * 64] __attribute__((aligned(64)));
    uint32_t n = (uint32_t)(kh_vu1_tri_end - kh_vu1_tri_start) / 8, q = 0;
    if (!kh_vu1_tri_start)
        return;
    if (n > 256 || (n & 1))
        kh_panic("VU1 microprogram size %u", (unsigned)n);
    /* CNT: FLUSHE, MPG n at 0 -- the program follows as the tag's data */
    chain[q * 2] = KH_DMATAG(n / 2, TAG_CNT, 0);
    chain[q * 2 + 1] = (uint64_t)KH_VIF_FLUSHE | ((uint64_t)KH_VIF_MPG(n, 0) << 32);
    q++;
    memcpy(&chain[q * 2], kh_vu1_tri_start, n * 8);
    q += n / 2;
    /* END: STCYCL 1,1; BASE 0, OFFSET 512 come in a second tag */
    chain[q * 2] = KH_DMATAG(0, TAG_CNT, 0);
    chain[q * 2 + 1] = (uint64_t)KH_VIF_STCYCL(1, 1) | ((uint64_t)KH_VIF_BASE(0) << 32);
    q++;
    chain[q * 2] = KH_DMATAG(0, TAG_END, 0);
    chain[q * 2 + 1] = (uint64_t)KH_VIF_OFFSET(512) | ((uint64_t)KH_VIF_NOP << 32);
    q++;
    if (q * 2 > sizeof chain / sizeof chain[0])
        kh_panic("VU1 init chain overflow");
    FlushCache(0);
    dma_channel_send_chain(DMA_CHANNEL_VIF1, chain, (int)q, DMA_FLAG_TRANSFERTAG, 0);
    dma_channel_wait(DMA_CHANNEL_VIF1, 0);
    KH_INFO("gs", "VU1 microprogram: %u instructions", (unsigned)n);
}

void kh_gs_upload_ref(KhGsPacket *p, const void *src, int bp, int bw, int psm, int x, int y, int w, int h)
{
    int bpp = (psm == GS_PSM_8) ? 8 : (psm == GS_PSM_4) ? 4 : (psm == GS_PSM_16 || psm == GS_PSM_16S) ? 16 : 32;
    uint32_t bits, bytes, qw;

    if (w <= 0 || h <= 0 || bw <= 0 || bp < 0)
        kh_panic("bad GS upload geometry: bp=%d bw=%d %dx%d", bp, bw, w, h);
    if ((uint32_t)bp * 256u >= GS_VRAM_BYTES)
        kh_panic("GS upload base 0x%x is outside 4 MiB VRAM", (unsigned)bp);
    bits = (uint32_t)w * (uint32_t)h * (uint32_t)bpp;
    bytes = (bits + 7u) / 8u;
    qw = (bytes + 15u) / 16u;
    if (bytes && !src)
        kh_panic("NULL GS upload source (%u bytes)", (unsigned)bytes);

    kh_gs_packet_ad_begin(p, 4);
    kh_gs_packet_q(p, GS_SET_BITBLTBUF(0, 0, 0, bp, bw, psm), GS_REG_BITBLTBUF);
    kh_gs_packet_q(p, GS_SET_TRXPOS(0, 0, x, y, 0), GS_REG_TRXPOS);
    kh_gs_packet_q(p, GS_SET_TRXREG(w, h), GS_REG_TRXREG);
    kh_gs_packet_q(p, GS_SET_TRXDIR(0), GS_REG_TRXDIR);
    kh_gs_packet_q(p, GIF_SET_TAG(qw, 1, 0, 0, GIF_FLG_IMAGE, 0), 0);

    /* Real VIF1/GIF REF DMA requires a qword-aligned source and consumes complete qwords.
     * PCSX2 can be more forgiving.  Keep the zero-copy fast path when both are naturally safe;
     * otherwise copy exactly the image bytes into the already-aligned frame packet and zero-pad
     * the last qword.  This fixes the class for every 2D/3D texture caller instead of teaching
     * individual scenes about DMAC alignment. */
    if ((((uintptr_t)src & 15u) == 0) && ((bytes & 15u) == 0)) {
        kh_gs_chain_ref(p, src, qw);
    } else {
        uint32_t padded = qw * 16u;
        uint8_t *dst;
        if (p->len + qw > p->cap)
            kh_panic("GS frame packet overflow while staging %u-byte IMAGE", (unsigned)bytes);
        dst = (uint8_t *)p->base + p->len * 16u;
        memcpy(dst, src, bytes);
        if (padded > bytes)
            memset(dst + bytes, 0, padded - bytes);
        p->len += qw;
    }
    /* texels written by a transfer are only guaranteed to be sampled after TEXFLUSH; without it
     * the GS (and PCSX2's texture cache) can keep drawing what was there before (old HUD digits
     * over the pause menu's buttons) */
    kh_gs_packet_ad_begin(p, 1);
    kh_gs_packet_q(p, 0, GS_REG_TEXFLUSH);
    kh_prof_tex_upload(qw * 16);
}

void kh_gs_packet_send(KhGsPacket *p)
{
    uint32_t off = 0;
    if (!p->len)
        return;
    FlushCache(0);
    while (off < p->len) {
        uint32_t n = p->len - off;
        if (n > 0xF000)
            n = 0xF000;
        dma_channel_wait(DMA_CHANNEL_GIF, 0);
        dma_channel_send_normal(DMA_CHANNEL_GIF, (u128 *)p->base + off, (int)n, 0, 0);
        off += n;
    }
    dma_channel_wait(DMA_CHANNEL_GIF, 0);
    p->len = 0;
}

static inline void ad(KhGsPacket *p, uint64_t reg, uint64_t val)
{
    kh_gs_packet_q(p, val, reg);
}

void kh_gs_packet_ad_begin(KhGsPacket *p, uint32_t nregs)
{
    kh_gs_packet_q(p, GIF_SET_TAG(nregs, 1, 0, 0, GIF_FLG_PACKED, 1), GIF_REG_AD);
}

/* --------------------------------------------------------------- VRAM */

uint32_t ps2_gs_vram_total(void) { return GS_VRAM_BYTES; }
uint32_t ps2_gs_vram_used(void) { return g_vram_top + kh_gs_texpool_used(); }
uint32_t kh_gs_texpool_base(void) { return g_vram_top; }

static int vram_alloc(int w, int h, int psm)
{
    int a = graph_vram_allocate(w, h, psm, GRAPH_ALIGN_PAGE);
    if (a < 0)
        kh_panic("GS VRAM exhausted allocating %dx%d psm %d", w, h, psm);
    {
        uint32_t end = (uint32_t)a * 4u + (uint32_t)graph_vram_size(w, h, psm, GRAPH_ALIGN_PAGE) * 4u;
        if (end > g_vram_top)
            g_vram_top = end;
    }
    return a;
}

/* ---------------------------------------------------------------- font */

static void upload_font(void)
{
    static uint8_t pix[128 * 128] __attribute__((aligned(64)));
    static uint32_t clut[256] __attribute__((aligned(64)));
    KhGsPacket p;
    int c, row, col;

    for (c = 0; c < 256; c++) {
        int cx = (c & 15) * 8, cy = (c >> 4) * 8;
        for (row = 0; row < 8; row++) {
            uint8_t bits = msx[c * 8 + row];
            for (col = 0; col < 8; col++)
                pix[(cy + row) * 128 + cx + col] = (bits & (0x80 >> col)) ? 1 : 0;
        }
    }
    memset(clut, 0, sizeof clut);
    clut[1] = 0x80ffffff; /* white, alpha 0x80 (= 1.0 on the GS) */

    g_fontbp = vram_alloc(128, 128, GS_PSM_8);
    g_fontclut = vram_alloc(16, 16, GS_PSM_32);

    kh_gs_packet_init(&p, 1024 + 64 + 64);
    kh_gs_upload(&p, pix, g_fontbp / 64, 2, GS_PSM_8, 128, 128);
    kh_gs_upload(&p, clut, g_fontclut / 64, 1, GS_PSM_32, 16, 16);
    kh_gs_packet_send(&p);
    kh_free(p.base);
}

/* Upload an image with a PATH3 IMAGE transfer.  bp = GS base pointer (in 64-word blocks), bw =
 * buffer width in 64-pixel units.  The pixels are sent by reference-free copy into the packet
 * for simplicity at this stage (the residency cache uses REF DMA tags instead). */
void kh_gs_upload(KhGsPacket *p, const void *src, int bp, int bw, int psm, int w, int h)
{
    int bpp = (psm == GS_PSM_8) ? 8 : (psm == GS_PSM_4) ? 4 : (psm == GS_PSM_16 || psm == GS_PSM_16S) ? 16 : 32;
    uint32_t bytes = (uint32_t)w * (uint32_t)h * (uint32_t)bpp / 8u;
    uint32_t qw = (bytes + 15) / 16;

    kh_gs_packet_ad_begin(p, 4);
    ad(p, GS_REG_BITBLTBUF, GS_SET_BITBLTBUF(0, 0, 0, bp, bw, psm));
    ad(p, GS_REG_TRXPOS, GS_SET_TRXPOS(0, 0, 0, 0, 0));
    ad(p, GS_REG_TRXREG, GS_SET_TRXREG(w, h));
    ad(p, GS_REG_TRXDIR, GS_SET_TRXDIR(0));
    kh_gs_packet_q(p, GIF_SET_TAG(qw, 1, 0, 0, GIF_FLG_IMAGE, 0), 0);
    if (p->len + qw > p->cap)
        kh_panic("GS upload of %u qwords overflows packet", qw);
    memcpy((u128 *)p->base + p->len, src, bytes);
    p->len += qw;
    kh_gs_packet_ad_begin(p, 1);
    kh_gs_packet_q(p, 0, GS_REG_TEXFLUSH);
    kh_prof_tex_upload(bytes);
}

/* ---------------------------------------------------------------- mode */

int kh_video_init(KhVideoMode mode)
{
    int region = graph_get_region();
    int pal;

    if (mode == KH_VIDEO_AUTO)
        pal = (region == GRAPH_MODE_PAL);
    else
        pal = (mode == KH_VIDEO_PAL);
    g_w = 640;
    g_h = pal ? 512 : 448;
    g_pal = pal;

    dma_channel_initialize(DMA_CHANNEL_GIF, NULL, 0);
    dma_channel_fast_waits(DMA_CHANNEL_GIF);
    dma_channel_initialize(DMA_CHANNEL_VIF1, NULL, 0);
    dma_channel_fast_waits(DMA_CHANNEL_VIF1);

    graph_vram_clear();
    g_vram_top = 0;
    g_fbp[0] = vram_alloc(g_w, g_h, FB_PSM);
    g_fbp[1] = vram_alloc(g_w, g_h, FB_PSM);
    g_zbp = vram_alloc(g_w, g_h, Z_PSM);

    /* full-height buffer, interlaced FIELD mode: each field reads every other line of it */
    graph_set_mode(GRAPH_MODE_INTERLACED, pal ? GRAPH_MODE_PAL : GRAPH_MODE_NTSC, GRAPH_MODE_FIELD, GRAPH_ENABLE);
    graph_set_screen(0, 0, g_w, g_h);
    graph_set_bgcolor(0, 0, 0);
    graph_set_framebuffer_filtered(g_fbp[1], g_w, FB_PSM, 0, 0);
    graph_enable_output();
    upload_font();          /* after the mode set: it resets the GS */
    kh_vu1_init();

    ps2_time_set_refresh(pal ? 50 : 60);
    kh_gs_packet_init(&g_frame_pkt[0], KH_GS_FRAME_QWORDS);
    kh_gs_packet_init(&g_frame_pkt[1], KH_GS_FRAME_QWORDS);
    g_draw = 0;
    g_video_ready = 1;
    KH_INFO("gs", "%s %dx%d 16-bit field mode, VRAM fixed %u KiB, texture pool %u KiB", pal ? "PAL" : "NTSC",
            g_w, g_h, g_vram_top / 1024, (GS_VRAM_BYTES - g_vram_top) / 1024);
    return 0;
}

int kh_video_width(void) { return g_w; }

/* ZBUF_1 for the frame's Z buffer, with Z writes masked or not */
uint64_t kh_gs_zbuf_value(int mask_writes) { return GS_SET_ZBUF(g_zbp / 2048, Z_PSM, mask_writes ? 1 : 0); }
/* the largest Z value the Z buffer holds (the 3D projection scales into 0..this) */
float kh_gs_z_max(void) { return 65535.0f; }
int kh_video_height(void) { return g_h; }

/* Set while the crash report draws.  Once kh_crash_active is set (ps2_crash.c), ordinary frames
 * are neither submitted nor flipped: the crash handler parks only the faulting thread, so after a
 * crash on a worker (music stream, audio, loader) the main loop would otherwise keep presenting
 * and replace the report within a frame. */
static int g_drawing_crash;
extern volatile int kh_crash_active;

int ps2_gs_crash_screen(const char *title, const char *const *lines, int count)
{
    int i;
    if (!g_video_ready || !g_frame_pkt[g_draw].base)
        return 0;

    /* Reassert the exact display mode used by the game.  libdebug's init_scr() assumes its own
     * PSMCT32 framebuffer at VRAM 0; the port normally displays PSMCT16 full-height FIELD buffers,
     * so using libdebug after a late game exception can produce a blue/patterned screen instead
     * of readable registers on real GS hardware. */
    graph_set_mode(GRAPH_MODE_INTERLACED,
                   g_pal ? GRAPH_MODE_PAL : GRAPH_MODE_NTSC,
                   GRAPH_MODE_FIELD, GRAPH_ENABLE);
    graph_set_screen(0, 0, g_w, g_h);
    graph_set_bgcolor(0, 0, 0);
    graph_set_framebuffer_filtered(g_fbp[g_draw], g_w, FB_PSM, 0, 0);
    graph_enable_output();

    kh_video_begin_frame(0x000018);
    kh_video_debug_text(16, 16, 0xffffff, "%s", title ? title : "PS2 crash");
    for (i = 0; i < count && i < 12; i++)
        kh_video_debug_text(16, 40 + i * 16, 0xffffff, "%s", lines[i]);
    kh_video_debug_text(16, 40 + count * 16 + 16, 0xffffff,
                        "Map EPC/RA with build/khdays-ps2.map");
    g_drawing_crash = 1;
    kh_video_submit_frame();
    g_drawing_crash = 0;
    g_flip_pending = 0;     /* the report is displayed directly below; never flip away from it */

    /* Show exactly the buffer just drawn; do not depend on the normal flip state machine. */
    graph_set_framebuffer_filtered(g_fbp[g_draw], g_w, FB_PSM, 0, 0);
    graph_enable_output();
    return 1;
}

/* TEX0 that reads the frame being drawn with the framebuffer's PSM (1024x512 addressing) */
uint64_t kh_gs_frame_tex0(void) { return GS_SET_TEX0(g_fbp[g_draw] / 64, g_w / 64, FB_PSM, 10, 9, 1, 0, 0, 0, 0, 0, 0); }

/* FRAME_1 of the frame being drawn, with write mask fbmsk (bits set = not written) */
uint64_t kh_gs_frame_value(uint32_t fbmsk) { return GS_SET_FRAME(g_fbp[g_draw] / 2048, g_w / 64, FB_PSM, fbmsk); }

KhGsPacket *kh_gs_frame_packet(void) { return &g_frame_pkt[g_draw]; }

void kh_video_begin_frame(uint32_t rgb)
{
    KhGsPacket *p = &g_frame_pkt[g_draw];
    kh_gs_chain_begin(p);
    kh_gs_packet_ad_begin(p, 12);
    ad(p, GS_REG_FRAME_1, GS_SET_FRAME(g_fbp[g_draw] / 2048, g_w / 64, FB_PSM, 0));
    ad(p, GS_REG_ZBUF_1, GS_SET_ZBUF(g_zbp / 2048, Z_PSM, 0));
    ad(p, GS_REG_XYOFFSET_1, GS_SET_XYOFFSET(KH_GS_OFS << 4, KH_GS_OFS << 4));
    ad(p, GS_REG_SCISSOR_1, GS_SET_SCISSOR(0, g_w - 1, 0, g_h - 1));
    ad(p, GS_REG_PRMODECONT, GS_SET_PRMODECONT(1));
    ad(p, GS_REG_COLCLAMP, GS_SET_COLCLAMP(1));
    ad(p, GS_REG_DTHE, GS_SET_DTHE(1));   /* 16-bit output: ordered dither (DIMX below) */
    ad(p, GS_REG_TEST_1, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 1));   /* Z always, for the clear */
    ad(p, GS_REG_PRIM, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 0, 0));
    ad(p, GS_REG_RGBAQ, GS_SET_RGBAQ((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, rgb & 0xff, 0x80, 0x3f800000));
    ad(p, GS_REG_XYZ2, GS_SET_XYZ(KH_GS_OFS << 4, KH_GS_OFS << 4, 0));
    ad(p, GS_REG_XYZ2, GS_SET_XYZ((KH_GS_OFS + g_w) << 4, (KH_GS_OFS + g_h) << 4, 0));
    /* 4x4 Bayer offsets 0..3 only: DS 2D colours (5-bit values << 3, drawn unmodulated) stay
     * exact, gradients of the 3D layer (shading, fog, blending) are dithered */
    kh_gs_packet_ad_begin(p, 1);
    ad(p, GS_REG_DIMX, GS_SET_DIMX(0, 2, 0, 2, 3, 1, 3, 1, 0, 2, 0, 2, 3, 1, 3, 1));
    kh_gs_packet_ad_begin(p, 1);
    ad(p, GS_REG_TEST_1, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 2)); /* Z GEQUAL from here on */
}

void kh_video_debug_text(int x, int y, uint32_t rgb, const char *fmt, ...)
{
    KhGsPacket *p = &g_frame_pkt[g_draw];
    char buf[128];
    int i, n;
    va_list ap;

    va_start(ap, fmt);
    n = vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    if (n <= 0)
        return;
    if (n > (int)sizeof buf - 1)
        n = sizeof buf - 1;
    if (p->len + 8 + (uint32_t)n * 4 > p->cap)
        return;
    kh_gs_packet_ad_begin(p, 5);
    ad(p, GS_REG_TEST_1, GS_SET_TEST(1, 6, 0, 0, 0, 0, 1, 1));  /* alpha GREATER 0, Z always */
    ad(p, GS_REG_TEX0_1, GS_SET_TEX0(g_fontbp / 64, 2, GS_PSM_8, 7, 7, 1, 0, g_fontclut / 64, GS_PSM_32, 0, 0, 1));
    ad(p, GS_REG_TEX1_1, GS_SET_TEX1(0, 0, 0, 0, 0, 0, 0));
    ad(p, GS_REG_PRIM, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 1, 0, 0, 0, 1, 0, 0));
    ad(p, GS_REG_RGBAQ, GS_SET_RGBAQ((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, rgb & 0xff, 0x80, 0x3f800000));
    kh_gs_packet_q(p, GIF_SET_TAG(n, 1, 0, 0, GIF_FLG_PACKED, 4),
                   (uint64_t)GIF_REG_UV | ((uint64_t)GIF_REG_XYZ2 << 4) | ((uint64_t)GIF_REG_UV << 8) | ((uint64_t)GIF_REG_XYZ2 << 12));
    for (i = 0; i < n; i++) {
        unsigned char c = (unsigned char)buf[i];
        int u = (c & 15) * 8, v = (c >> 4) * 8;
        int sx = KH_GS_OFS + x + i * 8, sy = KH_GS_OFS + y;
        kh_gs_packet_q(p, KH_PK_UV_LO(u << 4, v << 4), 0);
        kh_gs_packet_q(p, KH_PK_XYZ2_LO(sx << 4, sy << 4), KH_PK_XYZ2_HI(0));
        kh_gs_packet_q(p, KH_PK_UV_LO((u + 8) << 4, (v + 8) << 4), 0);
        kh_gs_packet_q(p, KH_PK_XYZ2_LO((sx + 8) << 4, (sy + 8) << 4), KH_PK_XYZ2_HI(0));
    }
    kh_gs_packet_ad_begin(p, 1);
    ad(p, GS_REG_TEST_1, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 2));
}

void kh_video_submit_frame(void)
{
    KhGsPacket *p = &g_frame_pkt[g_draw];
    if (kh_crash_active && !g_drawing_crash) {
        p->len = 0;          /* keep the crash report on screen: drop this frame */
        return;
    }
    kh_gs_packet_ad_begin(p, 1);
    ad(p, GS_REG_FINISH, 1);

    KH_PROF_ADD(KH_PC_GIF_BYTES, p->len * 16);
    kh_prof_begin(KH_PROF_GS_WAIT);
    *(volatile uint64_t *)0x12001000 = 2;       /* clear CSR.FINISH */
    kh_gs_chain_send(p);
    while (!(*(volatile uint64_t *)0x12001000 & 2))
        ;
    kh_prof_end(KH_PROF_GS_WAIT);
    g_flip_pending = 1;
}

void kh_video_flip(void)
{
    if (!g_flip_pending || kh_crash_active)
        return;
    g_flip_pending = 0;
    graph_set_framebuffer_filtered(g_fbp[g_draw], g_w, FB_PSM, 0, 0);
    g_draw ^= 1;
}

void kh_video_end_frame(void)
{
    kh_video_submit_frame();
    kh_vblank_wait();
    kh_video_flip();
}

/*
 * Draw one line of debug text IMMEDIATELY onto the buffer that is on the TV now (not the frame
 * being built), over a black bar.  For hangs that disable EE interrupts: nothing can report after
 * such a hang (no VBlank, no watchdog thread), so a breadcrumb is only useful if it is already
 * visible when the CPU stops.  Main thread only, between frames: kh_video_submit_frame has waited
 * for the previous frame's DMA and FINISH, so GIF/VIF are idle.  Everything set here (FRAME_1,
 * TEST, ZBUF mask, PRIM, TEX0/1) is set again by the next kh_video_begin_frame / layer setup.
 */
void kh_video_live_text(int x, int y, uint32_t rgb, const char *text)
{
    static KhGsPacket p;
    int n, i, front;

    if (!g_video_ready || !text)
        return;
    if (!p.base)
        kh_gs_packet_init(&p, 1024);
    n = (int)strlen(text);
    if (n > 100)
        n = 100;
    front = g_draw ^ 1;               /* the flipped (displayed) buffer, flip pending or not */

    p.len = 0;
    p.tag = -1;
    kh_gs_packet_ad_begin(&p, 10);
    ad(&p, GS_REG_FRAME_1, GS_SET_FRAME(g_fbp[front] / 2048, g_w / 64, FB_PSM, 0));
    ad(&p, GS_REG_ZBUF_1, GS_SET_ZBUF(g_zbp / 2048, Z_PSM, 1));          /* no Z writes */
    ad(&p, GS_REG_XYOFFSET_1, GS_SET_XYOFFSET(KH_GS_OFS << 4, KH_GS_OFS << 4));
    ad(&p, GS_REG_SCISSOR_1, GS_SET_SCISSOR(0, g_w - 1, 0, g_h - 1));
    ad(&p, GS_REG_TEST_1, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 1));         /* Z always */
    ad(&p, GS_REG_PRIM, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 0, 0));
    ad(&p, GS_REG_RGBAQ, GS_SET_RGBAQ(0, 0, 0, 0x80, 0x3f800000));
    ad(&p, GS_REG_XYZ2, GS_SET_XYZ((KH_GS_OFS + x - 2) << 4, (KH_GS_OFS + y - 2) << 4, 0));
    ad(&p, GS_REG_XYZ2, GS_SET_XYZ((KH_GS_OFS + g_w) << 4, (KH_GS_OFS + y + 10) << 4, 0));
    ad(&p, GS_REG_TEST_1, GS_SET_TEST(1, 6, 0, 0, 0, 0, 1, 1));         /* alpha > 0, Z always */
    kh_gs_packet_ad_begin(&p, 4);
    ad(&p, GS_REG_TEX0_1, GS_SET_TEX0(g_fontbp / 64, 2, GS_PSM_8, 7, 7, 1, 0, g_fontclut / 64, GS_PSM_32, 0, 0, 1));
    ad(&p, GS_REG_TEX1_1, GS_SET_TEX1(0, 0, 0, 0, 0, 0, 0));
    ad(&p, GS_REG_PRIM, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 1, 0, 0, 0, 1, 0, 0));
    ad(&p, GS_REG_RGBAQ, GS_SET_RGBAQ((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, rgb & 0xff, 0x80, 0x3f800000));
    if (n > 0) {
        kh_gs_packet_q(&p, GIF_SET_TAG(n, 1, 0, 0, GIF_FLG_PACKED, 4),
                       (uint64_t)GIF_REG_UV | ((uint64_t)GIF_REG_XYZ2 << 4) | ((uint64_t)GIF_REG_UV << 8) |
                           ((uint64_t)GIF_REG_XYZ2 << 12));
        for (i = 0; i < n; i++) {
            unsigned char c = (unsigned char)text[i];
            int u = (c & 15) * 8, v = (c >> 4) * 8;
            int sx = KH_GS_OFS + x + i * 8, sy = KH_GS_OFS + y;
            kh_gs_packet_q(&p, KH_PK_UV_LO(u << 4, v << 4), 0);
            kh_gs_packet_q(&p, KH_PK_XYZ2_LO(sx << 4, sy << 4), KH_PK_XYZ2_HI(0));
            kh_gs_packet_q(&p, KH_PK_UV_LO((u + 8) << 4, (v + 8) << 4), 0);
            kh_gs_packet_q(&p, KH_PK_XYZ2_LO((sx + 8) << 4, (sy + 8) << 4), KH_PK_XYZ2_HI(0));
        }
    }
    kh_gs_packet_ad_begin(&p, 2);
    ad(&p, GS_REG_FRAME_1, GS_SET_FRAME(g_fbp[g_draw] / 2048, g_w / 64, FB_PSM, 0));
    ad(&p, GS_REG_TEST_1, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 2));
    kh_gs_packet_send(&p);
}
