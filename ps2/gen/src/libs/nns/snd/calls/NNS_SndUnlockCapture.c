/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndUnlockCapture.c (ps2/tools/prep_sources.py). Do not edit. */


/* khdays: shared-bss */

#include "nitro/types.h"
#include "nitro/os.h"

u32 sAlarmLock;   /* sAlarmLock */
u32 data_0204a2fc;   /* sCaptureLock */
u32 sChannelLock;   /* sChannelLock */

/* NNS_SndUnlockCapture -- NitroSystem resource_mgr.c: NNS_SndUnlockCapture. */
void NNS_SndUnlockCapture (u32 capBitFlag)
{
    data_0204a2fc &= ~capBitFlag;
}
