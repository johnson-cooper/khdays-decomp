/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndArcStrmGetCurrentPlayingPos.c (ps2/tools/prep_sources.py). Do not edit. */


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

/* NNS_SndArcStrmGetCurrentPlayingPos -- NitroSystem sndarc_stream.c: NNS_SndArcStrmGetCurrentPlayingPos. */
u32 NNS_SndArcStrmGetCurrentPlayingPos (NNSSndStrmHandle * handle)
{
    NNSSndStrmPlayer * player;
    u64 pos;

    if (!NNS_SndStrmHandleIsValid(handle)) return 0;

    player = handle->player;

    pos = player->curSample;
    pos *= 1000;
    pos /= player->info.sampleRate;

    return (u32)pos;
}
