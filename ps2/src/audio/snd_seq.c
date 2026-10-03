/* Sequence players: the DS ARM7 driver's SSEQ interpreter, on the EE.
 *
 * A player runs up to 16 tracks over one sequence (SSEQ) with one instrument bank (SBNK, whose
 * wave archives (SWAR) the ARM9 has loaded and linked).  Each driver update the player's tempo
 * counter advances by tempo x tempo_ratio / 256; every 240 counts is one tick, in which each
 * track counts down its wait and runs commands until the next wait.  Notes allocate channels
 * (snd_chan.c) whose length counts down in ticks; every update the tracks' volume, pan and pitch
 * are pushed to their channels.  Semantics follow NitroSDK's snd_seq.c / snd_bank.c (command set,
 * prefixes for random / variable / conditional arguments, tie and mono modes, portamento,
 * modulation, envelope overrides, variables shared with the ARM9 through SNDSharedWork).
 */
#include "snd_internal.h"

#include <string.h>

SndPlayer g_snd_player[SND_PLAYER_NUM];
static s16 g_local_vars[SND_PLAYER_NUM][16], g_global_vars[16];

/* variables live in SNDSharedWork when the ARM9 has given it (it reads them there) */
s16 *snd_var(int player, int idx)
{
    if (idx < 16) {
        if (g_snd_shared)
            return (s16 *)(g_snd_shared + 0x20 + player * 0x24 + idx * 2);
        return &g_local_vars[player][idx];
    }
    if (g_snd_shared)
        return (s16 *)(g_snd_shared + 0x260 + (idx - 16) * 2);
    return &g_global_vars[idx - 16];
}

/* ------------------------------------------------------------ bank */

static int read_inst(const SndBank *bank, int prg, int key, SndInst *out)
{
    u32 off;
    const u8 *p;
    int type;
    if (!bank || prg < 0 || (u32)prg >= bank->count)
        return 0;
    off = bank->offset[prg];
    type = off & 0xff;
    p = (const u8 *)bank + (off >> 8);
    switch (type) {
    case INST_PCM: case INST_PSG: case INST_NOISE: case INST_DIRECTPCM: case INST_NULL:
        out->type = (u8)type;
        memcpy(&out->p, p, sizeof out->p);
        return 1;
    case INST_DRUM:
        if (key < p[0] || key > p[1])
            return 0;
        p += 2 + (key - p[0]) * 12;
        out->type = p[0];
        memcpy(&out->p, p + 2, sizeof out->p);
        return 1;
    case INST_SPLIT: {
        int i;
        for (i = 0; i < 8 && p[i]; i++)
            if (key <= p[i])
                break;
        if (i == 8 || !p[i])
            return 0;
        p += 8 + i * 12;
        out->type = p[0];
        memcpy(&out->p, p + 2, sizeof out->p);
        return 1;
    }
    default:
        return 0;
    }
}

/* SWAR offsets are file-relative on the DS; NitroSystem stores absolute addresses for waves it
 * loads one by one.  (The DS tells them apart by 0x02000000; EE addresses are below that, so the
 * archive's size does it here.) */
static const SndWaveParam *wave_data(const SndBank *bank, int arc_no, int wave_no)
{
    const SndWaveArc *arc;
    u32 off;
    if (!bank || arc_no < 0 || arc_no >= 4)
        return NULL;
    arc = bank->link[arc_no].arc;
    if (!arc || (u32)wave_no >= arc->count)
        return NULL;
    off = arc->offset[wave_no];
    if (!off)
        return NULL;
    return (const SndWaveParam *)(off < arc->fh.file_size ? (const u8 *)arc + off : (const u8 *)(uintptr_t)off);
}

/* ------------------------------------------------------------ tracks */

static void track_init(SndTrack *t, const u8 *base, const u8 *cur)
{
    memset(t, 0, sizeof *t);
    t->active = 1;
    t->base = base;
    t->cur = cur;
    t->note_wait = 1;
    t->pan_range = 127;
    t->volume = 127;
    t->volume2 = 127;
    t->bend_range = 2;
    t->prio = 64;
    t->attack = t->decay = t->sustain = t->release = 0xff;
    t->porta_key = 60;
    t->channel_mask = 0xffff;
    t->mod.speed = 16;
    t->mod.range = 1;
}

static void track_release_channels(SndTrack *t, int release)
{
    SndChannel *c;
    for (c = t->channels; c; c = c->next_in_track)
        snd_channel_release(c, release);
}

