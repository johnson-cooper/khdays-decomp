/* khsnd.irx - PCM output for the KH Days PS2 port (IOP side).
 *
 * The EE mixes at 48 kHz and currently sends one 512-frame block in each bulk RPC.
 * Each block contains two SPU2 input units: 256 left, 256 right, 256 left, 256 right.  SPU2 core 1
 * plays its sound-data input from a two-half loop (libsd SD_TRANS_LOOP block transfer); each time a
 * half has played, the transfer interrupt wakes the play thread to refill it from an 8-block
 * ring.  The EE push RPC waits while the ring is full, so the EE is paced by the SPU2 clock
 * without polling.
 *
 * (PS2BUILD's audsrv.irx is a voice-only build without PCM streaming: hence this module.)
 */
#include <irx.h>
#include <loadcore.h>
#include <thbase.h>
#include <thsemap.h>
#include <intrman.h>
#include <sifcmd.h>
#include <sifrpc.h>
#include <libsd.h>
#include <sysclib.h>
#include <stdio.h>

IRX_ID("khsnd", 1, 0);

#define SD_CORE_1 1                   /* sceSdSetParam: the core is the entry's low bit */

#define KHSND_RPC_ID   0x4b48534e     /* 'KHSN' */
#define KHSND_PUSH     1
#define BLOCK_BYTES    2048           /* 512 frames: two [256 L][256 R] units */
#define RPC_BATCH_BLOCKS 2            /* maximum blocks accepted in one RPC */
#define RPC_BYTES      (BLOCK_BYTES * RPC_BATCH_BLOCKS)
#define RING_BLOCKS    8

static u8 spu_buf[2 * BLOCK_BYTES] __attribute__((aligned(64)));
static u8 ring[RING_BLOCKS][BLOCK_BYTES] __attribute__((aligned(16)));
static volatile int ring_head, ring_tail;
static volatile u32 underruns, callbacks, irq_callbacks, callback_misses, callback_late;
static int sema_free, sema_transfer;

static SifRpcDataQueue_t rpc_queue;
static SifRpcServerData_t rpc_server;
static u8 rpc_buf[RPC_BYTES] __attribute__((aligned(64)));

static int transfer_done(void *arg)
{
    (void)arg;
    irq_callbacks++;
    if (iSignalSema(sema_transfer) < 0)
        callback_misses++;
    return 1;
}

static void play_thread(void *arg)
{
    u32 last_irq_seen = 0;
    (void)arg;
    for (;;) {
        int half, intr_state, consumed = 0;
        u32 irq_delta;
        WaitSema(sema_transfer);
        /* Match audsrv: make status + refill atomic with respect to the next DMA interrupt. */
        CpuSuspendIntr(&intr_state);
        /* A max-1 semaphore alone cannot reveal two callbacks that arrive before this thread
         * runs when the first callback woke a waiter.  Count IRQ deltas to catch that deadline
         * miss as well as explicit semaphore overflow, without double-counting later wakes. */
        irq_delta = irq_callbacks - last_irq_seen;
        if (irq_delta > 1)
            callback_late += irq_delta - 1;
        last_irq_seen = irq_callbacks;
        half = 1 - (int)(sceSdBlockTransStatus(1, 0) >> 24);
        if (ring_tail != ring_head) {
            memcpy(spu_buf + half * BLOCK_BYTES, ring[ring_tail], BLOCK_BYTES);
            ring_tail = (ring_tail + 1) % RING_BLOCKS;
            consumed = 1;
        } else {
            memset(spu_buf + half * BLOCK_BYTES, 0, BLOCK_BYTES);
            underruns++;
        }
        callbacks++;
        CpuResumeIntr(intr_state);
        if (consumed)
            SignalSema(sema_free);
    }
}

static void *rpc_handler(int fno, void *data, int size)
{
    if (fno == KHSND_PUSH && size >= BLOCK_BYTES) {
        const u8 *src = (const u8 *)data;
        u32 *reply = (u32 *)data;
        int blocks = size / BLOCK_BYTES;
        int block, depth;
        if (blocks > RPC_BATCH_BLOCKS)
            blocks = RPC_BATCH_BLOCKS;
        for (block = 0; block < blocks; block++) {
            /* When the ring fills, this RPC sleeps while the independent play thread
             * continues moving one half at a time to SPU2. */
            WaitSema(sema_free);
            memcpy(ring[ring_head], src + block * BLOCK_BYTES, BLOCK_BYTES);
            ring_head = (ring_head + 1) % RING_BLOCKS;
        }
        depth = ring_head - ring_tail;
        if (depth < 0)
            depth += RING_BLOCKS;
        /* All audio bytes are safely queued, so the RPC input buffer can become its reply. */
        reply[0] = underruns;
        reply[1] = (u32)depth;
        reply[2] = irq_callbacks;
        reply[3] = callbacks;
        reply[4] = callback_misses;
        reply[5] = callback_late;
    }
    return data;
}

static void rpc_thread(void *arg)
{
    (void)arg;
    sceSifSetRpcQueue(&rpc_queue, GetThreadId());
    sceSifRegisterRpc(&rpc_server, KHSND_RPC_ID, rpc_handler, rpc_buf, NULL, NULL, &rpc_queue);
    sceSifRpcLoop(&rpc_queue);
}

static int start_thread(void (*fn)(void *), int prio)
{
    iop_thread_t t;
    int id;
    memset(&t, 0, sizeof t);
    t.attr = TH_C;
    t.thread = fn;
    t.priority = prio;
    t.stacksize = 0x800;
    id = CreateThread(&t);
    if (id > 0)
        StartThread(id, NULL);
    return id;
}

int _start(int argc, char **argv)
{
    iop_sema_t s;
    (void)argc;
    (void)argv;
    memset(&s, 0, sizeof s);
    s.initial = RING_BLOCKS - 1;
    s.max = RING_BLOCKS - 1;
    sema_free = CreateSema(&s);
    s.initial = 0;
    s.max = 1;
    sema_transfer = CreateSema(&s);

    sceSdInit(0);
    sceSdSetParam(SD_CORE_1 | SD_PARAM_MVOLL, 0x3fff);
    sceSdSetParam(SD_CORE_1 | SD_PARAM_MVOLR, 0x3fff);
    sceSdSetParam(SD_CORE_1 | SD_PARAM_BVOLL, 0x3fff);
    sceSdSetParam(SD_CORE_1 | SD_PARAM_BVOLR, 0x3fff);
    memset(spu_buf, 0, sizeof spu_buf);
    sceSdSetTransCallback(1, transfer_done);
    sceSdBlockTrans(1, SD_TRANS_LOOP, spu_buf, sizeof spu_buf, 0);

    start_thread(play_thread, 20);
    start_thread(rpc_thread, 40);
#if defined(KH_PS2_DEBUG) && KH_PS2_DEBUG
    printf("khsnd: PCM output ready\n");
#endif
    return MODULE_RESIDENT_END;
}
