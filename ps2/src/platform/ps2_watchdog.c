/* Hang watchdog for real-hardware runs (no debugger there).
 *
 * The VBlank interrupt wakes a top-priority thread every 2 seconds.  If the game's main loop has
 * not completed a VBlank wait since the last check (kh_watchdog_progress), the thread logs the
 * last breadcrumb (kh_watchdog_mark: the file being opened/read, ...) and the state of every EE
 * thread - running, ready, or waiting on which semaphore - and commits the log file.  It logs
 * once per stall, shows a GS-safe report only after 10 seconds, then refreshes the in-RAM
 * evidence every 10 seconds while the stall continues.
 */
#include "platform/kh_platform.h"
#include "platform/kh_loadprof.h"
#include "ps2_internal.h"

#include <kernel.h>
#include <stdio.h>
#include <string.h>
#include <debug.h>

volatile uint32_t kh_watchdog_progress;
volatile const char *kh_watchdog_mark = "boot";

#if KH_PS2_DEBUG
static int g_sema = -1;
/* The diagnostic snapshot includes the bounded load trace; keep comfortable IRQ stack headroom. */
static u8 g_stack[12 * 1024] __attribute__((aligned(16)));

/* from the VBlank interrupt handler */
void kh_watchdog_vblank(uint32_t vblank)
{
    if (g_sema >= 0 && (vblank % 120) == 0)
        iSignalSema(g_sema);
}

static const char *status_name(int s)
{
    switch (s) {
    case 0x01: return "RUN";
    case 0x02: return "READY";
    case 0x04: return "WAIT";
    case 0x08: return "SUSPEND";
    case 0x0c: return "WAIT+SUSPEND";
    case 0x10: return "DORMANT";
    default: return "?";
    }
}

static void report(uint32_t now, int screen)
{
    extern void *volatile kh_io_site[64];
    extern const char *volatile kh_io_label[64];
    extern void ps2_log_ring(const char *line);
    char line[160];
    int id;

    if (!screen) {
        int lp;
        snprintf(line, sizeof line, "[W watchdog] main loop stalled (%u VBlank waits), last mark: %s\n",
                 (unsigned)now, kh_watchdog_mark ? (const char *)kh_watchdog_mark : "-");
        ps2_log_ring(line);
        for (lp = 0; lp < 4 && kh_loadprof_watchdog_line(lp, line, sizeof line - 1); lp++) {
            size_t len = strlen(line);
            if (len + 1 < sizeof line) {
                line[len] = '\n';
                line[len + 1] = 0;
            }
            ps2_log_ring(line);
        }
    } else {
        /* Never call libdebug init_scr() after the game owns the GS.  It assumes a PSMCT32
         * framebuffer at VRAM 0, while this port uses full-height PSMCT16 FIELD buffers; on
         * hardware that mismatch is the blue/patterned corruption previously seen during long
         * loads.  Build a compact report and draw it through the game's own GS-safe path. */
        char text[12][160];
        const char *lines[12];
        int n = 0;
        snprintf(text[n], sizeof text[n], "VBlank waits: %u", (unsigned)now);
        lines[n] = text[n]; n++;
        snprintf(text[n], sizeof text[n], "last mark: %s",
                 kh_watchdog_mark ? (const char *)kh_watchdog_mark : "-");
        lines[n] = text[n]; n++;

        {
            int lp;
            for (lp = 0; lp < 4 && n < 12; lp++) {
                if (!kh_loadprof_watchdog_line(lp, text[n], sizeof text[n]))
                    break;
                lines[n] = text[n];
                n++;
            }
        }

        for (id = 1; id < 64 && n < 12; id++) {
            ee_thread_status_t st;
            if (ReferThreadStatus(id, &st) < 0 || !st.status)
                continue;
            snprintf(text[n], sizeof text[n],
                     "t%02d p%02d %-7s wait %d/%d at %p iop %p %s",
                     id, st.current_priority, status_name(st.status),
                     st.waitType, st.waitId, st.func, kh_io_site[id],
                     kh_io_label[id] ? (const char *)kh_io_label[id] : "-");
            lines[n] = text[n];
            n++;
        }
        ps2_gs_crash_screen("KH Days PS2 - main loop stalled", lines, n);
        return;
    }

    for (id = 1; id < 64; id++) {
        ee_thread_status_t st;
        if (ReferThreadStatus(id, &st) < 0 || !st.status)
            continue;
        snprintf(line, sizeof line, "  thread %2d prio %3d %-7s wait %d/%d at %p iop %p %s\n", id,
                 st.current_priority, status_name(st.status), st.waitType, st.waitId, st.func,
                 kh_io_site[id], kh_io_label[id] ? (const char *)kh_io_label[id] : "-");
        ps2_log_ring(line);
    }
}

/* Nothing here goes through the IOP (no stdout, no log file): if the IOP side hangs, a report
 * that tried to log would block on it and never reach the screen. */
static void watchdog_thread(void *arg)
{
    uint32_t last = 0, stalled = 0;
    (void)arg;
    {
        extern void *volatile kh_io_site[64];
        extern const char *volatile kh_io_label[64];
        int tid = GetThreadId();
        if (tid > 0 && tid < 64) {
            kh_io_site[tid] = 0;    /* thread IDs can be reused; discard a stale previous owner */
            kh_io_label[tid] = 0;
        }
    }
    for (;;) {
        uint32_t now;
        WaitSema(g_sema);
        {
            extern volatile int kh_crash_active;
            if (kh_crash_active)
                continue;   /* preserve the primary EE exception screen/evidence */
        }
        now = kh_watchdog_progress;
        if (now != last) {
            last = now;
            stalled = 0;
            continue;
        }
        stalled++;
        if (now == 0)
            continue;               /* still in boot loading: the loop has not run yet */
        if (stalled == 1)
            report(now, 0);         /* 2 s: ring only; transient loads must not disturb the GS */
        else if (stalled == 5)
            report(now, 1);         /* 10 s: GS-safe visible report for a sustained stall */
        else if (stalled > 5 && (stalled % 5) == 0)
            report(now, 0);         /* every 10 s thereafter: keep the in-RAM evidence current */
    }
}

void kh_watchdog_start(void)
{
    ee_sema_t s;
    ee_thread_t t;
    int id;
    s.init_count = 0;
    s.max_count = 1;
    s.option = 0;
    g_sema = CreateSema(&s);
    t.func = (void *)watchdog_thread;
    t.stack = g_stack;
    t.stack_size = sizeof g_stack;
    t.gp_reg = &_gp;
    t.initial_priority = 1;
    t.attr = 0;
    t.option = 0;
    id = CreateThread(&t);
    if (id >= 0)
        StartThread(id, NULL);
}
#else
void kh_watchdog_vblank(uint32_t vblank) { (void)vblank; }
void kh_watchdog_start(void) {}
#endif
