/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndStrmSetup.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/mi.h"
#include "nitro/pxi.h"
#include "nitro/spi.h"
#include "nnsys/fnd.h"

#define offsetof(type, member) ((u32)&(((type *)0)->member))

extern OSIntrMode OS_DisableInterrupts(void);
extern OSIntrMode OS_RestoreInterrupts(OSIntrMode state);
typedef void (*SNDAlarmHandler) (void *);
typedef enum {
    SND_WAVE_FORMAT_PCM8,
    SND_WAVE_FORMAT_PCM16,
    SND_WAVE_FORMAT_ADPCM,
    SND_WAVE_FORMAT_PSG,
    SND_WAVE_FORMAT_NOISE = SND_WAVE_FORMAT_PSG
} SNDWaveFormat;
typedef enum {
    SND_CHANNEL_LOOP_MANUAL,
    SND_CHANNEL_LOOP_REPEAT,
    SND_CHANNEL_LOOP_1SHOT
} SNDChannelLoop;
typedef enum {
    SND_DUTY_1_8,
    SND_DUTY_2_8,
    SND_DUTY_3_8,
    SND_DUTY_4_8,
    SND_DUTY_5_8,
    SND_DUTY_6_8,
    SND_DUTY_7_8
} SNDDuty;
typedef enum {
    SND_CHANNEL_DATASHIFT_NONE,
    SND_CHANNEL_DATASHIFT_1BIT,
    SND_CHANNEL_DATASHIFT_2BIT,
    SND_CHANNEL_DATASHIFT_4BIT
} SNDChannelDataShift;
struct SNDExChannel;
typedef enum SNDExChannelCallbackStatus {
    SND_EX_CHANNEL_CALLBACK_DROP,
    SND_EX_CHANNEL_CALLBACK_FINISH
} SNDExChannelCallbackStatus;
typedef void (*SNDExChannelCallback) (struct SNDExChannel * ch_p, SNDExChannelCallbackStatus status, void * userData);
typedef struct SNDWaveParam {
        u8 format;
        u8 loopflag;
        u16 rate;
        u16 timer;
        u16 loopstart;
        u32 looplen;
    } SNDWaveParam;
typedef struct SNDLfoParam {
    u8 target;
    u8 speed;
    u8 depth;
    u8 range;
    u16 delay;
} SNDLfoParam;
typedef struct SNDLfo {
    struct SNDLfoParam param;
    u16 delay_counter;
    u16 counter;
} SNDLfo;
typedef struct SNDExChannel {
    u8 myNo;
    u8 type;
    u8 env_status;
    u8 active_flag : 1;
    u8 start_flag : 1;
    u8 auto_sweep : 1;
    u8 sync_flag : 5;
    u8 pan_range;
    u8 original_key;
    s16 user_decay2;
    u8 key;
    u8 velocity;
    s8 init_pan;
    s8 user_pan;
    s16 user_decay;
    s16 user_pitch;
    s32 env_decay;
    s32 sweep_counter;
    s32 sweep_length;
    u8 attack;
    u8 sustain;
    u16 decay;
    u16 release;
    u8 prio;
    u8 pan;
    u16 volume;
    u16 timer;
    struct SNDLfo lfo;
    s16 sweep_pitch;
    s32 length;
    struct SNDWaveParam wave;
    union {
        const void * data;
        SNDDuty duty;
    };
    SNDExChannelCallback callback;
    void * callback_data;
    struct SNDExChannel * nextLink;
} SNDExChannel;
void SND_SetupChannelPcm(
    int chNo,
    SNDWaveFormat format,
    const void *dataAddr,
    SNDChannelLoop loop,
    int loopStart,
    int dataLen,
    int volume,
    SNDChannelDataShift shift,
    int timer,
    int pan
);
void SND_SetupAlarm(
    int alarmNo,
    u32 tick,
    u32 period,
    SNDAlarmHandler handler,
    void *arg
);
struct SNDExChannel;
void NNS_FndAppendListObject(NNSFndList * list, void * object);
typedef enum NNSSndStrmFormat {
    NNS_SND_STRM_FORMAT_PCM8,
    NNS_SND_STRM_FORMAT_PCM16
} NNSSndStrmFormat;
typedef enum NNSSndStrmCallbackStatus {
    NNS_SND_STRM_CALLBACK_SETUP,
    NNS_SND_STRM_CALLBACK_INTERVAL
} NNSSndStrmCallbackStatus;
typedef void (*NNSSndStrmCallback)(NNSSndStrmCallbackStatus status, int numChannles, void * buffer[], u32 len, NNSSndStrmFormat format, void * arg);
typedef struct NNSSndStrm {
    NNSFndLink link;
    PMSleepCallbackInfo preSleepInfo;
    PMSleepCallbackInfo postSleepInfo;
    NNSSndStrmFormat format;
    BOOL activeFlag :1;
    BOOL startFlag :1;
    u32 chBufLen;
    int interval;
    NNSSndStrmCallback callback;
    void * callbackArg;
    int curBuffer;
    int volume;
    int alarmNo;
    u32 chBitMask;
    int numChannels;
    u8 channelNo[16 ];
} NNSSndStrm;
void NNS_SndStrmStop(NNSSndStrm * stream);
int NNS_SndAllocAlarm(void);
typedef struct NNSSndStrmChannel {
    void * buffer;
    int volume;
} NNSSndStrmChannel;
extern NNSSndStrmChannel data_0204ac30[ 16 ];
extern NNSFndList data_0204abe4;
extern void AlarmCallback(void * arg);
extern void StrmCallback(NNSSndStrm * stream, NNSSndStrmCallbackStatus status);
extern void NNS_SndStrmStop (NNSSndStrm * stream);
extern void AlarmCallback (void * arg);
extern void StrmCallback (NNSSndStrm * stream, NNSSndStrmCallbackStatus status);

