/* PS2 override: never park the game thread behind a physical stream refill.
 *
 * NitroSystem's original stream main loop force-stops a player as soon as a fade or end-of-file
 * completes.  ForceStopStrm_2 takes the stream-worker mutex and is allowed to wait because a DS
 * card refill is short.  A real PS2 USB/MMCE read can be much longer; taking that mutex from the
 * scene thread then freezes the whole transition (notably New Game after the difficulty prompt).
 *
 * There are two parts to preserving the DS-visible behaviour without that wait:
 *   1. once a stream is logically finished, invalidate its public handle immediately so callers
 *      waiting on NNS_SndArcStrmGetCurrentPlayingPos observe "stopped" on the same frame;
 *   2. retry the physical teardown with try-locks until the refill worker releases its mutex.
 *
 * A faded/finished player is already silent by the time its handle is detached.  Explicit scene
 * transition stops use kh_ps2_snd_stop_slot_nonblocking below: a real fade is armed through the
 * original non-blocking fade path, while an immediate/fallback stop only detaches the handle and
 * opportunistically tears the player down.  No title/menu frame is allowed to block on storage.
 */
#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nnsys/snd.h"

extern const s16 data_02041488[128];
extern NNSSndStrmPlayer data_0204b62c[4];
extern NNSSndStrmThread data_0204b140;
extern NNSSndStrmThread *sPrepareThread;
extern char *gSoundMgr;

extern int OS_TryLockMutex(OSMutex *mutex);
extern void OS_UnlockMutex(OSMutex *mutex);
extern void NNS_SndStrmStop(NNSSndStrm *stream);
extern void OSi_DestroyThread(NNSSndStrmPlayer *player);
extern void NNS_SndStrmStart(NNSSndStrm *stream);
extern void NNS_SndStrmSetVolume(NNSSndStrm *stream, int volume);
extern int NNSi_SndFaderGet(const NNSSndFader *fader);
extern void NNSi_SndFaderUpdate(NNSSndFader *fader);
extern BOOL NNSi_SndFaderIsFinished(const NNSSndFader *fader);
extern void SNDi_FreeVoiceChannel(NNSSndStrmPlayer *player, int fadeFrame);

static s16 calc_decibel(int scale)
{
    return data_02041488[scale];
}

static void detach_handle(NNSSndStrmPlayer *player)
{
    if (player->handle != NULL) {
        player->handle->player = NULL;
        player->handle = NULL;
    }
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

/* Scene-transition stop that is safe to call from the game thread on slow physical media.
 *
 * For a live player and a non-zero fade, SNDi_FreeVoiceChannel only arms the fader; its blocking
 * branch is entered when the player is not playing or fadeFrame == 0, both of which we avoid.
 * Immediate/fallback stops make the handle invalid first, then try cleanup without sleeping.
 */
void kh_ps2_snd_stop_slot_nonblocking(int slot, int fadeFrame)
{
    NNSSndStrmHandle *handle;
    NNSSndStrmPlayer *player;

    if (gSoundMgr == NULL || slot < 0 || slot >= NNS_SND_STRM_PLAYER_NUM)
        return;

    handle = (NNSSndStrmHandle *)(gSoundMgr + 0xb44bc + slot * 4);
    player = handle->player;
    if (player == NULL)
        return;

    if (fadeFrame > 0 && player->playFlag) {
        SNDi_FreeVoiceChannel(player, fadeFrame);
        return;
    }

    detach_handle(player);
    (void)try_force_stop(player);
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
            detach_handle(player);
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

        if (player->fadeOutFlag && NNSi_SndFaderIsFinished(&player->fader)) {
            detach_handle(player);
            (void)try_force_stop(player);
        }
    }
}
