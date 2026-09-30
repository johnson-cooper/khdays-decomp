/* PS2: mechanically prepared copy of libs/nns/snd/calls/AllocPlayer.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

inline BOOL NNS_SndStrmHandleIsValid (const NNSSndStrmHandle * handle)
{
    return handle->player != NULL ;
}
void NNS_SndStrmHandleRelease(struct NNSSndStrmHandle * handle);
extern NNSSndStrmPlayer data_0204b62c[4 ];
extern void ForceStopStrm_2(NNSSndStrmPlayer * player);
extern void NNS_SndStrmHandleRelease (NNSSndStrmHandle * handle);
extern void ForceStopStrm_2 (NNSSndStrmPlayer * player);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* AllocPlayer -- NitroSystem sndarc_stream.c: AllocPlayer. */
NNSSndStrmPlayer * AllocPlayer (NNSSndStrmHandle * handle, int playerNo, int prio)
{
    NNSSndStrmPlayer * player;

    if (NNS_SndStrmHandleIsValid(handle)) {
        NNS_SndStrmHandleRelease(handle);
    }

    player = &data_0204b62c[ playerNo ];

    if (player->buffer == NULL) return NULL;

    if (player->activeFlag) {
        if (prio < player->prio) return NULL;
        ForceStopStrm_2(player);
    }

    player->prio = prio;
    player->activeFlag = TRUE;

    player->handle = handle;
    handle->player = player;

    return player;
}
