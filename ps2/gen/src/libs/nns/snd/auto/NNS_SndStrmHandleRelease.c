/* PS2: mechanically prepared copy of libs/nns/snd/auto/NNS_SndStrmHandleRelease.c (ps2/tools/prep_sources.py). Do not edit. */


/* khdays: shared-bss */

#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* NNS_SndStrmHandleRelease -- NitroSystem sndarc_stream.c: NNS_SndStrmHandleRelease. */
void NNS_SndStrmHandleRelease (NNSSndStrmHandle * handle)
{

    if (handle->player == NULL) return;

    handle->player->handle = NULL;
    handle->player = NULL;
}
