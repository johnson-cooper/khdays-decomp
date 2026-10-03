/* The PS2 replacement for the DS ARM7 sound driver.
 *
 * On the DS the ARM9 half of the sound library (NitroSDK SND + NitroSystem SND, both kept as
 * the decomp's C) builds lists of SNDCommand and sends each list's address to the ARM7 over PXI
 * (tag 7).  The ARM7 runs the sequencer, the channel mixer and the envelopes, then advances
 * SNDSharedWork.finishCommandTag, which the ARM9 waits on, and keeps player/channel status in
 * the shared work area for the ARM9 to read.
 *
 * Here PXI_SendWordByFifo(7, list) executes the list immediately on the EE and completes it, so
 * the ARM9 code never waits.  This file is the driver's command front end; the sequencer is
 * snd_seq.c, the channels and the mixer snd_chan.c.  The platform's audio thread pulls mixed
 * stereo through kh_snd_render(), which also runs the driver updates at the DS rate (191.97 Hz),
 * so music and effects are clocked by the audio output.  Commands (game thread) and rendering
 * (audio thread) exclude each other with one semaphore.
 */
#include "snd_internal.h"

#include <string.h>
#include <kernel.h>

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
volatile u8 *g_snd_shared;            /* the same, byte-addressed (snd_seq.c) */
static u32 g_cmd_count[SND_COMMAND_COUNT];

static int g_lock_sema = -1;
void snd_lock(void)
{
    if (g_lock_sema < 0) {
        ee_sema_t sm;
        sm.init_count = 1;
        sm.max_count = 1;
        sm.option = 0;
        sm.attr = 0;
        g_lock_sema = CreateSema(&sm);
    }
    {
        int w = kh_io_begin();
        WaitSema(g_lock_sema);
        kh_io_end(w);
    }
}
void snd_unlock(void) { SignalSema(g_lock_sema); }

extern void snd_direct_timer(SndChannel *c, u32 timer);

/* --- SND alarms (the ARM7's snd_alarm.c) ---
 * SND_SetupAlarm records an alarm's first delay, period (OS ticks) and generation id;
 * SND_StartTimer / SND_StopTimer arm and disarm alarms by bit mask.  On the DS each expiry sends
 * (id << 8 | alarm) back over PXI and the ARM9's PxiFifoCallback runs SNDi_CallAlarmHandler.
 * NitroSystem's stream players are driven entirely by these alarms (one per buffer half: the
 * stream position advances and the next block is requested), so without them a stream never
 * progresses or finishes - the Mission Mode launch waits for a stream to end.  Expiries are
 * delivered at thread level through SNDi_CallAlarmHandler.  On PS2 they must follow the audio
 * render clock, not wall time: the renderer works ahead of the speaker, and a wall-clock alarm
 * can otherwise refill a stream block while the software mixer is still reading it. */
#define SND_ALARM_COUNT 8
#define SND_ALARM_SCALE (32ull * SND_MIX_RATE) /* alarm ticks use the 16.756991 MHz clock / 32 */
#define SND_ALARM_STEP  16756991ull             /* scaled alarm-clock units per output frame */
typedef struct SndAlarm {
    u32 tick, period;
    u8 id, armed;
    u64 fire;
} SndAlarm;
static SndAlarm g_alarm[SND_ALARM_COUNT];
static u64 g_alarm_clock;

extern void SNDi_CallAlarmHandler(int msg);

static void alarms_start(u32 mask)
{
    int i;
    for (i = 0; i < SND_ALARM_COUNT; i++)
        if (mask & (1u << i)) {
            g_alarm[i].armed = 1;
            g_alarm[i].fire = g_alarm_clock + (u64)g_alarm[i].tick * SND_ALARM_SCALE;
        }
}

static void alarms_stop(u32 mask)
{
    int i;
    for (i = 0; i < SND_ALARM_COUNT; i++)
        if (mask & (1u << i))
            g_alarm[i].armed = 0;
}

