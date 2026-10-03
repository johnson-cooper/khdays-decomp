/* The 16 DS sound channels: the driver's channel layer (the SDK's SNDExChannel: allocation by
 * priority, ADSR envelopes, LFO, pitch sweep, the dB/pitch -> register conversion) and the
 * channel hardware itself (PCM8 / PCM16 / IMA-ADPCM sample playback with the DS loop rules, PSG
 * square waves, the noise LFSR), mixed in software to stereo at SND_MIX_RATE.
 *
 * As on the DS hardware, samples are not interpolated (a channel holds its current sample until
 * the next timer tick), the ADPCM loop restarts from the decoder state saved when the loop start
 * was first reached, and a channel's left/right gains are its volume times (128 - pan) / pan.
 */
#include "snd_internal.h"

#include <math.h>
#include <string.h>

SndChannel g_snd_ch[SND_CHANNEL_NUM];
u32 g_snd_locked;
u8 g_snd_master_volume = 127;
s32 g_snd_peak;                         /* output peak since the last status line */

static s16 g_dbsq[128];                 /* SNDi_DecibelSquareTable */
static u16 g_gain[-SND_DB_MIN + 1];     /* -dB (1/10) -> amplitude, 32768 = 1.0 */
static s8 g_sin[128];
static int g_tables;

static void tables(void)
{
    int i;
    for (i = 0; i < 128; i++) {
        double db = i ? 400.0 * log10((double)i / 127.0) : -723.0;
        g_dbsq[i] = (s16)(db < -723.0 ? -723 : (int)floor(db + 0.5));
        g_sin[i] = (s8)floor(127.0 * sin(2.0 * 3.14159265358979 * i / 128.0) + 0.5);
    }
    for (i = 0; i <= -SND_DB_MIN; i++)
        g_gain[i] = i == -SND_DB_MIN ? 0 : (u16)(32768.0 * pow(10.0, -(double)i / 200.0) > 32767.0 ? 32767
                                                 : 32768.0 * pow(10.0, -(double)i / 200.0));
    g_tables = 1;
}

s16 snd_db_square(int v)
{
    if (!g_tables)
        tables();
    if (v < 0) v = 0;
    if (v > 127) v = 127;
    return g_dbsq[v];
}

static s32 db_gain(int db)
{
    if (db <= SND_DB_MIN)
        return 0;
    if (db >= 0)
        return 32767;
    return g_gain[-db];
}

int snd_calc_attack(int a)
{
    static const u8 t[19] = { 0, 1, 5, 14, 26, 38, 51, 63, 73, 84, 92, 100, 109, 116, 123, 127, 132, 137, 143 };
    a &= 127;
    return a < 109 ? 255 - a : t[127 - a];
}

int snd_calc_release(int v)
{
    v &= 127;
    if (v == 127) return 0xffff;
    if (v == 126) return 0x3c00;
    if (v < 50) return v * 2 + 1;
    return 0x1e00 / (126 - v);
}

/* ------------------------------------------------------------ hardware */

static s32 mix_step(u32 timer, int pitch)
{
    float rate;
    if (!timer)
        return 0;
    rate = (float)SND_TIMER_CLOCK / (float)timer;
    if (pitch)
        rate *= powf(2.0f, (float)pitch / 768.0f);
    return (s32)(rate * 65536.0f / (float)SND_MIX_RATE);
}

static void direct_gains(SndChannel *c)
{
    static const u8 k_div[4] = { 0, 1, 2, 4 };
    s32 amp = ((s32)c->hw_vol * 32768 / 127) >> k_div[c->hw_shift & 3];
    c->gain_l = amp * (128 - c->hw_pan) / 128;
    c->gain_r = amp * c->hw_pan / 128;
}

static const u8 k_adpcm_idx[8] = { 0, 0, 0, 0, 2, 4, 6, 8 };
static const u16 k_adpcm_step[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97,
    107, 118, 130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
    876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871,
    5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385,
    24623, 27086, 29794, 32767 };