/* release the track's channels and let them finish on their own (tie changes, track end) */
static void track_detach_channels(SndTrack *t)
{
    while (t->channels) {
        SndChannel *c = t->channels;
        snd_channel_release(c, -1);
        t->channels = c->next_in_track;
        c->next_in_track = NULL;
        c->track = NULL;
    }
}

static void track_free_channels(SndTrack *t)
{
    while (t->channels)
        snd_channel_stop(t->channels);        /* unlinks it */
}

void snd_seq_channel_freed(SndChannel *c)
{
    SndTrack *t = c->track;
    SndChannel **pp;
    if (!t)
        return;
    for (pp = &t->channels; *pp; pp = &(*pp)->next_in_track)
        if (*pp == c) {
            *pp = c->next_in_track;
            break;
        }
    c->next_in_track = NULL;
    c->track = NULL;
}

static u32 read_u8(SndTrack *t) { return *t->cur++; }
static u32 read_u16(SndTrack *t) { u32 v = t->cur[0] | t->cur[1] << 8; t->cur += 2; return v; }
static u32 read_u24(SndTrack *t) { u32 v = t->cur[0] | t->cur[1] << 8 | t->cur[2] << 16; t->cur += 3; return v; }
static u32 read_var(SndTrack *t)
{
    u32 v = 0;
    u8 b;
    do {
        b = *t->cur++;
        v = (v << 7) | (b & 0x7f);
    } while (b & 0x80);
    return v;
}

/* argument reader honouring the random / variable prefixes (kind: 0 none, 1 random, 2 variable) */
enum { ARG_U8, ARG_S16, ARG_VAR, ARG_RAND };
static u32 g_rng = 0x12345678u;
static s32 read_arg(SndTrack *t, int player, int type, int prefix)
{
    if (prefix == 1) {
        s32 lo = (s16)read_u16(t), hi = (s16)read_u16(t);
        g_rng = g_rng * 1664525u + 1013904223u;
        return lo + (s32)((g_rng >> 16) % (u32)(hi - lo + 1 > 0 ? hi - lo + 1 : 1));
    }
    if (prefix == 2)
        return *snd_var(player, (int)read_u8(t));
    switch (type) {
    case ARG_U8:  return (s32)read_u8(t);
    case ARG_S16: return (s16)read_u16(t);
    default:      return (s32)read_var(t);
    }
}

static void note_on(SndPlayer *pl, int pno, SndTrack *t, int key, int velocity, s32 length)
{
    SndInst inst;
    SndChannel *c = NULL;
    int type;
    if (!read_inst(pl->bank, t->prg, key, &inst))
        return;
    switch (inst.type) {
    case INST_PCM: case INST_DIRECTPCM: type = CH_PCM; break;
    case INST_PSG:   type = CH_PSG; break;
    case INST_NOISE: type = CH_NOISE; break;
    default: return;
    }
    if (t->tie && t->channels) {               /* tie: the note continues on the same channel */
        c = t->channels;
        c->key = (u8)key;
        c->velocity = (u8)velocity;
    } else {
        int prio = t->prio + pl->prio;
        if (prio > 255) prio = 255;
        c = snd_alloc_channel(type, t->channel_mask, prio, t);
        if (!c)
            return;
        if (type == CH_PCM) {
            const SndWaveParam *w;
            if (inst.type == INST_DIRECTPCM)
                w = (const SndWaveParam *)(uintptr_t)(inst.p.wave[0] | (u32)inst.p.wave[1] << 16);
            else
                w = wave_data(pl->bank, inst.p.wave[1], inst.p.wave[0]);
            if (!w) {
                c->prio = 0;
                return;
            }
            c->wave = *w;
            snd_hw_setup(c, (const u8 *)(w + 1), w->format, w->loop, w->loop_start, w->loop_len);
        } else {
            c->duty = (u8)inst.p.wave[0];
            c->wave.format = FMT_PSG;
        }
        c->key = (u8)key;
        c->original_key = inst.p.key;
        c->velocity = (u8)velocity;
        c->init_pan = (s8)(inst.p.pan - 64);
        c->attack = (u8)snd_calc_attack(t->attack != 0xff ? t->attack : inst.p.attack);
        c->decay = (u16)snd_calc_release(t->decay != 0xff ? t->decay : inst.p.decay);
        c->sustain = t->sustain != 0xff ? t->sustain : inst.p.sustain;
        c->release = (u16)snd_calc_release(t->release != 0xff ? t->release : inst.p.release);
        c->next_in_track = t->channels;
        t->channels = c;
        c->track = t;
        snd_channel_start(c);
    }
    c->length = (length == 0 || t->tie) ? -1 : length;      /* untimed: until tie end / sample end */
    c->lfo.target = t->mod.target;
    c->lfo.speed = t->mod.speed;
    c->lfo.depth = t->mod.depth;
    c->lfo.range = t->mod.range;
    c->lfo.delay = t->mod.delay;
    /* sweep: the track's sweep pitch plus portamento from the previous key */
    c->sweep_pitch = t->sweep_pitch;
    c->sweep_counter = 0;
    c->auto_sweep = 0;
    c->sweep_length = length > 0 ? length : 0;
    if (t->porta) {
        c->sweep_pitch += (s16)((t->porta_key - key) << 6);
        if (t->porta_time) {
            s32 tt = t->porta_time * t->porta_time;
            s32 sp = c->sweep_pitch < 0 ? -c->sweep_pitch : c->sweep_pitch;
            c->sweep_length = (sp * tt) >> 11;
            c->auto_sweep = 1;
        }
    }
    (void)pno;
}

