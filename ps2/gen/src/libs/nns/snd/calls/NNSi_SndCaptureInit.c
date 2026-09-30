/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNSi_SndCaptureInit.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nnsys/snd.h"

#define offsetof(type, member) ((u32)&(((type *)0)->member))

typedef struct CaptureParam {
    BOOL activeFlag;
    NNSSndCaptureType type;
    NNSSndCaptureFormat format;
    void * bufferL;
    void * bufferR;
    u32 bufLen;
    u32 blockSize;
    int curBuffer;
    u32 chBitMask;
    u32 playChBitMask;
    u32 capBitMask;
    int alarmNo;
    int interval;
    NNSSndCaptureCallback callback;
    void * callbackArg;
    NNSSndFader fader;
    BOOL fadeOutFlag;
    int volume;
} CaptureParam;
extern CaptureParam data_0204acf8;

/* khdays: shared-bss */
volatile BOOL data_0204acb0;   /* sIsThreadCreated */

/* NNSi_SndCaptureInit -- NitroSystem capture.c: NNSi_SndCaptureInit. */
void NNSi_SndCaptureInit (void)
{
    data_0204acb0 = FALSE;
    data_0204acf8.activeFlag = FALSE;
}
