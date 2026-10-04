/* Real-hardware breadcrumb screen for blocking bring-up paths.
 *
 * Debug builds use the game's GS-safe 16-bit FIELD path, not libdebug, so a stage remains visible
 * even if the very next call hard-locks the EE or blocks in IOP/file I/O.  Release builds are a
 * no-op.
 */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <stdio.h>

void kh_debug_stage(const char *stage, int a, int b)
{
#if KH_PS2_DEBUG
    extern volatile const char *kh_watchdog_mark;
    static char mark[160];
    static char detail[160];
    const char *lines[2];

    snprintf(mark, sizeof mark, "%s a=%d b=%d", stage ? stage : "(null)", a, b);
    kh_watchdog_mark = mark;

    snprintf(detail, sizeof detail, "%s   a=%d   b=%d", stage ? stage : "(null)", a, b);
    lines[0] = detail;
    lines[1] = "If this remains on screen, the next call did not return.";
    ps2_gs_crash_screen("KH Days PS2 - opening probe", lines, 2);
#else
    (void)stage; (void)a; (void)b;
#endif
}