/* one tick of one track; returns 0 when the track has ended */
static int track_step(SndPlayer *pl, int pno, SndTrack *t, int play)
{
    SndChannel *c;
    for (c = t->channels; c; c = c->next_in_track) {
        if (c->length > 0)
            c->length--;
        if (!c->auto_sweep && c->sweep_counter < c->sweep_length)
            c->sweep_counter++;
    }
    for (c = t->channels; c; c = c->next_in_track)
        if (c->length == 0 && c->env_status != ENV_RELEASE)
            snd_channel_release(c, -1);
    if (t->note_finish_wait) {
        if (t->channels)
            return 1;
        t->note_finish_wait = 0;
    }
    if (t->wait > 0) {
        if (--t->wait > 0)
            return 1;
    }
    while (t->wait == 0 && !t->note_finish_wait) {
        int prefix = 0, run = 1;
        u32 cmd = read_u8(t);
        if (cmd == 0xa2) {                     /* if: run the next command only when cmp is set */
            run = t->cmp;
            cmd = read_u8(t);
        }
        if (cmd == 0xa0) { prefix = 1; cmd = read_u8(t); }
        else if (cmd == 0xa1) { prefix = 2; cmd = read_u8(t); }

        if (cmd < 0x80) {                      /* note: velocity, length */
            int vel = (int)read_u8(t);
            s32 len = read_arg(t, pno, ARG_VAR, prefix);
            int key = (int)cmd + t->transpose;
            if (!run)
                continue;
            if (key < 0) key = 0;
            if (key > 127) key = 127;
            if (!t->mute && play)
                note_on(pl, pno, t, key, vel, len);
            t->porta_key = (u8)key;
            if (t->note_wait) {
                t->wait = len;
                if (len == 0)
                    t->note_finish_wait = 1;
            }
            continue;
        }
        switch (cmd) {
        case 0x80: { s32 v = read_arg(t, pno, ARG_VAR, prefix); if (run) t->wait = v; break; }
        case 0x81: { s32 v = read_arg(t, pno, ARG_VAR, prefix); if (run && v <= 0xffff) t->prg = (u16)v; break; }
        case 0x93: {                            /* open track: number, offset */
            u32 tr = read_u8(t), off = read_u24(t);
            if (run && tr < SND_TRACK_NUM && !(pl->track_open & (1u << tr))) {
                track_init(&pl->track[tr], t->base, t->base + off);
                pl->track_open |= 1u << tr;
            }
            break;
        }
        case 0x94: { u32 off = read_u24(t); if (run) t->cur = t->base + off; break; }
        case 0x95: {
            u32 off = read_u24(t);
            if (run && t->depth < 3) {
                t->call_stack[t->depth] = t->cur;
                t->loop_count[t->depth] = 0;
                t->depth++;
                t->cur = t->base + off;
            }
            break;
        }
        /* variables: number, value */
        case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6:
        case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: {
            int vn = (int)read_u8(t);
            s32 v = read_arg(t, pno, ARG_S16, prefix);
            s16 *var = snd_var(pno, vn);
            if (!run)
                break;
            switch (cmd) {
            case 0xb0: *var = (s16)v; break;
            case 0xb1: *var = (s16)(*var + v); break;
            case 0xb2: *var = (s16)(*var - v); break;
            case 0xb3: *var = (s16)(*var * v); break;
            case 0xb4: if (v) *var = (s16)(*var / v); break;
            case 0xb5: *var = (s16)(v >= 0 ? *var << v : *var >> -v); break;
            case 0xb6: g_rng = g_rng * 1664525u + 1013904223u;
                       *var = (s16)(v < 0 ? -(s32)((g_rng >> 16) % (u32)(-v + 1)) : (s32)((g_rng >> 16) % (u32)(v + 1)));
                       break;
            case 0xb8: t->cmp = *var == v; break;
            case 0xb9: t->cmp = *var >= v; break;
            case 0xba: t->cmp = *var > v; break;
            case 0xbb: t->cmp = *var <= v; break;
            case 0xbc: t->cmp = *var < v; break;
            case 0xbd: t->cmp = *var != v; break;
            }
            break;
        }
        /* one-byte parameters */
        case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
        case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
        case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: {
            s32 v = read_arg(t, pno, ARG_U8, prefix);
            if (!run)
                break;
            switch (cmd) {
            case 0xc0: t->pan = (s8)(v - 64); break;
            case 0xc1: t->volume = (u8)v; break;
            case 0xc2: pl->volume = (u8)v; break;
            case 0xc3: t->transpose = (s8)v; break;
            case 0xc4: t->pitch_bend = (s8)v; break;
            case 0xc5: t->bend_range = (u8)v; break;
            case 0xc6: t->prio = (u8)v; break;
            case 0xc7: t->note_wait = (u8)(v & 1); break;
            case 0xc8: t->tie = (u8)(v & 1); track_detach_channels(t); break;
            case 0xc9: t->porta_key = (u8)(v + t->transpose); t->porta = 1; break;
            case 0xca: t->mod.depth = (u8)v; break;
            case 0xcb: t->mod.speed = (u8)v; break;
            case 0xcc: t->mod.target = (u8)v; break;
            case 0xcd: t->mod.range = (u8)v; break;
            case 0xce: t->porta = (u8)(v & 1); break;
            case 0xcf: t->porta_time = (u8)v; break;
            case 0xd0: t->attack = (u8)v; break;
            case 0xd1: t->decay = (u8)v; break;
            case 0xd2: t->sustain = (u8)v; break;
            case 0xd3: t->release = (u8)v; break;
            case 0xd4:                          /* loop start: count */
                if (t->depth < 3) {
                    t->call_stack[t->depth] = t->cur;
                    t->loop_count[t->depth] = (u8)v;
                    t->depth++;
                }
                break;
            case 0xd5: t->volume2 = (u8)v; break;
            case 0xd6: break;                   /* print variable (debug) */
            }
            break;
        }
        case 0xe0: { s32 v = read_arg(t, pno, ARG_S16, prefix); if (run) t->mod.delay = (u16)v; break; }
        case 0xe1: { s32 v = read_arg(t, pno, ARG_S16, prefix); if (run) pl->tempo = (u16)v; break; }
        case 0xe3: { s32 v = read_arg(t, pno, ARG_S16, prefix); if (run) t->sweep_pitch = (s16)v; break; }
        case 0xfc:                              /* loop end */
            if (run && t->depth) {
                u8 *n = &t->loop_count[t->depth - 1];
                if (*n == 1) {
                    t->depth--;                 /* last pass */
                } else {
                    if (*n)
                        (*n)--;
                    t->cur = t->call_stack[t->depth - 1];
                }
            }
            break;
        case 0xfd:                              /* return */
            if (run && t->depth) {
                t->depth--;
                t->cur = t->call_stack[t->depth];
            }
            break;
        case 0xfe: read_u16(t); break;          /* allocate tracks (opened by 0x93) */
        case 0xff:                              /* end of track */
            if (run) {
                t->active = 0;
                return 0;
            }
            break;
        default:
            KH_WARN("snd", "unknown sequence command %02x: track stopped", (unsigned)cmd);
            t->active = 0;
            return 0;
        }
    }
    return 1;
}