/* khdays: shared-bss */
BOOL data_0204abe0;   /* bInitialized$2853 */

/* NNS_SndStrmSetup -- NitroSystem stream.c: NNS_SndStrmSetup. */
BOOL NNS_SndStrmSetup (NNSSndStrm * stream, NNSSndStrmFormat format, void * buffer, u32 bufSize, int timer, int interval, NNSSndStrmCallback callback, void * arg)
{
    NNSSndStrmChannel * chp;
    unsigned int samples;
    unsigned int alarmTimer;
    int chNo;
    int index;

    if (stream->activeFlag) {
        NNS_SndStrmStop(stream);
    }

    bufSize /= 32 * interval * stream->numChannels;
    stream->chBufLen = bufSize * interval * 32;

    samples = stream->chBufLen;
    if (format == NNS_SND_STRM_FORMAT_PCM16) samples >>= 1;

    alarmTimer = timer * samples / interval;

    stream->alarmNo = NNS_SndAllocAlarm();
    if (stream->alarmNo < 0) return FALSE;

    for (index = 0; index < stream->numChannels; index++) {
        chNo = stream->channelNo[ index ];

        chp = &data_0204ac30[ chNo ];

        chp->buffer = (u8 *)buffer + stream->chBufLen * index;

        chp->volume = 0;

        SND_SetupChannelPcm(
            chNo,
            (SNDWaveFormat)format,
            chp->buffer,
            SND_CHANNEL_LOOP_REPEAT,
            0,
            (int)(stream->chBufLen >> 2),
            127,
            SND_CHANNEL_DATASHIFT_NONE,
            timer << 5,
                64
            );
    }

    SND_SetupAlarm(
        stream->alarmNo,
        alarmTimer,
        alarmTimer,
        AlarmCallback,
        stream
        );

    NNS_FndAppendListObject(&data_0204abe4, stream);

    stream->format = format;
    stream->interval = interval;
    stream->callback = callback;
    stream->callbackArg = arg;
    stream->curBuffer = 0;

    stream->volume = 0;

    stream->activeFlag = TRUE;

    {
        OSIntrMode old = OS_DisableInterrupts();

        stream->interval = 1;
        StrmCallback(stream, NNS_SND_STRM_CALLBACK_SETUP);
        stream->interval = interval;

        (void)OS_RestoreInterrupts(old);
    }

    return TRUE;
}