/* a status line every ~10 s: players and channels in use, output peak */
#if KH_PS2_DEBUG
static void status_line(void)
{
    extern s32 g_snd_peak;
    static u32 n;
    int i, players = 0, chans = 0;
    if (++n % 600)
        return;
    for (i = 0; i < SND_PLAYER_NUM; i++)
        players += g_snd_player[i].active;
    for (i = 0; i < SND_CHANNEL_NUM; i++)
        chans += g_snd_ch[i].active;
    KH_INFO("snd", "players %d channels %d peak %d | seq starts %u pcm setups %u", players, chans, (int)g_snd_peak,
            (unsigned)g_cmd_count[SND_COMMAND_START_SEQ] + (unsigned)g_cmd_count[SND_COMMAND_START_PREPARED_SEQ],
            (unsigned)g_cmd_count[SND_COMMAND_SETUP_CHANNEL_PCM]);
    g_snd_peak = 0;
    for (i = 0; i < SND_CHANNEL_NUM; i++) {
        const SndChannel *c = &g_snd_ch[i];
        if (c->active)
            KH_INFO("snd", "  ch%d %s started %d fmt %d loop %d vol %d>>%d pan %d gain %d/%d step %u pos %u/%u data %p",
                    i, c->direct ? "direct" : "seq", c->started, c->wave.format, c->wave.loop, c->hw_vol, c->hw_shift,
                    c->hw_pan, (int)c->gain_l, (int)c->gain_r, (unsigned)c->step, (unsigned)c->pos, (unsigned)c->len,
                    (const void *)c->data);
    }
}
#endif

void kh_snd_run_alarms(void)
{
#if KH_PS2_DEBUG
    status_line();
#endif
}

/* Advance alarms by samples actually rendered and collect callbacks for delivery after releasing
 * the sound lock.  Some non-stream alarm handlers submit sound commands themselves. */
static int alarms_advance(int frames, int *messages, int capacity)
{
    int i, n, count = 0;
    g_alarm_clock += (u64)frames * SND_ALARM_STEP;
    for (i = 0; i < SND_ALARM_COUNT; i++) {
        for (n = 0; n < 8 && g_alarm[i].armed && g_alarm_clock >= g_alarm[i].fire; n++) {
            SndAlarm *a = &g_alarm[i];
            if (a->period)
                a->fire += (u64)a->period * SND_ALARM_SCALE;
            else
                a->armed = 0;           /* one-shot */
            if (count < capacity)
                messages[count++] = i | (a->id << 8);
        }
        if (g_alarm[i].armed && g_alarm_clock >= g_alarm[i].fire)   /* far behind: resynchronise */
            g_alarm[i].fire = g_alarm_clock + (u64)g_alarm[i].period * SND_ALARM_SCALE;
    }
    return count;
}

static int in_range(const void *p, u32 start, u32 end)
{
    return (u32)(uintptr_t)p >= start && (u32)(uintptr_t)p < end;
}

