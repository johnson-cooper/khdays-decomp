/* Platform bring-up in dependency order.  Each step logs; a failing step panics with a message
 * on screen instead of leaving a black screen. */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <stdio.h>
#include <string.h>
#include <kernel.h>
#include <sifrpc.h>

#define KH_BUILD_TAG "khdays-ps2 " __DATE__ " " __TIME__

/* where each EE thread is waiting on the IOP right now (return address of the waiting call),
 * for the hang watchdog's report */
void *volatile kh_io_site[64];
const char *volatile kh_io_label[64];

static int io_begin(const char *label, void *ra)
{
    unsigned int status;
    int tid = GetThreadId();
    if (tid > 0 && tid < 64) {
        kh_io_site[tid] = ra;
        kh_io_label[tid] = label;
    }
    __asm__ volatile("mfc0 %0, $12" : "=r"(status));
    if (status & 0x10000)               /* Status.EIE: interrupts on */
        return 0;
    EIntr();
    {   /* report each call site once: these are the waits that hang real hardware */
        static void *seen[16];
        static int nseen;
        int i;
        for (i = 0; i < nseen && seen[i] != ra; i++)
            ;
        if (i == nseen && nseen < 16) {
            seen[nseen++] = ra;
            KH_WARN("io", "IOP/VBlank wait with interrupts disabled (from %p): enabled for it", ra);
        }
    }
    return 1;
}

int kh_io_begin(void) { return io_begin("IOP/kernel wait", __builtin_return_address(0)); }
int kh_io_begin_tag(const char *label) { return io_begin(label, __builtin_return_address(0)); }

void kh_io_end(int was_off)
{
    int tid = GetThreadId();
    if (tid > 0 && tid < 64) {
        kh_io_site[tid] = 0;
        kh_io_label[tid] = 0;
    }
    if (was_off)
        DIntr();
}

static char g_self[256];              /* argv[0]: what kh_platform_restart executes again */

/* Restarts the game from scratch by executing this ELF again (the DS soft reset: the title
 * screen's "return to title" resets the system).  Everything is rebuilt by the normal boot path,
 * IOP reset included; save data is already on its device. */
void kh_platform_restart(void)
{
    char *args[1];
    KH_INFO("boot", "restart: executing %s again", g_self[0] ? g_self : "(unknown)");
    kh_log_flush();
    if (!g_self[0])
        kh_panic("restart: the boot path is unknown (no argv[0])");
    {
        /* PCSX2 boots an ELF as "host:E:dirdirkhdays-ps2.elf" (separators stripped) with host:
         * rooted at the ELF's folder (ps2_vfs.c): re-execute "host:<file name>" then */
        const char *colon = strchr(g_self, ':');
        if (colon && !strchr(colon, '/') && !strchr(colon, '\\') && !strncmp(g_self, "host", 4)) {
            const char *name = NULL, *q = g_self;
            while ((q = strstr(q, "khdays")) != NULL)
                name = q++;
            if (!name)
                name = strrchr(g_self, ':') + 1;
            memmove(g_self + 5, name, strlen(name) + 1);
            memcpy(g_self, "host:", 5);
        }
    }
    KH_INFO("boot", "restart: LoadExecPS2(%s)", g_self);
    kh_log_flush();
    args[0] = g_self;
    SifExitRpc();
    LoadExecPS2(g_self, 0, args);
    kh_panic("restart: LoadExecPS2(%s) returned", g_self);
}

void kh_platform_init(int argc, char **argv)
{
    if (argc > 0 && argv[0]) {
        strncpy(g_self, argv[0], sizeof g_self - 1);
        g_self[sizeof g_self - 1] = 0;
    }
#if KH_PS2_DEBUG
    printf("\n==== Kingdom Hearts 358/2 Days - native PS2 port (%s) ====\n", KH_BUILD_TAG);
#endif
    KH_INFO("boot", "argv[0] = %s", argc > 0 && argv[0] ? argv[0] : "(none)");

    ps2_crash_install();                  /* exceptions show a crash screen, not a black one */
    ps2_iop_reset_and_load_base();        /* IOP: sio2man, padman, mcman, iomanX, fileXio */
    KH_INFO("boot", "IOP ready");
    kh_time_init();
    kh_mem_init();                        /* arenas before anything allocates */
    KH_INFO("boot", "memory ready");
    kh_vfs_init(argc, argv);              /* boot device drivers + path */
    ps2_log_open_file();
    kh_input_init();
    KH_INFO("boot", "pads ready");
    kh_video_init(KH_VIDEO_AUTO);
    ps2_time_install_vblank();            /* after graph_* set the mode */
    {
        extern void kh_watchdog_start(void);
        kh_watchdog_start();              /* logs thread states if the main loop stalls */
    }
    if (kh_audio_init() < 0)
        KH_WARN("boot", "audio unavailable, continuing without sound");
#if KH_PS2_DEBUG
    kh_mem_report();
#endif
}

void kh_platform_shutdown(void)
{
    kh_log_flush();
}