static inline void adpcm_decode_one(SndChannel *c)
{
    u32 n = c->adpcm_next;
    int nib = (c->data[4 + (n >> 1)] >> ((n & 1) * 4)) & 15;
    int step = k_adpcm_step[c->adpcm_idx];
    int diff = step >> 3;
    if (nib & 1) diff += step >> 2;
    if (nib & 2) diff += step >> 1;
    if (nib & 4) diff += step;
    if (nib & 8) {
        c->adpcm_pred -= diff;
        if (c->adpcm_pred < -0x7fff) c->adpcm_pred = -0x7fff;
    } else {
        c->adpcm_pred += diff;
        if (c->adpcm_pred > 0x7fff) c->adpcm_pred = 0x7fff;
    }
    c->adpcm_idx += (nib & 7) < 4 ? -1 : k_adpcm_idx[nib & 7];
    if (c->adpcm_idx < 0) c->adpcm_idx = 0;
    if (c->adpcm_idx > 88) c->adpcm_idx = 88;
    c->adpcm_next = n + 1;
}

/* the sample at c->pos; returns 0 when a one-shot sound has ended */
static int fetch(SndChannel *c)
{
    if (c->type == CH_PSG) {
        c->sample = (int)(c->pos & 7) <= c->duty ? 0x7fff : -0x7fff;
        return 1;
    }
    if (c->type == CH_NOISE) {
        int carry = c->lfsr & 1;
        c->lfsr >>= 1;
        if (carry)
            c->lfsr ^= 0x6000;
        c->sample = carry ? -0x7fff : 0x7fff;
        return 1;
    }
    if (c->pos >= c->len) {
        if (c->wave.loop != 1 || c->len <= c->loop_start)
            return 0;
        c->pos = c->loop_start + (c->pos - c->len) % (c->len - c->loop_start);
        if (c->wave.format == FMT_ADPCM) {
            if (c->loop_saved) {
                c->adpcm_pred = c->loop_pred;
                c->adpcm_idx = c->loop_idx;
                c->adpcm_next = c->loop_start;
            }
        }
    }
    switch (c->wave.format) {
    case FMT_PCM8:  c->sample = (s32)(s8)c->data[c->pos] * 256; break;
    case FMT_PCM16: c->sample = ((const s16 *)c->data)[c->pos]; break;
    default:
        if (c->adpcm_next > c->pos + 1) {     /* moved backwards (should not happen): restart */
            c->adpcm_pred = (s16)(c->data[0] | c->data[1] << 8);
            c->adpcm_idx = c->data[2] > 88 ? 88 : c->data[2];
            c->adpcm_next = 0;
        }
        while (c->adpcm_next <= c->pos) {
            if (c->adpcm_next == c->loop_start && !c->loop_saved) {
                c->loop_pred = c->adpcm_pred;
                c->loop_idx = c->adpcm_idx;
                c->loop_saved = 1;
            }
            adpcm_decode_one(c);
        }
        c->sample = c->adpcm_pred;
        break;
    }
    return 1;
}

/* Wave data at `data` (after its SNDWaveParam), loop start / length in words (4 bytes) */
void snd_hw_setup(SndChannel *c, const u8 *data, int fmt, int loop, u32 loop_start, u32 loop_len)
{
    u32 ls = loop_start * 4, ll = loop_len * 4;
    c->data = data;
    c->wave.format = (u8)fmt;
    c->wave.loop = (u8)loop;
    c->pos = 0;
    c->frac = 0;
    c->loop_saved = 0;
    c->lfsr = 0x7fff;
    switch (fmt) {
    case FMT_PCM8:  c->loop_start = ls;     c->len = ls + ll; break;
    case FMT_PCM16: c->loop_start = ls / 2; c->len = (ls + ll) / 2; break;
    case FMT_ADPCM:
        c->loop_start = ls >= 4 ? (ls - 4) * 2 : 0;
        c->len = ls + ll >= 4 ? (ls + ll - 4) * 2 : 0;
        c->adpcm_pred = (s16)(data[0] | data[1] << 8);
        c->adpcm_idx = data[2] > 88 ? 88 : data[2];
        c->adpcm_next = 0;
        break;
    default:        c->loop_start = 0;      c->len = 0; break;
    }
}

