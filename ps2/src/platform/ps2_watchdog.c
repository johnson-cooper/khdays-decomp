/* Hang watchdog for real-hardware runs (no debugger there).
 *
 * The VBlank interrupt wakes a top-priority thread every 2 seconds.  If the game's main loop has
 * not completed a VBlank wait since the last check (kh_watchdog_progress), the thread logs the
 * last breadcrumb (kh_watchdog_mark: the file being opened/read, ...) and the state of every EE
 * thread - running, ready, or waiting on which semaphore - and commits the log file.  It logs
 * once per stall, then again only if the stall goes on (every 10 s).
 */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <kernel.h>
#include <stdio.h>
#include <debug.h>

volatile uint32_t kh_watchdog_progress;
volatile const char *kh_watchdog_mark = "boot";

#if KH_PS2_DEBUG
static int g_sema = -1;
static u8 g_stack[8 * 1024] __attribute__((aligned(16)));

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
    extern void ps2_log_ring(const char *line);
    extern void ps2_log_print_recent(int lines);
    char line[160];
    int id;
    if (screen) {
        init_scr();
        scr_clear();
        scr_printf("\n  KH Days PS2 - hang report (main loop stalled, %u VBlank waits)\n", (unsigned)now);
        scr_printf("  last mark: %s\n\n", kh_watchdog_mark ? (const char *)kh_watchdog_mark : "-");
    } else {
        snprintf(line, sizeof line, "[W watchdog] main loop stalled (%u VBlank waits), last mark: %s\n",
                 (unsigned)now, kh_watchdog_mark ? (const char *)kh_watchdog_mark : "-");
        ps2_log_ring(line);
    }
    for (id = 1; id < 64; id++) {
        ee_thread_status_t st;
        if (ReferThreadStatus(id, &st) < 0 || !st.status)
            continue;
        snprintf(line, sizeof line, "  thread %2d prio %3d %-7s wait %d/%d at %p  iop %p\n", id,
                 st.current_priority, status_name(st.status), st.waitType, st.waitId, st.func, kh_io_site[id]);
        if (screen)
            scr_printf("%s", line);
        else
            ps2_log_ring(line);
    }
    if (screen) {
        scr_printf("\n  recent log:\n");
        ps2_log_print_recent(14);
    }
}

/* Nothing here goes through the IOP (no stdout, no log file): if the IOP side hangs, a report
 * that tried to log would block on it and never reach the screen. */
static void watchdog_thread(void *arg)
{
    uint32_t last = 0, stalled = 0;
    (void)arg;
    for (;;) {
        uint32_t now;
        WaitSema(g_sema);
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
            report(now, 0);         /* 2 s: into the ring (shown on screen and in the panic report) */
        else if (stalled == 2)
            report(now, 1);         /* 4 s: on screen */
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
