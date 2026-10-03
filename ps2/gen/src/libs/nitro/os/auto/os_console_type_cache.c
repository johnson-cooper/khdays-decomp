/* PS2: mechanically prepared copy of libs/nitro/os/auto/os_console_type_cache.c (ps2/tools/prep_sources.py). Do not edit. */
/* NitroSDK os_emulator.c: OSi_ConsoleTypeCache, the console type OS_GetConsoleType caches;
 * OSi_CONSOLE_NOT_DETECT (-1) until the first call. */

#include "nitro/types.h"
#include "nitro/os.h"

u32 data_020422b0 __attribute__((aligned(__alignof__(u32)))) = OSi_CONSOLE_NOT_DETECT;
