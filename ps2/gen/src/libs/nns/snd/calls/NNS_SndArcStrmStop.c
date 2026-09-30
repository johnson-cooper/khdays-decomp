/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndArcStrmStop.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

#define offsetof(type, member) ((u32)&(((type *)0)->member))

inline BOOL NNS_SndStrmHandleIsValid (const NNSSndStrmHandle * handle)
{
    return handle->player != NULL ;
}
extern void SNDi_FreeVoiceChannel(NNSSndStrmPlayer * player, int fadeFrame);
extern void SNDi_FreeVoiceChannel (NNSSndStrmPlayer * player, int fadeFrame);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* NNS_SndArcStrmStop -- NitroSystem sndarc_stream.c: NNS_SndArcStrmStop. */
void NNS_SndArcStrmStop (NNSSndStrmHandle * handle, int fadeFrame)
{

    if (!NNS_SndStrmHandleIsValid(handle)) return;

    SNDi_FreeVoiceChannel(handle->player, fadeFrame);
}
