/* PS2: mechanically prepared copy of libs/nns/snd/calls/PrepareStrm.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

#define BLOCK_NUM 4

BOOL NNS_SndStrmSetup(NNSSndStrm * stream, NNSSndStrmFormat format, void * buffer, u32 bufSize, int timer, int interval, NNSSndStrmCallback callback, void * arg);
void NNS_SndStrmSetChannelPan(NNSSndStrm * stream, int chNo, int pan);
void NNSi_SndFaderInit(NNSSndFader * fader);
void NNSi_SndFaderSet(NNSSndFader * fader, int target, int frame);
typedef enum StrmFormat {
    STRM_FORMAT_PCM8,
    STRM_FORMAT_PCM16,
    STRM_FORMAT_ADPCM
} StrmFormat;
extern NNSSndStrmPlayer * AllocPlayer(NNSSndStrmHandle * handle, int playerNo, int playerPrio);
extern void FreePlayer(NNSSndStrmPlayer * player);
extern BOOL AllocChannel(NNSSndStrmPlayer * player, int numChannels, const u8 chNoList[]);
extern void FreeChannel(NNSSndStrmPlayer * player);
extern void StrmCallback_2(NNSSndStrmCallbackStatus status, int numChannels, void * buffer[], u32 len, NNSSndStrmFormat format, void * arg);
extern void SetupStreamFunction(NNSSndStrmPlayer * player, u32 fileId);
extern NNSSndStrmPlayer * AllocPlayer (NNSSndStrmHandle * handle, int playerNo, int prio);
extern void FreePlayer (NNSSndStrmPlayer * player);
extern BOOL AllocChannel (NNSSndStrmPlayer * player, int numChannels, const u8 chNoList[]);
extern void FreeChannel (NNSSndStrmPlayer * player);
extern void StrmCallback_2 (NNSSndStrmCallbackStatus status, int numChannels, void * buffer[], u32 len, NNSSndStrmFormat, void * arg);
extern void SetupStreamFunction (NNSSndStrmPlayer * player, u32 fileId);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* PrepareStrm -- NitroSystem sndarc_stream.c: PrepareStrm. */
BOOL PrepareStrm (struct NNSSndStrmHandle * handle, const NNSSndArcStrmInfo * strmInfo, int playerNo, int playerPrio, int strmNo, u32 offset, NNSSndStrmCallback strmCallback, void * strmCallbackArg, NNSSndArcStrmCallback sndArcStrmCallback, void * sndArcStrmCallbackArg)
{
    NNSSndStrmPlayer * player;
    NNSSndStrmFormat format;
    int numChannels;
    u64 tmpU64;
    BOOL ret;

    player = AllocPlayer(handle, playerNo, playerPrio);
    if (player == NULL) return FALSE;

    SetupStreamFunction(player, strmInfo->fileId);

    if (!player->openStreamFunc(player, strmInfo->fileId)) {
        FreePlayer(player);
        return FALSE;
    }

    tmpU64 = player->info.sampleRate;
    tmpU64 *= offset;
    tmpU64 /= 1000;
    player->curSample = (u32)tmpU64;

    if (player->curSample != 0 && player->info.format == STRM_FORMAT_ADPCM) {
        player->dirtyFlag = TRUE;
    } else {
        player->dirtyFlag = FALSE;
    }

    player->finishCounter = BLOCK_NUM;
    player->finishFlag = FALSE;
    player->playFlag = FALSE;
    player->prepareFlag = FALSE;
    player->startFlag = FALSE;
    player->fadeOutFlag = FALSE;
    player->commandCount = 0;

    player->strmCallback = strmCallback;
    player->strmCallbackArg = strmCallbackArg;
    player->sndArcStrmCallback = sndArcStrmCallback;
    player->sndArcStrmCallbackArg = sndArcStrmCallbackArg;

    player->strmNo = strmNo;

    player->volume = 0;
    player->initVolume = strmInfo->volume;
    player->extVolume = 127;

    NNSi_SndFaderInit(&player->fader);
    NNSi_SndFaderSet(&player->fader, 127 << 8, 1);

    switch (player->info.format) {
    case STRM_FORMAT_PCM8:
        format = NNS_SND_STRM_FORMAT_PCM8;
        break;
    case STRM_FORMAT_PCM16:
    case STRM_FORMAT_ADPCM:
        format = NNS_SND_STRM_FORMAT_PCM16;
        break;
    }

    numChannels = player->info.numChannels;
    if (strmInfo->flags & NNS_SND_ARC_STRM_FORCE_STEREO) {
        numChannels = 2;
    }
    if (numChannels > player->numChannels) {
        numChannels = player->numChannels;
    }
    player->monoFlag = (numChannels == 1) ? TRUE : FALSE;

    ret = AllocChannel(player, numChannels, player->chNoList);
    if (!ret) {
        player->closeStreamFunc(player);
        FreePlayer(player);
        return FALSE;
    }

    ret = NNS_SndStrmSetup(
        &player->stream,
        format,
        player->buffer,
        player->bufSize * numChannels / player->numChannels,
        player->info.timer,
        BLOCK_NUM,
        StrmCallback_2,
        player
        );
    if (!ret) {
        FreeChannel(player);
        player->closeStreamFunc(player);
        FreePlayer(player);
        return FALSE;
    }

    if (numChannels == 2) {
        NNS_SndStrmSetChannelPan(&player->stream, 0, 0);
        NNS_SndStrmSetChannelPan(&player->stream, 1, 127);
    }

    return TRUE;
}
