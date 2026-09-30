/* PS2: mechanically prepared copy of libs/nns/snd/auto/FreePlayer.c (ps2/tools/prep_sources.py). Do not edit. */


/* khdays: shared-bss */

#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* FreePlayer -- NitroSystem sndarc_stream.c: FreePlayer. */
void FreePlayer (NNSSndStrmPlayer * player)
{

    if (player->handle != NULL) {
        player->handle->player = NULL;
        player->handle = NULL;
    }

    player->activeFlag = FALSE;
    player->startFlag = FALSE;
    player->playFlag = FALSE;
}
