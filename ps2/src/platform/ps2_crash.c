/* EE exception screen: an unexpected exception (bad address, bus error, TLB miss, trap, ...)
 * shows what happened instead of a frozen black screen.  Addresses map back to functions with
 * the link map (build/khdays-ps2.map) or `addr2line -e build/obj/khdays-ps2/khdays-ps2.unstripped.elf`. */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <stdio.h>
#include <kernel.h>
#include <debug.h>
#include <ee_debug.h>

static const char *const k_cause[32] = {
    [1] = "TLB modified", [2] = "TLB miss (load)", [3] = "TLB miss (store)",
    [4] = "address error (load)", [5] = "address error (store)", [6] = "bus error (instruction)",
    [7] = "bus error (data)", [9] = "breakpoint", [10] = "reserved instruction",
    [11] = "coprocessor unusable", [12] = "arithmetic overflow", [13] = "trap",
};

static int crash_handler(EE_RegFrame *f)
{
    int code = (int)((f->cause >> 2) & 31);
    static char line[6][96];
    const char *screen_line[6];
    int i;

    snprintf(line[0], sizeof line[0], "EE exception %d: %s", code, k_cause[code] ? k_cause[code] : "?");
    snprintf(line[1], sizeof line[1], "EPC %08x  BadVAddr %08x  RA %08x  SP %08x",
             (unsigned)f->epc, (unsigned)f->badvaddr, f->ra[0], f->sp[0]);
    snprintf(line[2], sizeof line[2], "v0 %08x v1 %08x a0 %08x a1 %08x a2 %08x a3 %08x",
             f->v0[0], f->v1[0], f->a0[0], f->a1[0], f->a2[0], f->a3[0]);
    snprintf(line[3], sizeof line[3], "s0 %08x s1 %08x s2 %08x s3 %08x s4 %08x s5 %08x",
             f->s0[0], f->s1[0], f->s2[0], f->s3[0], f->s4[0], f->s5[0]);
    snprintf(line[4], sizeof line[4], "t0 %08x t1 %08x t2 %08x t3 %08x gp %08x fp %08x",
             f->t0[0], f->t1[0], f->t2[0], f->t3[0], f->gp[0], f->fp[0]);
    snprintf(line[5], sizeof line[5], "status %08x cause %08x", (unsigned)f->status, (unsigned)f->cause);
    for (i = 0; i < 6; i++) {
        screen_line[i] = line[i];
        printf("CRASH %s\n", line[i]);
    }
    fflush(stdout);

    if (!ps2_gs_crash_screen("Kingdom Hearts 358/2 Days (PS2) - crash", screen_line, 6)) {
        /* Very early exception, before kh_video_init(): retain the SDK fallback. */
        init_scr();
        scr_clear();
        scr_printf("\n  Kingdom Hearts 358/2 Days (PS2) - crash\n\n");
        for (i = 0; i < 6; i++)
            scr_printf("  %s\n", line[i]);
        scr_printf("\n  Map EPC/RA to functions with build/khdays-ps2.map.\n");
    }
    for (;;)
        SleepThread();
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
