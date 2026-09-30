/* PS2: mechanically prepared copy of libs/nns/snd/auto/NNS_SndArcStrmStartPrepared.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

inline BOOL NNS_SndStrmHandleIsValid (const NNSSndStrmHandle * handle)
{
    return handle->player != NULL ;
}

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* NNS_SndArcStrmStartPrepared -- NitroSystem sndarc_stream.c: NNS_SndArcStrmStartPrepared. */
void NNS_SndArcStrmStartPrepared (NNSSndStrmHandle * handle)
{

    if (!NNS_SndStrmHandleIsValid(handle)) return;

    handle->player->startFlag = TRUE;
}
