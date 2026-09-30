/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndLockChannel.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os.h"

void SND_LockChannel(u32 chBitMask, u32 flags);

/* khdays: shared-bss */
u32 sAlarmLock;   /* sAlarmLock */
u32 data_0204a2fc;   /* sCaptureLock */
u32 sChannelLock;   /* sChannelLock */

/* NNS_SndLockChannel -- NitroSystem resource_mgr.c: NNS_SndLockChannel. */
BOOL NNS_SndLockChannel (u32 chBitFlag)
{
    if (chBitFlag == 0) return TRUE;
    if (chBitFlag & sChannelLock) return FALSE;

    SND_LockChannel(chBitFlag, 0);

    sChannelLock |= chBitFlag;

    return TRUE;
}
