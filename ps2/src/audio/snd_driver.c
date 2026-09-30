/* The PS2 replacement for the DS ARM7 sound driver.
 *
 * On the DS the ARM9 half of the sound library (NitroSDK SND + NitroSystem SND, both kept as
 * the decomp's C) builds lists of SNDCommand and sends each list's address to the ARM7 over PXI
 * (tag 7).  The ARM7 runs the sequencer, the channel mixer and the envelopes, then advances
 * SNDSharedWork.finishCommandTag, which the ARM9 waits on, and keeps player/channel status in
 * the shared work area for the ARM9 to read.
 *
 * Here PXI_SendWordByFifo(7, list) executes the list immediately on the EE and completes it, so
 * the ARM9 code never waits.  This file is the driver's command front end; the sequencer
 * (SSEQ), wave playback and the SPU2 voice back end are built on these handlers
 * (docs/PS2_PORT.md 3.14).  Until then commands are accepted and logged, sequences report as
 * not playing, and the game runs silent.
 */
#include "platform/kh_platform.h"

#include <string.h>
#include <tamtypes.h>

typedef struct SNDCommand {
    struct SNDCommand *next;
    u32 id;
    u32 arg[4];
} SNDCommand;

enum {
    SND_COMMAND_START_SEQ, SND_COMMAND_STOP_SEQ, SND_COMMAND_PREPARE_SEQ, SND_COMMAND_START_PREPARED_SEQ,
    SND_COMMAND_PAUSE_SEQ, SND_COMMAND_SKIP_SEQ, SND_COMMAND_PLAYER_PARAM, SND_COMMAND_TRACK_PARAM,
    SND_COMMAND_MUTE_TRACK, SND_COMMAND_ALLOCATABLE_CHANNEL, SND_COMMAND_PLAYER_LOCAL_VAR,
    SND_COMMAND_PLAYER_GLOBAL_VAR, SND_COMMAND_START_TIMER, SND_COMMAND_STOP_TIMER,
    SND_COMMAND_SETUP_CHANNEL_PCM, SND_COMMAND_SETUP_CHANNEL_PSG, SND_COMMAND_SETUP_CHANNEL_NOISE,
    SND_COMMAND_SETUP_CAPTURE, SND_COMMAND_SETUP_ALARM, SND_COMMAND_CHANNEL_TIMER,
    SND_COMMAND_CHANNEL_VOLUME, SND_COMMAND_CHANNEL_PAN, SND_COMMAND_SURROUND_DECAY,
    SND_COMMAND_MASTER_VOLUME, SND_COMMAND_MASTER_PAN, SND_COMMAND_OUTPUT_SELECTOR,
    SND_COMMAND_LOCK_CHANNEL, SND_COMMAND_UNLOCK_CHANNEL, SND_COMMAND_STOP_UNLOCKED_CHANNEL,
    SND_COMMAND_SHARED_WORK, SND_COMMAND_INVALIDATE_SEQ, SND_COMMAND_INVALIDATE_BANK,
    SND_COMMAND_INVALIDATE_WAVE, SND_COMMAND_READ_DRIVER_INFO, SND_COMMAND_COUNT
};

#define PXI_FIFO_TAG_SOUND 7

static volatile u32 *g_shared;        /* SNDSharedWork; word 0 = finishCommandTag */
static u32 g_cmd_count[SND_COMMAND_COUNT];

static void run_command(const SNDCommand *c)
{
    if (c->id < SND_COMMAND_COUNT)
        g_cmd_count[c->id]++;
    switch (c->id) {
    case SND_COMMAND_SHARED_WORK:
        g_shared = (volatile u32 *)(uintptr_t)c->arg[0];
        KH_INFO("snd", "shared work at %p", (void *)g_shared);
        break;
    case SND_COMMAND_START_SEQ:
    case SND_COMMAND_PREPARE_SEQ:
        KH_DBG("snd", "seq %s: player %u, data %p", c->id == SND_COMMAND_START_SEQ ? "start" : "prepare",
               (unsigned)c->arg[0], (void *)(uintptr_t)c->arg[1]);
        KH_UNIMPLEMENTED_ONCE("snd: sequence playback (running silent)");
        break;
    case SND_COMMAND_SETUP_CHANNEL_PCM:
        KH_UNIMPLEMENTED_ONCE("snd: PCM channels (running silent)");
        break;
    default:
        break;
    }
}

int PXI_SendWordByFifo(int tag, u32 data, int err)
{
    (void)err;
    if (tag != PXI_FIFO_TAG_SOUND) {
        KH_UNIMPLEMENTED_ONCE("PXI to the ARM7 for a non-sound service");
        return 0;
    }
    if (data == 0)            /* "process your queue": everything was processed on arrival */
        return 0;
    {
        const SNDCommand *c = (const SNDCommand *)(uintptr_t)data;
        for (; c; c = c->next)
            run_command(c);
    }
    if (g_shared)
        g_shared[0]++;        /* finishCommandTag: the list is done */
    return 0;
}

/* The ARM9 checks the ARM7 is running before allocating commands (IsCommandAvailable reads a
 * main-memory word the ARM7 sets); the PS2 driver is always there. */
int IsCommandAvailable(void) { return 1; }