void snd_mix(s16 *out, int frames)
{
    static s32 acc[2 * 512];
    int i, f;
    if (frames > 512)
        frames = 512;
    memset(acc, 0, sizeof(s32) * 2 * (size_t)frames);
    for (i = 0; i < SND_CHANNEL_NUM; i++) {
        SndChannel *c = &g_snd_ch[i];
        s32 gl = c->gain_l, gr = c->gain_r, smp;
        u32 step = c->step;
        if (!c->active || !c->started || (!c->data && c->type == CH_PCM))
            continue;
        if (c->direct) {
            direct_gains(c);
            gl = c->gain_l;
            gr = c->gain_r;
        }
        smp = c->sample;
        if (c->direct && c->type == CH_PCM && c->timer) {
            /* Stream refills are alarmed from the exact DS sound clock.  Advance direct PCM in
             * that same clock domain: rounding its rate down to Q16 lets an alarm occasionally
             * overwrite the last samples of a ring block before the mixer has consumed them. */
            const u32 period = (u32)c->timer * SND_MIX_RATE;
            for (f = 0; f < frames; f++) {
                u32 phase;
                acc[f * 2] += (smp * gl) >> 15;
                acc[f * 2 + 1] += (smp * gr) >> 15;
                phase = c->frac + SND_TIMER_CLOCK;
                if (phase >= period) {
                    u32 advance = 1;
                    phase -= period;
                    /* Archive streams normally advance at most once per output sample, so their
                     * hot path needs no divide.  Retain the general case for very small timers. */
                    if (phase >= period) {
                        advance += phase / period;
                        phase %= period;
                    }
                    c->frac = phase;
                    c->pos += advance;
                    if (!fetch(c)) {
                        snd_channel_stop(c);
                        break;
                    }
                    smp = c->sample;
                } else {
                    c->frac = phase;
                }
            }
        } else {
            for (f = 0; f < frames; f++) {
                acc[f * 2] += (smp * gl) >> 15;
                acc[f * 2 + 1] += (smp * gr) >> 15;
                c->frac += step;
                if (c->frac >= 0x10000) {
                    c->pos += c->frac >> 16;
                    c->frac &= 0xffff;
                    if (!fetch(c)) {
                        snd_channel_stop(c);
                        break;
                    }
                    smp = c->sample;
                }
            }
        }
    }
    for (f = 0; f < frames * 2; f++) {
        s32 v = (acc[f] * g_snd_master_volume) / 127;
        if (v < 0 ? -v > g_snd_peak : v > g_snd_peak)
            g_snd_peak = v < 0 ? -v : v;
        out[f] = (s16)(v > 32767 ? 32767 : v < -32768 ? -32768 : v);
    }
}

/* ------------------------------------------------------------ channel layer */

static int channel_level(const SndChannel *c)
{
    return c->active ? (c->env_decay >> 7) + c->user_decay + c->user_decay2 : -32768;
}

SndChannel *snd_alloc_channel(int type, u32 mask, int prio, SndTrack *t)
{
    /* the SDK's search order: prefer channels the hardware mixes without special use */
    static const u8 k_order[16] = { 4, 5, 6, 7, 2, 0, 3, 1, 8, 9, 10, 11, 14, 12, 15, 13 };
    static const u32 k_type_mask[3] = { 0xffff, 0x3f00, 0xc000 };
    SndChannel *best = NULL;
    int i;
    mask &= k_type_mask[type] & ~g_snd_locked;
    for (i = 0; i < 16; i++) {
        SndChannel *c = &g_snd_ch[k_order[i]];
        if (!(mask & (1u << k_order[i])))
            continue;
        if (best) {
            if (c->prio > best->prio)
                continue;
            if (c->prio == best->prio && channel_level(c) >= channel_level(best))
                continue;
        }
        best = c;
    }
    if (!best || prio < best->prio)
        return NULL;
    if (best->active)
        snd_channel_stop(best);
    {
        u8 locked = best->locked;
        memset(best, 0, sizeof *best);
        best->locked = locked;
    }
    best->type = (u8)type;
    best->prio = (u8)prio;
    best->track = t;
    return best;
}

