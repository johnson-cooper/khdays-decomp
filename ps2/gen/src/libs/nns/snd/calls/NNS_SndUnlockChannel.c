/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndUnlockChannel.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os.h"

void SND_UnlockChannel(u32 chBitMask, u32 flags);

/* khdays: shared-bss */
u32 sAlarmLock;   /* sAlarmLock */
u32 data_0204a2fc;   /* sCaptureLock */
u32 sChannelLock;   /* sChannelLock */

/* NNS_SndUnlockChannel -- NitroSystem resource_mgr.c: NNS_SndUnlockChannel. */
void NNS_SndUnlockChannel (u32 chBitFlag)
{

    if (chBitFlag == 0) return;

    SND_UnlockChannel(chBitFlag, 0);

    sChannelLock &= ~chBitFlag;
}
