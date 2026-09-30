/* Frame composition: the DS screens' logical state -> GS.
 *
 * The DS has two 256x192 screens; the PS2 one television picture.  Until the per-scene
 * single-screen presentation exists (docs/PS2_PORT.md, "Dual-screen UI conversion"), the
 * layout is selectable with L3:
 *   0  both screens side by side (bring-up / debug view)
 *   1  top screen full, bottom screen as an inset
 *   2  bottom screen full, top screen as an inset
 * POWCNT bit 15 says which 2D engine drives the top screen, as on the DS.
 */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

extern void kh_ds2d_draw(int eng, int x, int y, int w, int h);
extern void kh_ge_render(int x, int y, int w, int h);

/* One DS screen: the 3D layer (engine A's BG0 in 3D mode) under the 2D layers. */
static void draw_screen(int eng, int x, int y, int w, int h)
{
    if (eng == 0 && (*(volatile u32 *)kh_ds_io & 8))
        kh_ge_render(x, y, w, h);
    kh_ds2d_draw(eng, x, y, w, h);
}

static int g_layout;

void kh_nitro_render_frame(void)
{
    const KhProfStats *ps = kh_prof_stats();
    const KhPadState *pad = kh_input_pad(0);
    int top_eng = (*(volatile u16 *)(kh_ds_io + 0x304) & 0x8000) ? 0 : 1;
    int bot_eng = top_eng ^ 1;
    int W = kh_video_width(), H = kh_video_height();

    if (pad->pressed & KH_BTN_L3)
        g_layout = (g_layout + 1) % 3;

    kh_video_begin_frame(0x000000);
    switch (g_layout) {
    case 0:
        draw_screen(top_eng, 0, (H - 192) / 2, W / 2, 192);
        draw_screen(bot_eng, W / 2, (H - 192) / 2, W / 2, 192);
        break;
    case 1:
        draw_screen(top_eng, (W - 548) / 2, 0, 548, H);
        draw_screen(bot_eng, W - 160, H - 60, 160, 60);
        break;
    default:
        draw_screen(bot_eng, (W - 548) / 2, 0, 548, H);
        draw_screen(top_eng, W - 160, H - 60, 160, 60);
        break;
    }
    kh_video_debug_text(8, H - 10, 0x80ff80, "fps %.1f  frame %u  L3: layout", ps->fps, (unsigned)kh_vblank_count());
}