/* the track's volume / pan / pitch onto its channels */
static void track_update_channels(SndPlayer *pl, SndTrack *t)
{
    SndChannel *c;
    s32 vol = snd_db_square(t->volume) + snd_db_square(t->volume2) + snd_db_square(pl->volume) +
              t->ext_fader + pl->ext_fader;
    s32 pitch = ((s32)t->pitch_bend * t->bend_range * 64) / 128 + t->ext_pitch;
    s32 pan = t->pan + t->ext_pan;
    if (vol < -32768) vol = -32768;
    if (pan < -128) pan = -128;
    if (pan > 127) pan = 127;
    for (c = t->channels; c; c = c->next_in_track) {
        c->user_decay2 = (s16)vol;
        c->user_pitch = (s16)pitch;
        c->user_pan = (s8)pan;
        c->pan_range = t->pan_range;
        c->lfo.target = t->mod.target;
        c->lfo.speed = t->mod.speed;
        c->lfo.depth = t->mod.depth;
        c->lfo.range = t->mod.range;
        c->lfo.delay = t->mod.delay;
    }
}

static void player_finish(int pno)
{
    SndPlayer *pl = &g_snd_player[pno];
    int i;
    for (i = 0; i < SND_TRACK_NUM; i++) {
        SndTrack *t = &pl->track[i];
        if (pl->track_open & (1u << i)) {
            track_detach_channels(t);           /* they keep releasing, unowned */
        }
    }
    pl->active = 0;
    pl->prepared = 0;
    pl->track_open = 0;
}

