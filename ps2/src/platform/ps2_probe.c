/* Real-hardware breadcrumb screen for blocking bring-up paths.
 *
 * Debug builds use the game's GS-safe 16-bit FIELD path, not libdebug.  The latest stage is also
 * redrawn over every normal frame so a later compositor pass cannot erase the breadcrumb.
 */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <stdio.h>
#include <string.h>
#include <kernel.h>

#if KH_PS2_DEBUG
static char g_stage[160];
static int g_stage_a;
static int g_stage_b;
static int g_calendar_phase;
static int g_calendar_gate;
static int g_calendar_position;
static int g_calendar_elapsed;
static int g_calendar_phase_frame;
static int g_calendar_complete;
static int g_calendar_entry_phase = -1;
static unsigned int g_calendar_calls;
/* main-loop step (kh_debug_loop_mark): watchdog/stall screens only, never the overlay DBG line */
static char g_loop_mark[96];
#endif

/*
 * Live handoff trace.  While armed (main.c: from the frame the calendar requests the field until
 * the field has presented a while), every breadcrumb from the main thread is drawn IMMEDIATELY on
 * the picture currently on the TV, one numbered line per mark in a rolling 20-line list.  This is
 * for hangs that run with EE interrupts disabled: those stop the VBlank interrupt, so neither the
 * watchdog nor the crash/stall screens can ever draw, and only text already on screen survives.
 * The highest number is the last step the CPU reached.
 */
#if KH_PS2_DEBUG
#define LIVE_ROWS 20
static int g_live_on;
static int g_live_tid = -1;
static unsigned int g_live_seq;
#endif

void kh_debug_live_marks(int on)
{
#if KH_PS2_DEBUG
    extern void kh_video_live_text(int x, int y, uint32_t rgb, const char *text);
    if (on && !g_live_on) {
        g_live_tid = GetThreadId();
        g_live_on = 1;
        kh_video_live_text(8, 24, 0x40ffff, "LIVE handoff trace: highest number = last step reached");
        return;
    }
    if (!on)
        g_live_on = 0;
#else
    (void)on;
#endif
}

#if KH_PS2_DEBUG
static void live_mark(const char *text)
{
    extern void kh_video_live_text(int x, int y, uint32_t rgb, const char *text);
    char line[112];
    unsigned int row;

    if (!g_live_on || GetThreadId() != g_live_tid)
        return;
    row = g_live_seq % LIVE_ROWS;
    snprintf(line, sizeof line, "%04u %s", g_live_seq & 0xffffu, text);
    g_live_seq++;
    kh_video_live_text(8, 40 + (int)row * 12, 0xffff40, line);
    /* mark where the next line goes so the newest is easy to find even after wrapping */
    kh_video_live_text(8, 40 + (int)((row + 1) % LIVE_ROWS) * 12, 0x808080, "----");
}
#endif

/* ov004 phase at the START of Ov004_StepSceneFrame, before its phase handler runs.  The CAL line
 * shows "in=3 ph=4 done=0" on the frame where phase 3 (fade-out) hands over to phase 4: handler 4
 * (Ov004_MarkTransitionComplete, done=1) only runs on the next frame. */
void kh_debug_calendar_entry(int phase)
{
#if KH_PS2_DEBUG
    g_calendar_entry_phase = phase;
#else
    (void)phase;
#endif
}

/* Main-loop step breadcrumb: updates the watchdog mark but not the overlay's DBG stage.  The
 * overlay is drawn inside KhNitro_PresentFrame, so a loop step such as "present" would otherwise
 * be printed on every successfully presented frame and hide the last game-side breadcrumb. */
void kh_debug_loop_mark(const char *stage, int a, int b)
{
#if KH_PS2_DEBUG
    extern volatile const char *kh_watchdog_mark;
    snprintf(g_loop_mark, sizeof g_loop_mark, "%s a=%d b=%d", stage ? stage : "(null)", a, b);
    kh_watchdog_mark = g_loop_mark;
    live_mark(g_loop_mark);
#else
    (void)stage; (void)a; (void)b;
#endif
}