void snd_channel_start(SndChannel *c)
{
    c->active = 1;
    c->started = 1;
    c->env_decay = SND_ENV_MIN;
    c->env_status = ENV_ATTACK;
    c->lfo.counter = 0;
    c->lfo.delay_counter = 0;
    c->sweep_counter = 0;
    if (c->type != CH_PCM) {
        c->pos = 0;
        c->frac = 0;
        c->lfsr = 0x7fff;
    }
    fetch(c);
}

void snd_channel_release(SndChannel *c, int release)
{
    if (!c->active)
        return;
    if (release >= 0)
        c->release = (u16)snd_calc_release(release);
    c->env_status = ENV_RELEASE;
    c->prio = 1;
}

void snd_channel_stop(SndChannel *c)
{
    if (c->track)
        snd_seq_channel_freed(c);
    c->active = 0;
    c->started = 0;
    c->prio = 0;
    c->track = NULL;
    c->direct = 0;
}

void snd_direct_start(SndChannel *c)
{
    c->pos = 0;
    c->frac = 0;
    if (!fetch(c))
        snd_channel_stop(c);
}

void snd_channels_update(void)
{
    int i;
    if (!g_tables)
        tables();
    for (i = 0; i < SND_CHANNEL_NUM; i++) {
        SndChannel *c = &g_snd_ch[i];
        s32 vol, pitch, pan, lfo = 0, sweep = 0;
        if (!c->active || c->direct)
            continue;
        /* envelope */
        switch (c->env_status) {
        case ENV_ATTACK:
            c->env_decay = -(s32)(((s64)-c->env_decay * c->attack) >> 8);
            if (c->env_decay == 0)
                c->env_status = ENV_DECAY;
            break;
        case ENV_DECAY: {
            s32 sus = (s32)snd_db_square(c->sustain) << 7;
            c->env_decay -= c->decay;
            if (c->env_decay <= sus) {
                c->env_decay = sus;
                c->env_status = ENV_SUSTAIN;
            }
            break;
        }
        case ENV_RELEASE:
            c->env_decay -= c->release;
            if (c->env_decay <= SND_ENV_MIN) {
                snd_channel_stop(c);
                continue;
            }
            break;
        default:
            break;
        }
        /* sweep (portamento / sweep pitch), counted per update when automatic, per tick otherwise */
        if (c->sweep_pitch && c->sweep_length && c->sweep_counter < c->sweep_length) {
            sweep = (s32)((s64)c->sweep_pitch * (c->sweep_length - c->sweep_counter) / c->sweep_length);
            if (c->auto_sweep)
                c->sweep_counter++;
        }
        /* LFO */
        if (c->lfo.depth) {
            if (c->lfo.delay_counter < c->lfo.delay) {
                c->lfo.delay_counter++;
            } else {
                c->lfo.counter = (u16)((c->lfo.counter + ((u32)c->lfo.speed << 6)) & 0x7fff);
                lfo = (s32)g_sin[(c->lfo.counter >> 8) & 127] * c->lfo.depth * c->lfo.range;
            }
        }
        vol = snd_db_square(c->velocity) + (c->env_decay >> 7) + c->user_decay + c->user_decay2;
        pitch = ((s32)(c->key - c->original_key) << 6) + sweep + c->user_pitch;
        pan = c->init_pan + c->user_pan;
        switch (c->lfo.target) {
        case 1:  vol += (lfo * 60) >> 14; break;
        case 2:  pan += lfo >> 14; break;
        default: pitch += (lfo << 6) >> 14; break;
        }
        if (pan < -64) pan = -64;
        if (pan > 63) pan = 63;
        c->hw_pan = (u8)(pan + 64);
        {
            s32 amp = db_gain(vol);
            c->gain_l = amp * (128 - c->hw_pan) / 128;
            c->gain_r = amp * c->hw_pan / 128;
        }
        /* PCM: the wave's timer at its original key; PSG / noise: A4 (440 Hz, 8 steps per period) */
        c->step = mix_step(c->type == CH_PCM ? c->wave.timer : 4760, pitch);
    }
}

/* direct channel register changes (stream players) */
void snd_direct_timer(SndChannel *c, u32 timer) { c->timer = (u16)timer; c->step = mix_step(timer, 0); }