static void run_command(const SNDCommand *c)
{
    u32 a0 = c->arg[0], a1 = c->arg[1], a2 = c->arg[2], a3 = c->arg[3];
    int i;
    if (c->id < SND_COMMAND_COUNT)
        g_cmd_count[c->id]++;
    switch (c->id) {
    /* ---- sequences */
    case SND_COMMAND_START_SEQ:
        snd_seq_prepare((int)a0, (const u8 *)(uintptr_t)a1, a2, (const SndBank *)(uintptr_t)a3);
        snd_seq_start((int)a0);
        break;
    case SND_COMMAND_PREPARE_SEQ:
        snd_seq_prepare((int)a0, (const u8 *)(uintptr_t)a1, a2, (const SndBank *)(uintptr_t)a3);
        break;
    case SND_COMMAND_START_PREPARED_SEQ: snd_seq_start((int)a0); break;
    case SND_COMMAND_STOP_SEQ:           snd_seq_stop((int)a0); break;
    case SND_COMMAND_PAUSE_SEQ:
        if (a0 < SND_PLAYER_NUM)
            g_snd_player[a0].paused = (u8)(a1 != 0);
        break;
    case SND_COMMAND_SKIP_SEQ:           snd_seq_skip((int)a0, a1); break;
    case SND_COMMAND_PLAYER_PARAM:       /* player, byte offset in the ARM7's SNDPlayer, value, size */
        if (a0 < SND_PLAYER_NUM) {
            SndPlayer *pl = &g_snd_player[a0];
            switch (a1) {
            case 4:    pl->prio = (u8)a2; break;
            case 5:    pl->volume = (u8)a2; break;
            case 6:    pl->ext_fader = (s16)a2; break;
            case 0x18: pl->tempo = (u16)a2; break;
            case 0x1a: pl->tempo_ratio = (u16)a2; break;
            default:   KH_UNIMPLEMENTED_ONCE("snd: player parameter at an unhandled offset"); break;
            }
        }
        break;
    case SND_COMMAND_TRACK_PARAM: {      /* player | size << 24, track mask, offset in SNDTrack, value */
        u32 pno = a0 & 0xffffff;
        if (pno >= SND_PLAYER_NUM)
            break;
        for (i = 0; i < SND_TRACK_NUM; i++) {
            SndTrack *t = &g_snd_player[pno].track[i];
            if (!(a1 & (1u << i)))
                continue;
            switch (a2) {
            case 4:   t->volume = (u8)a3; break;
            case 5:   t->volume2 = (u8)a3; break;
            case 8:   t->pan = (s8)a3; break;
            case 9:   t->ext_pan = (s8)a3; break;
            case 0xa: t->ext_fader = (s16)a3; break;
            case 0xc: t->ext_pitch = (s16)a3; break;
            default:  KH_UNIMPLEMENTED_ONCE("snd: track parameter at an unhandled offset"); break;
            }
        }
        break;
    }
    case SND_COMMAND_MUTE_TRACK:         snd_seq_mute((int)a0, a1, (int)a2); break;
    case SND_COMMAND_ALLOCATABLE_CHANNEL:
        if (a0 < SND_PLAYER_NUM)
            for (i = 0; i < SND_TRACK_NUM; i++)
                if (a1 & (1u << i))
                    g_snd_player[a0].track[i].channel_mask = (u16)a2;
        break;
    case SND_COMMAND_PLAYER_LOCAL_VAR:
        if (a0 < SND_PLAYER_NUM && a1 < 16)
            *snd_var((int)a0, (int)a1) = (s16)a2;
        break;
    case SND_COMMAND_PLAYER_GLOBAL_VAR:
        if (a0 < 16)
            *snd_var(0, 16 + (int)a0) = (s16)a1;
        break;

    /* ---- channels used directly (streams, effects): registers as given */
    case SND_COMMAND_SETUP_CHANNEL_PCM: {
        u32 ch = a0 & 0xffff;
        SndChannel *ch_p;
        if (ch >= SND_CHANNEL_NUM)
            break;
        if (!g_cmd_count[SND_COMMAND_SETUP_CHANNEL_PCM] || g_cmd_count[SND_COMMAND_SETUP_CHANNEL_PCM] == 1)
            KH_INFO("snd", "PCM channels: first direct channel set up (stream)");
        ch_p = &g_snd_ch[ch];
        if (ch_p->active)
            snd_channel_stop(ch_p);
        ch_p->type = CH_PCM;
        ch_p->direct = 1;
        ch_p->active = 1;
        ch_p->started = 0;
        ch_p->hw_vol = (u8)(a2 >> 24);
        ch_p->hw_shift = (u8)((a2 >> 22) & 3);
        ch_p->hw_pan = (u8)((a3 >> 16) & 0x7f);
        snd_hw_setup(ch_p, (const u8 *)(uintptr_t)a1, (int)((a3 >> 24) & 3), (int)((a3 >> 26) & 3),
                     a3 & 0xffff, a2 & 0x3fffff);
        snd_direct_timer(ch_p, a0 >> 16);
        break;
    }
    case SND_COMMAND_SETUP_CHANNEL_PSG:
    case SND_COMMAND_SETUP_CHANNEL_NOISE:
    case SND_COMMAND_SETUP_CAPTURE:
        KH_UNIMPLEMENTED_ONCE("snd: direct PSG / noise channels, capture");
        break;
    case SND_COMMAND_START_TIMER:        /* channels, captures, alarms, flags */
        for (i = 0; i < SND_CHANNEL_NUM; i++)
            if ((a0 & (1u << i)) && g_snd_ch[i].active && !g_snd_ch[i].started) {
                SndChannel *ch_p = &g_snd_ch[i];
                ch_p->started = 1;
                if (ch_p->direct)
                    snd_direct_start(ch_p);
                else {
                    ch_p->pos = 0;
                    ch_p->frac = 0;
                    ch_p->sample = 0;
                }
            }
        alarms_start(a2);
        break;
    case SND_COMMAND_STOP_TIMER:
        for (i = 0; i < SND_CHANNEL_NUM; i++)
            if ((a0 & (1u << i)) && g_snd_ch[i].active)
                snd_channel_stop(&g_snd_ch[i]);
        alarms_stop(a2);
        break;
    case SND_COMMAND_CHANNEL_TIMER:
        for (i = 0; i < SND_CHANNEL_NUM; i++)
            if (a0 & (1u << i))
                snd_direct_timer(&g_snd_ch[i], a1);
        break;
    case SND_COMMAND_CHANNEL_VOLUME:
        for (i = 0; i < SND_CHANNEL_NUM; i++)
            if (a0 & (1u << i)) {
                g_snd_ch[i].hw_vol = (u8)a1;
                g_snd_ch[i].hw_shift = (u8)(a2 & 3);
            }
        break;
    case SND_COMMAND_CHANNEL_PAN:
        for (i = 0; i < SND_CHANNEL_NUM; i++)
            if (a0 & (1u << i))
                g_snd_ch[i].hw_pan = (u8)(a1 & 0x7f);
        break;
    case SND_COMMAND_MASTER_VOLUME: g_snd_master_volume = (u8)(a0 > 127 ? 127 : a0); break;
    case SND_COMMAND_LOCK_CHANNEL:       /* taken away from the sequencer */
        g_snd_locked |= a0 & 0xffff;
        for (i = 0; i < SND_CHANNEL_NUM; i++)
            if ((a0 & (1u << i)) && g_snd_ch[i].active && !g_snd_ch[i].direct)
                snd_channel_stop(&g_snd_ch[i]);
        break;
    case SND_COMMAND_UNLOCK_CHANNEL:     g_snd_locked &= ~a0; break;
    case SND_COMMAND_STOP_UNLOCKED_CHANNEL:
        for (i = 0; i < SND_CHANNEL_NUM; i++)
            if ((a0 & (1u << i)) && !(g_snd_locked & (1u << i)) && g_snd_ch[i].active)
                snd_channel_stop(&g_snd_ch[i]);
        break;

    /* ---- data about to be freed by the ARM9: nothing may keep reading it */
    case SND_COMMAND_INVALIDATE_SEQ:
        for (i = 0; i < SND_PLAYER_NUM; i++)
            if ((g_snd_player[i].active || g_snd_player[i].prepared) &&
                in_range(g_snd_player[i].track[0].base, a0, a1))
                snd_seq_stop(i);
        break;
    case SND_COMMAND_INVALIDATE_BANK:
        for (i = 0; i < SND_PLAYER_NUM; i++)
            if ((g_snd_player[i].active || g_snd_player[i].prepared) && in_range(g_snd_player[i].bank, a0, a1))
                snd_seq_stop(i);
        break;
    case SND_COMMAND_INVALIDATE_WAVE:
        for (i = 0; i < SND_CHANNEL_NUM; i++)
            if (g_snd_ch[i].active && g_snd_ch[i].type == CH_PCM && in_range(g_snd_ch[i].data, a0, a1))
                snd_channel_stop(&g_snd_ch[i]);
        break;

    case SND_COMMAND_SETUP_ALARM:
        if (c->arg[0] < SND_ALARM_COUNT) {
            SndAlarm *a = &g_alarm[c->arg[0]];
            a->tick = c->arg[1];
            a->period = c->arg[2];
            a->id = (u8)c->arg[3];
            a->armed = 0;
        }
        break;
    case SND_COMMAND_SHARED_WORK:
        g_shared = (volatile u32 *)(uintptr_t)c->arg[0];
        g_snd_shared = (volatile u8 *)g_shared;
        KH_INFO("snd", "shared work at %p", (void *)g_shared);
        break;
    default:
        break;
    }
}

