/* Platform bring-up in dependency order.  Each step logs; a failing step panics with a message
 * on screen instead of leaving a black screen. */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <stdio.h>
#include <kernel.h>

#define KH_BUILD_TAG "khdays-ps2 " __DATE__ " " __TIME__

void kh_platform_init(int argc, char **argv)
{
    printf("\n==== Kingdom Hearts 358/2 Days - native PS2 port (%s) ====\n", KH_BUILD_TAG);
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
    if (kh_audio_init() < 0)
        KH_WARN("boot", "audio unavailable, continuing without sound");
    kh_mem_report();
}

void kh_platform_shutdown(void)
{
    kh_log_flush();
}
