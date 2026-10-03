/* Audio output: an EE thread that pulls 48 kHz 16-bit stereo from a render callback (the sound
 * driver, ps2/src/audio) one SPU2 half at a time, converts it to the SPU2 block-input layout
 * (256 left, 256 right, 256 left, 256 right) and pushes it to khsnd.irx over one SIF
 * RPC.  The push returns only when the IOP's ring has room, so the thread is paced by the SPU2's
 * output clock and sleeps in between (the RPC wait blocks it).  It runs above the main game
 * thread but below the music refill worker, which can preempt it at a refill alarm.
 *
 * PS2BUILD's audsrv.irx is a voice-only build (no PCM streaming), hence the module of our own.
 */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <delaythread.h>
#include <kernel.h>
#include <sifrpc.h>
#include <string.h>

#define AUDIO_RATE    48000
#define SPU_UNIT_FRAMES 256                     /* fixed layout unit: 256 L then 256 R */
#define SPU_HALF_FRAMES (SPU_UNIT_FRAMES * 2)   /* one 2048-byte libsd transfer half */
#define AUDIO_BATCH_HALVES 1                    /* 10.7 ms lets streamed-music refills run in time */
#define AUDIO_FRAMES  (SPU_HALF_FRAMES * AUDIO_BATCH_HALVES)
#define AUDIO_PRIO    27                        /* stream worker 26; main game thread 32 */
#define KHSND_RPC_ID  0x4b48534e
#define KHSND_PUSH    1

static int g_audio_ok;
static SifRpcClientData_t g_rpc;
static void (*g_render)(int16_t *stereo, int frames);
static int16_t g_mix[AUDIO_FRAMES * 2] __attribute__((aligned(64)));
static int16_t g_block[AUDIO_FRAMES * 2] __attribute__((aligned(64)));
static u32 g_reply[6] __attribute__((aligned(64)));
static u8 g_stack[16 * 1024] __attribute__((aligned(16)));
volatile u32 kh_audio_chunks;                   /* (diagnostics) */
volatile u32 kh_audio_underruns;
volatile u32 kh_audio_queue_depth;
volatile u32 kh_audio_irq_callbacks;
volatile u32 kh_audio_fills;
volatile u32 kh_audio_callback_misses;
volatile u32 kh_audio_callback_late;
volatile u32 kh_audio_rpc_errors;

int kh_audio_init(void)
{
    int tries;
    if (ps2_iop_load_audio() < 0)
        return -1;
    for (tries = 0; tries < 1000; tries++) {
        if (SifBindRpc(&g_rpc, KHSND_RPC_ID, 0) < 0)
            break;
        if (g_rpc.server)
            break;
        nopdelay();
    }
    if (!g_rpc.server) {
        KH_ERR("audio", "khsnd.irx RPC server not found: running silent");
        return -1;
    }
    g_audio_ok = 1;
    KH_INFO("audio", "khsnd ready: %d Hz stereo to SPU2 core 1", AUDIO_RATE);
    return 0;
}

static void audio_thread(void *arg)
{
    (void)arg;
    for (;;) {
        int i;
        kh_prof_begin(KH_PROF_AUDIO);
        if (g_render)
            g_render(g_mix, AUDIO_FRAMES);
        else
            memset(g_mix, 0, sizeof g_mix);
        /* Each 2048-byte libsd half contains two 256-frame units.  Keep every unit planar:
         * [256 L][256 R], repeated across both halves in this RPC.  A [512 L][512 R]
         * half swaps channels midway through playback. */
        for (i = 0; i < AUDIO_FRAMES; i++) {
            int unit = i / SPU_UNIT_FRAMES;
            int frame = i % SPU_UNIT_FRAMES;
            int base = unit * SPU_UNIT_FRAMES * 2;
            g_block[base + frame] = g_mix[i * 2];
            g_block[base + SPU_UNIT_FRAMES + frame] = g_mix[i * 2 + 1];
        }
        kh_prof_end(KH_PROF_AUDIO);
        for (;;) {
            int result;
            int w = kh_io_begin();     /* (marks the wait for the hang watchdog) */
            result = SifCallRpc(&g_rpc, KHSND_PUSH, 0, g_block, sizeof g_block,
                                g_reply, sizeof g_reply, NULL, NULL);
            kh_io_end(w);
            if (result >= 0)
                break;
            /* A transient SIF error must not discard time from the audio stream.  Retry this
             * already-rendered half, yielding so a persistent failure cannot busy-spin. */
            kh_audio_rpc_errors++;
            DelayThread(1000);
        }
        kh_audio_underruns = g_reply[0];
        kh_audio_queue_depth = g_reply[1];
        kh_audio_irq_callbacks = g_reply[2];
        kh_audio_fills = g_reply[3];
        kh_audio_callback_misses = g_reply[4];
        kh_audio_callback_late = g_reply[5];
        kh_audio_chunks++;
    }
}

void kh_audio_start(void (*render)(int16_t *stereo, int frames))
{
    ee_thread_t th;
    int id;
    g_render = render;
    if (!g_audio_ok)
        return;
    memset(&th, 0, sizeof th);
    th.func = (void *)audio_thread;
    th.stack = g_stack;
    th.stack_size = sizeof g_stack;
    th.gp_reg = &_gp;
    th.initial_priority = AUDIO_PRIO;
    id = CreateThread(&th);
    if (id < 0 || StartThread(id, NULL) < 0) {
        KH_ERR("audio", "audio thread failed (%d)", id);
        return;
    }
    KH_INFO("audio", "audio thread started");
}

void kh_audio_update(void)
{
    (void)g_audio_ok;
}
