/* Frame composition: the DS screens' logical state -> GS.
 *
 * The DS has two 256x192 screens; the PS2 one television picture.  Until the per-scene
 * single-screen presentation exists (docs/PS2_PORT.md, "Dual-screen UI conversion"), the
 * layout is selectable with L3 (in this order):
 *   0  vertical: top screen above bottom screen, as on the DS (default)
 *   1  horizontal: both screens side by side
 *   2  top screen only, full picture
 *   3  bottom screen only, full picture
 * The picture is 640x448 shown as 4:3, so a 4:3 DS screen h lines tall is h * 640 / 448 wide.
 * POWCNT bit 15 says which 2D engine drives the top screen, as on the DS.
 */
#include "platform/kh_platform.h"
#include "platform/kh_loadprof.h"
#include "nitro_internal.h"

#include <stdarg.h>
#include <stdio.h>

extern void kh_ds2d_draw(int eng, int x, int y, int w, int h);
extern void kh_ge_render(int x, int y, int w, int h);

/* One DS screen: the backdrop, then the layers in priority order - the 3D layer (engine A's BG0
 * in 3D mode) among them, drawn by the compositor at BG0's priority. */
static void draw_screen(int eng, int x, int y, int w, int h)
{
    extern void kh_ds2d_backdrop(int eng, int x, int y, int w, int h);
    kh_ds2d_backdrop(eng, x, y, w, h);
    kh_ds2d_draw(eng, x, y, w, h);
}

static int g_layout;
#if KH_PS2_DEBUG
static u32 g_layout_shown;    /* VBlank count when the layout last changed (the caption shows) */
#endif

/* 2D register readout (all builds): hold R3 and press L3.  A screenshot of a wrongly composed
 * screen then carries the display state the compositor drew it from - layer enables, BG
 * controls, blending, windows, capture, VRAM banks - which the frame alone cannot show. */
static int g_regs_overlay;

static void regs_line(int x, int y, const char *fmt, ...)
{
    char buf[96];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    kh_video_debug_text(x + 1, y + 1, 0x000000, "%s", buf);   /* shadow: readable over any frame */
    kh_video_debug_text(x, y, 0xffff80, "%s", buf);
}

static void draw_regs_overlay(int top_eng, int H)
{
    extern uint32_t kh_nitro_view_banks(int view);
#define R16(o) (*(volatile u16 *)(kh_ds_io + (o)))
#define R32(o) (*(volatile u32 *)(kh_ds_io + (o)))
    int y = g_layout == 2 ? 4 : H - 6 * 10 - 4, x = 4;
    regs_line(x, y, "2D  top=%c  POW %04x  CAP %08x  3DCNT %04x  CLEAR %08x",
              top_eng ? 'B' : 'A', R16(0x304), R32(0x64), R16(0x60), R32(0x350));
    y += 10;
    regs_line(x, y, "A DISP %08x  BG %04x %04x %04x %04x  MB %04x",
              R32(0x0), R16(0x8), R16(0xa), R16(0xc), R16(0xe), R16(0x6c));
    y += 10;
    regs_line(x, y, "A BLD %04x ALPHA %04x Y %02x  WIN %04x %04x %04x %04x IN %04x OUT %04x",
              R16(0x50), R16(0x52), R16(0x54) & 0x1f, R16(0x40), R16(0x42), R16(0x44), R16(0x46),
              R16(0x48), R16(0x4a));
    y += 10;
    regs_line(x, y, "A OFS1 %03x,%03x OFS2 %03x,%03x OFS3 %03x,%03x  VRAM BG %03x OBJ %03x",
              R16(0x14) & 0x1ff, R16(0x16) & 0x1ff, R16(0x18) & 0x1ff, R16(0x1a) & 0x1ff,
              R16(0x1c) & 0x1ff, R16(0x1e) & 0x1ff, (unsigned)kh_nitro_view_banks(0),
              (unsigned)kh_nitro_view_banks(1));
    y += 10;
    regs_line(x, y, "B DISP %08x  BG %04x %04x %04x %04x  BLD %04x ALPHA %04x Y %02x",
              R32(0x1000), R16(0x1008), R16(0x100a), R16(0x100c), R16(0x100e), R16(0x1050),
              R16(0x1052), R16(0x1054) & 0x1f);
#undef R16
#undef R32
}

void kh_nitro_render_frame(void)
{
#if KH_PS2_DEBUG
    const KhProfStats *ps = kh_prof_stats();
#endif
    const KhPadState *pad = kh_input_pad(0);
    int top_eng = (*(volatile u16 *)(kh_ds_io + 0x304) & 0x8000) ? 0 : 1;
    int bot_eng = top_eng ^ 1;
    int W = kh_video_width(), H = kh_video_height();

#if KH_PS2_DEBUG
    if (pad->pressed & KH_BTN_R3) {
        if (pad->held & KH_BTN_SELECT) {
            /* Deliberately user-triggered so slow-device log I/O is outside timed loads. */
            KH_INFO("loadprof", "manual diagnostic log flush");
            kh_log_flush();
        } else {                              /* bring-up: dump the game's object/task list */
            extern void kh_debug_dump_objects(void);
            kh_debug_dump_objects();
        }
    }
#endif
    if ((pad->pressed & KH_BTN_L3) && (pad->held & KH_BTN_R3)) {
        g_regs_overlay ^= 1;
    } else if (pad->pressed & KH_BTN_L3) {
        g_layout = (g_layout + 1) % 4;
#if KH_PS2_DEBUG
        g_layout_shown = kh_vblank_count();
#endif
    }

    {
        extern void kh_ds2d_frame_begin(void);
        kh_ds2d_frame_begin();
    }
    kh_video_begin_frame(0x000000);
    switch (g_layout) {
    case 0:
        draw_screen(top_eng, W / 4, 0, W / 2, H / 2);
        if (!kh_newgame_loading())
            draw_screen(bot_eng, W / 4, H / 2, W / 2, H / 2);
        break;
    case 1:
        draw_screen(top_eng, 0, H / 4, W / 2, H / 2);
        if (!kh_newgame_loading())
            draw_screen(bot_eng, W / 2, H / 4, W / 2, H / 2);
        break;
    case 2:
        draw_screen(top_eng, 0, 0, W, H);
        break;
    default:
        if (!kh_newgame_loading())
            draw_screen(bot_eng, 0, 0, W, H);
        break;
    }
    if (g_regs_overlay)
        draw_regs_overlay(top_eng, H);
#if KH_PS2_DEBUG
    if (kh_vblank_count() - g_layout_shown < 180) {
        /* integer formatting: %f would pull in soft-float double printf every frame */
        static const char *const k_name[4] = { "vertical", "horizontal", "top screen", "bottom screen" };
        int f10 = (int)(ps->fps * 10.0f);
        kh_video_debug_text(8, H - 10, 0x80ff80, "fps %d.%d  frame %u  L3: layout (%s)", f10 / 10, f10 % 10,
                            (unsigned)kh_vblank_count(), k_name[g_layout]);
    }
#endif
}
