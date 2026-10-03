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
#include "nitro_internal.h"

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
    if (pad->pressed & KH_BTN_R3) {           /* bring-up: dump the game's object/task list */
        extern void kh_debug_dump_objects(void);
        kh_debug_dump_objects();
    }
#endif
    if (pad->pressed & KH_BTN_L3) {
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
        draw_screen(bot_eng, W / 4, H / 2, W / 2, H / 2);
        break;
    case 1:
        draw_screen(top_eng, 0, H / 4, W / 2, H / 2);
        draw_screen(bot_eng, W / 2, H / 4, W / 2, H / 2);
        break;
    case 2:
        draw_screen(top_eng, 0, 0, W, H);
        break;
    default:
        draw_screen(bot_eng, 0, 0, W, H);
        break;
    }
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
