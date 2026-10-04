/* PS2 override: never park the game thread behind a physical stream refill.
 *
 * NitroSystem's original stream main loop force-stops a player as soon as a fade or end-of-file
 * completes.  ForceStopStrm_2 takes the stream-worker mutex and is allowed to wait because a DS
 * card refill is short.  A real PS2 USB/MMCE read can be much longer; taking that mutex from the
 * scene thread then freezes the whole transition (notably New Game after the difficulty prompt).
 *
 * A faded/finished player is already silent.  Try the teardown without waiting; if the worker is
 * still filling one buffer, leave the handle valid and retry next frame.  Immediate explicit
 * stops still use the original blocking ForceStopStrm_2 path, preserving its synchronous API.
 */
#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nnsys/snd.h"

extern const s16 data_02041488[128];
extern NNSSndStrmPlayer data_0204b62c[4];
extern NNSSndStrmThread data_0204b140;
extern NNSSndStrmThread *sPrepareThread;

extern int OS_TryLockMutex(OSMutex *mutex);
extern void OS_UnlockMutex(OSMutex *mutex);
extern void NNS_SndStrmStop(NNSSndStrm *stream);
extern void OSi_DestroyThread(NNSSndStrmPlayer *player);
extern void NNS_SndStrmStart(NNSSndStrm *stream);
extern void NNS_SndStrmSetVolume(NNSSndStrm *stream, int volume);
extern int NNSi_SndFaderGet(const NNSSndFader *fader);
extern void NNSi_SndFaderUpdate(NNSSndFader *fader);
extern BOOL NNSi_SndFaderIsFinished(const NNSSndFader *fader);

static s16 calc_decibel(int scale)
{
    return data_02041488[scale];
}

static BOOL try_force_stop(NNSSndStrmPlayer *player)
{
    NNSSndStrmThread *prepare = sPrepareThread;

    if (!OS_TryLockMutex(&data_0204b140.mutex))
        return FALSE;
    if (prepare && !OS_TryLockMutex(&prepare->mutex)) {
        OS_UnlockMutex(&data_0204b140.mutex);
        return FALSE;
    }

    if (player->playFlag)
        NNS_SndStrmStop(&player->stream);
    if (player->activeFlag)
        player->cancelStreamFunc(player);
    OSi_DestroyThread(player);

    if (prepare)
        OS_UnlockMutex(&prepare->mutex);
    OS_UnlockMutex(&data_0204b140.mutex);
    return TRUE;
}

void NNSi_SndArcStrmMain(void)
{
    int playerNo;

    for (playerNo = 0; playerNo < NNS_SND_STRM_PLAYER_NUM; ++playerNo) {
        NNSSndStrmPlayer *player = &data_0204b62c[playerNo];
        int volume;

        if (!player->activeFlag)
            continue;

        if (player->finishCounter == 0) {
            (void)try_force_stop(player);
            continue;
        }

        if (player->startFlag && player->prepareFlag) {
            NNS_SndStrmStart(&player->stream);
            player->playFlag = TRUE;
            player->startFlag = FALSE;
        }

        if (!player->playFlag)
            continue;

        NNSi_SndFaderUpdate(&player->fader);
        volume = calc_decibel(NNSi_SndFaderGet(&player->fader) >> 8)
               + calc_decibel(player->initVolume)
               + calc_decibel(player->extVolume);
        if (volume != player->volume) {
            NNS_SndStrmSetVolume(&player->stream, volume);
            player->volume = volume;
        }

        if (player->fadeOutFlag && NNSi_SndFaderIsFinished(&player->fader))
            (void)try_force_stop(player);
    }
}
