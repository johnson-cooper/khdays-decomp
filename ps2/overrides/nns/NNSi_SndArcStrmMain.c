/* PS2 override: keep physical stream teardown off the game/scene thread.
 *
 * DS NitroSystem can synchronously stop a streamed player because card refills and the ARM7
 * command path are short.  On real PS2 storage, ForceStopStrm_2 can wait behind a refill mutex,
 * cancel/close a file, and wait for sound-command processing.  Any of those waits on the scene
 * thread can leave New Game visibly frozen on the difficulty confirmation screen.
 *
 * The PS2 path therefore separates logical stop from physical teardown:
 *   1. invalidate the public handle immediately so scene code observes the stream as stopped;
 *   2. queue ForceStopStrm_2 to a low-priority Nitro/EE cleanup thread, where blocking is harmless
 *      to rendering and input;
 *   3. while cleanup is pending, NNSi_SndArcStrmMain leaves that player alone.
 *
 * Fade requests remain on the original fader path.  SNDi_FreeVoiceChannel is only called when
 * playFlag is true and fadeFrame is non-zero; in that branch it only arms the fader.  The eventual
 * physical stop is queued after the fade finishes.
 */
#include "nitro/types.h"
#include "nitro/os.h"
#include "nnsys/snd.h"

extern const s16 data_02041488[128];
extern NNSSndStrmPlayer data_0204b62c[4];
extern char *gSoundMgr;

extern void OS_InitMessageQueue(OSMessageQueue *mq, OSMessage *array, s32 count);
extern int OS_SendMessage(OSMessageQueue *mq, OSMessage msg, s32 flags);
extern int OS_ReceiveMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags);
extern void OS_CreateThread(OSThread *thread, void (*func)(void *), void *arg,
                            void *stack, u32 stackSize, u32 prio);
extern void OS_WakeupThreadDirect(OSThread *thread);
extern void ForceStopStrm_2(NNSSndStrmPlayer *player);
extern void NNS_SndStrmStart(NNSSndStrm *stream);
extern void NNS_SndStrmSetVolume(NNSSndStrm *stream, int volume);
extern int NNSi_SndFaderGet(const NNSSndFader *fader);
extern void NNSi_SndFaderUpdate(NNSSndFader *fader);
extern BOOL NNSi_SndFaderIsFinished(const NNSSndFader *fader);
extern void SNDi_FreeVoiceChannel(NNSSndStrmPlayer *player, int fadeFrame);

static OSThread s_cleanup_thread;
static OSMessageQueue s_cleanup_queue;
static OSMessage s_cleanup_messages[NNS_SND_STRM_PLAYER_NUM];
static volatile u8 s_cleanup_pending[NNS_SND_STRM_PLAYER_NUM];
static BOOL s_cleanup_started;

static s16 calc_decibel(int scale)
{
    return data_02041488[scale];
}

static int player_index(NNSSndStrmPlayer *player)
{
    int index = (int)(player - data_0204b62c);
    if (index < 0 || index >= NNS_SND_STRM_PLAYER_NUM)
        return -1;
    return index;
}

static void detach_handle(NNSSndStrmPlayer *player)
{
    if (player->handle != NULL) {
        player->handle->player = NULL;
        player->handle = NULL;
    }
}

static void cleanup_thread_main(void *arg)
{
    OSMessage message;
    (void)arg;

    for (;;) {
        NNSSndStrmPlayer *player;
        int index;

        if (!OS_ReceiveMessage(&s_cleanup_queue, &message, OS_MESSAGE_BLOCK))
            continue;

        player = (NNSSndStrmPlayer *)message;
        index = player_index(player);

        /*
         * This is intentionally the original blocking teardown.  It is now running on a
         * dedicated low-priority OS thread rather than the title/game thread, so a slow USB/MMCE
         * refill, FS cancel/close, or sound-command wait cannot freeze the frame loop.
         */
        ForceStopStrm_2(player);

        if (index >= 0)
            s_cleanup_pending[index] = 0;
    }
}

static void ensure_cleanup_thread(void)
{
    if (s_cleanup_started)
        return;

    OS_InitMessageQueue(&s_cleanup_queue, s_cleanup_messages,
                        NNS_SND_STRM_PLAYER_NUM);
    OS_CreateThread(&s_cleanup_thread, cleanup_thread_main, NULL, NULL, 0x800, 31);
    s_cleanup_started = TRUE;
    OS_WakeupThreadDirect(&s_cleanup_thread);
}

static void queue_cleanup(NNSSndStrmPlayer *player)
{
    int index = player_index(player);

    if (index < 0)
        return;

    ensure_cleanup_thread();

    if (s_cleanup_pending[index])
        return;

    s_cleanup_pending[index] = 1;
    if (!OS_SendMessage(&s_cleanup_queue, (OSMessage)player, OS_MESSAGE_NOBLOCK)) {
        /*
         * There are only four stream players and four queue entries, so this should only be
         * reachable if state is already corrupt.  Clear the pending bit rather than permanently
         * suppressing the player's normal main-loop handling.
         */
        s_cleanup_pending[index] = 0;
    }
}

/* Scene-transition stop that never performs physical teardown on the caller. */
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

    /*
     * Verified against SNDi_FreeVoiceChannel: with playFlag set and a non-zero fade it only
     * configures the fader and returns.  Its blocking ForceStopStrm_2 branches are avoided here.
     */
    if (fadeFrame > 0 && player->playFlag) {
        SNDi_FreeVoiceChannel(player, fadeFrame);
        return;
    }

    detach_handle(player);
    queue_cleanup(player);
}

void NNSi_SndArcStrmMain(void)
{
    int playerNo;

    ensure_cleanup_thread();

    for (playerNo = 0; playerNo < NNS_SND_STRM_PLAYER_NUM; ++playerNo) {
        NNSSndStrmPlayer *player = &data_0204b62c[playerNo];
        int volume;

        if (s_cleanup_pending[playerNo])
            continue;
        if (!player->activeFlag)
            continue;

        if (player->finishCounter == 0) {
            detach_handle(player);
            queue_cleanup(player);
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
            queue_cleanup(player);
        }
    }
}
