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
#if KH_PS2_DEBUG
    static char detail[160];
    const char *lines[2];

    kh_debug_mark(stage, a, b);

    snprintf(detail, sizeof detail, "%s   a=%d   b=%d", g_stage, a, b);
    lines[0] = detail;
    lines[1] = "If this remains on screen, the next call did not return.";
    ps2_gs_crash_screen("KH Days PS2 - transition probe", lines, 2);
#else
    (void)stage; (void)a; (void)b;
#endif
}

void kh_debug_stage_overlay(void)
{
#if KH_PS2_DEBUG
    if (g_stage[0] != 0) {
        int y = kh_video_height() - 20;
        kh_video_debug_text(8, y, 0xffffff, "DBG %s  a=%d b=%d",
                            g_stage, g_stage_a, g_stage_b);
    }
#endif
}
