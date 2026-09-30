/* PS2: mechanically prepared copy of libs/nns/snd/calls/ForceStopStrm_2.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

void OS_LockMutex(OSMutex * mutex);
void OS_UnlockMutex(OSMutex * mutex);
void NNS_SndStrmStop(NNSSndStrm * stream);
extern NNSSndStrmThread data_0204b140;
extern void OSi_DestroyThread(NNSSndStrmPlayer * player);
extern void OSi_DestroyThread (NNSSndStrmPlayer * player);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* ForceStopStrm_2 -- NitroSystem sndarc_stream.c: ForceStopStrm. */
void ForceStopStrm_2 (NNSSndStrmPlayer * player)
{

    OS_LockMutex(&data_0204b140.mutex);
    if (sPrepareThread) OS_LockMutex(&sPrepareThread->mutex);

    if (player->playFlag) {
        NNS_SndStrmStop(&player->stream);
    }

    if (player->activeFlag) {
        player->cancelStreamFunc(player);
    }

    OSi_DestroyThread(player);

    OS_UnlockMutex(&data_0204b140.mutex);
    if (sPrepareThread) OS_UnlockMutex(&sPrepareThread->mutex);
}
