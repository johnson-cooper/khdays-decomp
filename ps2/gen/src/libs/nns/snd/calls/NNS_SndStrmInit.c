/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndStrmInit.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

#define offsetof(type, member) ((u32)&(((type *)0)->member))

static inline void PM_SetSleepCallbackInfo (PMSleepCallbackInfo * info, PMSleepCallback callback, void * arg)
{
    info->callback = callback;
    info->arg = arg;
}
void NNS_FndInitList(NNSFndList * list, u16 offset);
extern NNSFndList data_0204abe4;
extern void BeginSleep_2(void * arg);
extern void EndSleep(void * arg);
extern void BeginSleep_2 (void * arg);
extern void EndSleep (void * arg);

/* khdays: shared-bss */
BOOL data_0204abe0;   /* bInitialized$2853 */

/* NNS_SndStrmInit -- NitroSystem stream.c: NNS_SndStrmInit. */
void NNS_SndStrmInit (NNSSndStrm * stream)
{

    {

        if (!data_0204abe0) {
            NNS_FND_INIT_LIST(&data_0204abe4, NNSSndStrm, link);
            data_0204abe0 = TRUE;
        }
    }

    PM_SetSleepCallbackInfo(&stream->preSleepInfo, BeginSleep_2, stream);
    PM_SetSleepCallbackInfo(&stream->postSleepInfo, EndSleep, stream);

    stream->chBitMask = 0;
    stream->numChannels = 0;
    stream->activeFlag = FALSE;
    stream->startFlag = FALSE;
}
