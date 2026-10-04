/* Real-hardware breadcrumb screen for blocking bring-up paths.
 *
 * Debug builds use the game's GS-safe 16-bit FIELD path, not libdebug.  The latest stage is also
 * redrawn over every normal frame so a later compositor pass cannot erase the breadcrumb.
 */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <stdio.h>
#include <string.h>

#if KH_PS2_DEBUG
static char g_stage[160];
static int g_stage_a;
static int g_stage_b;
#endif

void kh_debug_mark(const char *stage, int a, int b)
{
#if KH_PS2_DEBUG
    extern volatile const char *kh_watchdog_mark;
    static char mark[160];

    snprintf(g_stage, sizeof g_stage, "%s", stage ? stage : "(null)");
    g_stage_a = a;
    g_stage_b = b;
    snprintf(mark, sizeof mark, "%s a=%d b=%d", g_stage, a, b);
    kh_watchdog_mark = mark;
#else
    (void)stage; (void)a; (void)b;
#endif
}

void kh_debug_stage(const char *stage, int a, int b)
{
    /* Stage probes used to draw a full-screen GS report immediately.  That was useful during
     * early bring-up, but it also overwrote the game's own output and could perturb VIF/GIF/GS
     * state during the opening -> field handoff.  Keep the same call sites as lightweight
     * breadcrumbs only; a real sustained stall is displayed by the watchdog. */
    kh_debug_mark(stage, a, b);
}

void kh_debug_stage_overlay(void)
{
#if KH_PS2_DEBUG
    if (g_stage[0] != 0) {
        int y = kh_video_height() - 20;
        kh_video_debug_text(8, y, 0xffffff, "DBG %s  a=%d b=%d",
                            g_stage, g_stage_a, g_stage_b);
        {
            /* Keep the scene handoff state visible even when the last breadcrumb came from an
             * object callback.  gSceneCtl is five words: obj, entry, curId, pendId, pendArg. */
            extern char gSceneCtl[];
            int *scene = (int *)gSceneCtl;
            int *obj = (int *)scene[0];
            int state = 0x7fffffff;
            int flags = 0;
            unsigned int p = (unsigned int)obj;

            /* Only dereference plausible EE RAM pointers; a stale scene pointer is itself useful
             * evidence and must not make the diagnostic overlay crash. */
            if (p >= 0x00010000u && p < 0x02000000u) {
                flags = obj[0];
                state = obj[5];
            }
            kh_video_debug_text(8, y - 14, 0xffffff,
                                "SCN cur=%d pend=%d arg=%d state=%d flags=%x",
                                scene[2], scene[3], scene[4], state, flags);
        }
    }
#endif
}