void kh_debug_calendar_state(int phase, int gate, int position, int elapsed,
                             int phase_frame, int complete)
{
#if KH_PS2_DEBUG
    g_calendar_phase = phase;
    g_calendar_gate = gate;
    g_calendar_position = position;
    g_calendar_elapsed = elapsed;
    g_calendar_phase_frame = phase_frame;
    g_calendar_complete = complete;
    g_calendar_calls++;
#else
    (void)phase; (void)gate; (void)position; (void)elapsed;
    (void)phase_frame; (void)complete;
#endif
}

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
    live_mark(mark);
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
            {
                /* Pacing at a glance: frame-rate mode (gObjSystem byte 0: 0/1 = wait 1/2 VBlanks
                 * per frame, 2 = none), the game's VBlank counter against the real one (they
                 * advance together unless the game's VBlank IRQ stopped), and fps. */
                extern unsigned char gObjSystem;
                extern unsigned int data_027e0088;
                const KhProfStats *ps = kh_prof_stats();
                int f10 = (int)(ps->fps * 10.0f);
                kh_video_debug_text(8, y - 70, 0xffffff,
                                    "PERF fps %d.%d mode %d gamevb %u vb %u aud %uus load %uus",
                                    f10 / 10, f10 % 10, (int)gObjSystem,
                                    data_027e0088, (unsigned)kh_vblank_count(),
                                    (unsigned)ps->zone_us[KH_PROF_AUDIO],
                                    (unsigned)ps->zone_us[KH_PROF_LOAD]);
                {
                    extern int kh_vfs_stream_stats(char *out, int n);
                    char sl[100];
                    kh_vfs_stream_stats(sl, (int)sizeof sl);
                    kh_video_debug_text(8, y - 84, 0xffffff, "%s", sl);
                }
            }
            if (scene[2] == 5) {
                kh_video_debug_text(8, y - 28, 0xffffff,
                                    "CAL in=%d ph=%d gate=%d pos=%x age=%d",
                                    g_calendar_entry_phase, g_calendar_phase, g_calendar_gate,
                                    g_calendar_position, g_calendar_elapsed);
                kh_video_debug_text(8, y - 42, 0xffffff,
                                    "CAL pfrm=%d done=%d calls=%u",
                                    g_calendar_phase_frame, g_calendar_complete,
                                    g_calendar_calls);
            }
        }
    }
#endif
}

/*
 * Main-thread report for "the loop is alive but nothing has been presented for seconds"
 * (nitro_core.c check_present_stall).  The hang watchdog cannot see that case because VBlank
 * waits keep happening.  Called between frames from OS_WaitVBlankIntr, so the frame packet is
 * free; the report is flipped in at the next VBlank like any frame.
 */
void kh_debug_present_stall_screen(unsigned idle, unsigned presents, unsigned game_vblank,
                                   unsigned irq_mask, const void *vblank_fn)
{
#if KH_PS2_DEBUG
    extern volatile const char *kh_watchdog_mark;
    extern char gSceneCtl[];
    int *scene = (int *)gSceneCtl;
    int *obj = (int *)scene[0];
    unsigned int p = (unsigned int)obj;
    int state = 0x7fffffff, flags = 0;
    int y = 24;

    if (p >= 0x00010000u && p < 0x02000000u) {
        flags = obj[0];
        state = obj[5];
    }

    kh_video_begin_frame(0x180000);
    kh_video_debug_text(16, y, 0xffffff, "KH Days PS2 - loop alive, no frame presented");
    y += 20;
    kh_video_debug_text(16, y, 0xffffff, "idle %u VBlanks  presents %u  vblank %u",
                        idle, presents, (unsigned)kh_vblank_count());
    y += 14;
    kh_video_debug_text(16, y, 0xffffff, "game vblank %u  irq mask %x  vblank fn %p",
                        game_vblank, irq_mask, vblank_fn);
    y += 14;
    kh_video_debug_text(16, y, 0xffffff, "loop: %s", g_loop_mark[0] ? g_loop_mark : "-");
    y += 14;
    kh_video_debug_text(16, y, 0xffffff, "mark: %s",
                        kh_watchdog_mark ? (const char *)kh_watchdog_mark : "-");
    y += 14;
    kh_video_debug_text(16, y, 0xffffff, "DBG %s  a=%d b=%d", g_stage, g_stage_a, g_stage_b);
    y += 14;
    kh_video_debug_text(16, y, 0xffffff, "SCN cur=%d pend=%d arg=%d state=%d flags=%x",
                        scene[2], scene[3], scene[4], state, flags);
    y += 14;
    kh_video_debug_text(16, y, 0xffffff, "CAL in=%d ph=%d done=%d calls=%u age=%d",
                        g_calendar_entry_phase, g_calendar_phase, g_calendar_complete,
                        g_calendar_calls, g_calendar_elapsed);
    kh_video_submit_frame();
#else
    (void)idle; (void)presents; (void)game_vblank; (void)irq_mask; (void)vblank_fn;
#endif
}