/* The audio thread's source: `frames` stereo samples, running the driver updates in between. */
void kh_snd_render(s16 *out, int frames)
{
    static float until_update;
    const float period = (float)SND_MIX_RATE / SND_UPDATE_HZ;
    int done = 0;
    while (done < frames) {
        int n, i, message_count;
        int messages[SND_ALARM_COUNT * 8];
        snd_lock();
        if (until_update <= 0.0f) {
            u32 status = 0;
            snd_seq_update();
            snd_channels_update();
            for (n = 0; n < SND_CHANNEL_NUM; n++)
                if (g_snd_ch[n].active)
                    status |= 1u << n;
            if (g_snd_shared)
                *(volatile u16 *)(g_snd_shared + 8) = (u16)status;
            until_update += period;
        }
        n = (int)until_update + 1;
        if (n > frames - done)
            n = frames - done;
        snd_mix(out + done * 2, n);
        done += n;
        until_update -= (float)n;
        message_count = alarms_advance(n, messages, SND_ALARM_COUNT * 8);
        snd_unlock();
        for (i = 0; i < message_count; i++)
            SNDi_CallAlarmHandler(messages[i]);
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
        snd_lock();
        for (; c; c = c->next)
            run_command(c);
        snd_unlock();
    }
    if (g_shared)
        g_shared[0]++;        /* finishCommandTag: the list is done */
    return 0;
}

/* The ARM9 checks the ARM7 is running before allocating commands (IsCommandAvailable reads a
 * main-memory word the ARM7 sets); the PS2 driver is always there. */
int IsCommandAvailable(void) { return 1; }
