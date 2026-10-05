/* EE exception screen: an unexpected exception (bad address, bus error, TLB miss, trap, ...)
 * shows what happened instead of a frozen black screen.  Addresses map back to functions with
 * the link map (build/khdays-ps2.map) or `addr2line -e build/obj/khdays-ps2/khdays-ps2.unstripped.elf`. */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <stdio.h>
#include <string.h>
#include <kernel.h>
#include <debug.h>
#include <ee_debug.h>

volatile int kh_crash_active;

static const char *const k_cause[32] = {
    [1] = "TLB modified", [2] = "TLB miss (load)", [3] = "TLB miss (store)",
    [4] = "address error (load)", [5] = "address error (store)", [6] = "bus error (instruction)",
    [7] = "bus error (data)", [9] = "breakpoint", [10] = "reserved instruction",
    [11] = "coprocessor unusable", [12] = "arithmetic overflow", [13] = "trap",
};

/*
 * Two-stage crash report.
 *
 * ee_debug calls level-1 handlers in EXCEPTION context (EXL set, interrupts off, on its private
 * l1 stack) and, after the handler returns, _ee_load_frame restores EPC/SP/Status from the frame
 * and erets.  The old handler did printf + fflush(stdout) and the GS report right there: stdout is
 * a SIF RPC to the IOP, which waits for an interrupt that can never arrive with EXL set, so on
 * hardware the handler hung before drawing anything.  Because kh_crash_active was already set the
 * hang watchdog stayed silent too, and the TV simply kept the last presented frame - a crash that
 * looks exactly like a freeze (the DAY 255 card after the calendar's teardown began).
 *
 * Now the handler only snapshots the registers, then rewrites the frame so the eret "returns" into
 * crash_report() in ordinary thread context, on a dedicated stack (the faulting stack may be the
 * problem).  crash_report draws the GS screen first and only then touches stdout.
 */
static EE_RegFrame g_snap;
static char g_line[11][128];
static int g_tid;
static u32 g_live_status, g_live_cause, g_live_epc, g_live_badvaddr;
static u8 g_report_stack[16 * 1024] __attribute__((aligned(16)));

static void crash_park(void)
{
    for (;;)
        SleepThread();
}

static void crash_report(void)
{
    const char *screen_line[11];
    extern volatile const char *kh_watchdog_mark;
    /* weak: the platform-test ELF links this library without the game */
    extern char gSceneCtl[] __attribute__((weak));
    static const int k_no_scene[5];
    const EE_RegFrame *f = &g_snap;
    const int *scene = gSceneCtl ? (const int *)gSceneCtl : k_no_scene;
    int code, i;

    EIntr();                    /* the fault may have hit inside a DIntr() section */
    g_tid = GetThreadId();
    code = (int)((f->cause >> 2) & 31);
    snprintf(g_line[0], sizeof g_line[0], "EE exception %d: %s  thread %d",
             code, k_cause[code] ? k_cause[code] : "?", g_tid);
    snprintf(g_line[1], sizeof g_line[1], "last mark: %s",
             kh_watchdog_mark ? (const char *)kh_watchdog_mark : "-");
    snprintf(g_line[2], sizeof g_line[2], "frame EPC %08x BadV %08x RA %08x SP %08x",
             (unsigned)f->epc, (unsigned)f->badvaddr, f->ra[0], f->sp[0]);
    snprintf(g_line[3], sizeof g_line[3], "live  EPC %08x BadV %08x status %08x cause %08x",
             (unsigned)g_live_epc, (unsigned)g_live_badvaddr,
             (unsigned)g_live_status, (unsigned)g_live_cause);
    snprintf(g_line[4], sizeof g_line[4], "v0 %08x v1 %08x a0 %08x a1 %08x a2 %08x a3 %08x",
             f->v0[0], f->v1[0], f->a0[0], f->a1[0], f->a2[0], f->a3[0]);
    snprintf(g_line[5], sizeof g_line[5], "s0 %08x s1 %08x s2 %08x s3 %08x s4 %08x s5 %08x",
             f->s0[0], f->s1[0], f->s2[0], f->s3[0], f->s4[0], f->s5[0]);
    snprintf(g_line[6], sizeof g_line[6], "t0 %08x t1 %08x t2 %08x t3 %08x gp %08x fp %08x",
             f->t0[0], f->t1[0], f->t2[0], f->t3[0], f->gp[0], f->fp[0]);
    snprintf(g_line[7], sizeof g_line[7], "s6 %08x s7 %08x t8 %08x t9 %08x status %08x",
             f->s6[0], f->s7[0], f->t8[0], f->t9[0], (unsigned)f->status);
    snprintf(g_line[8], sizeof g_line[8], "SCN cur=%d pend=%d obj=%08x",
             scene[2], scene[3], (unsigned)scene[0]);
    snprintf(g_line[9], sizeof g_line[9], "Map frame EPC and RA with build/khdays-ps2.map");
    g_line[10][0] = 0;

    for (i = 0; i < 10; i++)
        screen_line[i] = g_line[i];
    if (!ps2_gs_crash_screen("Kingdom Hearts 358/2 Days (PS2) - crash", screen_line, 10)) {
        /* Very early exception, before kh_video_init(): retain the SDK fallback. */
        init_scr();
        scr_clear();
        scr_printf("\n  Kingdom Hearts 358/2 Days (PS2) - crash\n\n");
        for (i = 0; i < 10; i++)
            scr_printf("  %s\n", g_line[i]);
    }

    /* Only now risk the IOP: the screen is already up if this blocks. */
    for (i = 0; i < 10; i++)
        printf("CRASH %s\n", g_line[i]);
    fflush(stdout);
    crash_park();
}

static void redirect(EE_RegFrame *f, void (*to)(void))
{
    u32 sp = ((u32)(uintptr_t)(g_report_stack + sizeof g_report_stack) - 64u) & ~15u;
    f->epc = (u32)(uintptr_t)to;
    f->sp[0] = sp;
    f->sp[1] = f->sp[2] = f->sp[3] = 0;
    f->ra[0] = (u32)(uintptr_t)crash_park;
    f->ra[1] = f->ra[2] = f->ra[3] = 0;
}

static int crash_handler(EE_RegFrame *f)
{
    /* A fault inside the report itself: park that thread; the first report keeps the evidence. */
    if (kh_crash_active) {
        redirect(f, crash_park);
        return 0;
    }
    kh_crash_active = 1;
    memcpy(&g_snap, f, sizeof g_snap);
    /* no syscalls here (GetThreadId etc.): crash_report runs on the same thread and asks there */
    __asm__ volatile("mfc0 %0, $12" : "=r"(g_live_status));
    __asm__ volatile("mfc0 %0, $13" : "=r"(g_live_cause));
    __asm__ volatile("mfc0 %0, $14" : "=r"(g_live_epc));
    __asm__ volatile("mfc0 %0, $8"  : "=r"(g_live_badvaddr));

    redirect(f, crash_report);
    return 0;
}

void ps2_crash_install(void)
{
    static const int causes[] = { 1, 2, 3, 4, 5, 6, 7, 10, 12, 13 };
    unsigned i;
    ee_dbg_install(1);
    for (i = 0; i < sizeof causes / sizeof causes[0]; i++)
        ee_dbg_set_level1_handler(causes[i], crash_handler);
}
