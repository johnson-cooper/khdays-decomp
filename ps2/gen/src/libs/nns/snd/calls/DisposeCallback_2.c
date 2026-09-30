/* PS2: mechanically prepared copy of libs/nns/snd/calls/DisposeCallback_2.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

void OS_LockMutex(OSMutex * mutex);
void OS_UnlockMutex(OSMutex * mutex);
void NNS_SndStrmFreeChannel(NNSSndStrm * stream);
extern NNSSndStrmThread data_0204b140;
extern void ForceStopStrm_2(NNSSndStrmPlayer * player);
extern void ForceStopStrm_2 (NNSSndStrmPlayer * player);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* DisposeCallback_2 -- NitroSystem sndarc_stream.c: DisposeCallback. */
void DisposeCallback_2 (void * mem, u32, u32 data1, u32)
{
    NNSSndStrmPlayer * player = (NNSSndStrmPlayer *)data1;

    if (mem == player->buffer) {
        OS_LockMutex(&data_0204b140.mutex);
        if (sPrepareThread) OS_LockMutex(&sPrepareThread->mutex);

        ForceStopStrm_2(player);

        player->buffer = NULL;
        player->bufSize = 0;

        player->numChannels = 0;
        if (player->allocChannelCount > 0) {
            NNS_SndStrmFreeChannel(&player->stream);
            player->allocChannelCount = 0;
        }

        OS_UnlockMutex(&data_0204b140.mutex);
        if (sPrepareThread) OS_UnlockMutex(&sPrepareThread->mutex);
    }
}