/* one tick of a player; returns 0 when every track has ended */
static int player_step(int pno, int play)
{
    SndPlayer *pl = &g_snd_player[pno];
    int i, alive = 0;
    for (i = 0; i < SND_TRACK_NUM; i++) {
        SndTrack *t = &pl->track[i];
        if (!(pl->track_open & (1u << i)))
            continue;
        if (t->active && !track_step(pl, pno, t, play))
            track_release_channels(t, -1);
        if (t->active || t->channels)
            alive = 1;
    }
    return alive;
}

void snd_seq_prepare(int pno, const u8 *base, u32 offset, const SndBank *bank)
{
    SndPlayer *pl;
    if (pno < 0 || pno >= SND_PLAYER_NUM)
        return;
    pl = &g_snd_player[pno];
    if (pl->active)
        player_finish(pno);
    memset(pl->track, 0, sizeof pl->track);
    pl->bank = bank;
    pl->tempo = 120;
    pl->tempo_ratio = 256;
    pl->tempo_counter = 240;          /* the first tick runs at once */
    pl->volume = 127;
    pl->ext_fader = 0;
    pl->prio = 64;
    pl->paused = 0;
    pl->ticks = 0;
    track_init(&pl->track[0], base, base + offset);
    pl->track_open = 1;
    pl->prepared = 1;
}

void snd_seq_start(int pno)
{
    if (pno >= 0 && pno < SND_PLAYER_NUM && g_snd_player[pno].prepared)
        g_snd_player[pno].active = 1;
}

void snd_seq_stop(int pno)
{
    if (pno < 0 || pno >= SND_PLAYER_NUM)
        return;
    if (g_snd_player[pno].active || g_snd_player[pno].prepared) {
        int i;
        for (i = 0; i < SND_TRACK_NUM; i++)
            if (g_snd_player[pno].track_open & (1u << i))
                track_free_channels(&g_snd_player[pno].track[i]);
        player_finish(pno);
    }
}

void snd_seq_skip(int pno, u32 ticks)
{
    if (pno < 0 || pno >= SND_PLAYER_NUM || !g_snd_player[pno].active)
        return;
    while (ticks--)
        if (!player_step(pno, 0)) {
            player_finish(pno);
            break;
        }
}

void snd_seq_mute(int pno, u32 mask, int flag)
{
    int i;
    if (pno < 0 || pno >= SND_PLAYER_NUM)
        return;
    for (i = 0; i < SND_TRACK_NUM; i++) {
        SndTrack *t = &g_snd_player[pno].track[i];
        if (!(mask & (1u << i)) || !(g_snd_player[pno].track_open & (1u << i)))
            continue;
        t->mute = flag != 0;
        if (flag == 2)
            track_release_channels(t, -1);
        else if (flag == 3)
            track_free_channels(t);
    }
}

void snd_seq_update(void)
{
    int p, i;
    u32 status = 0;
    for (p = 0; p < SND_PLAYER_NUM; p++) {
        SndPlayer *pl = &g_snd_player[p];
        if (!pl->active)
            continue;
        if (!pl->paused) {
            while (pl->tempo_counter >= 240) {
                pl->tempo_counter -= 240;
                if (!player_step(p, 1)) {
                    player_finish(p);
                    break;
                }
                pl->ticks++;
            }
            if (!pl->active)
                continue;
            pl->tempo_counter = (u16)(pl->tempo_counter + ((u32)pl->tempo * pl->tempo_ratio >> 8));
        }
        for (i = 0; i < SND_TRACK_NUM; i++)
            if (pl->track_open & (1u << i))
                track_update_channels(pl, &pl->track[i]);
        status |= 1u << p;
        if (g_snd_shared)
            *(volatile u32 *)(g_snd_shared + 0x20 + p * 0x24 + 0x20) = pl->ticks;
    }
    if (g_snd_shared)
        *(volatile u32 *)(g_snd_shared + 4) = status;
}
