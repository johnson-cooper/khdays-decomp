/* The PS2 sound driver (the DS ARM7 sound driver's replacement): shared declarations.
 *
 *   snd_driver.c  command front end (PXI_SendWordByFifo), alarms, shared work, locking
 *   snd_seq.c     sequence players: SSEQ interpreter, SBNK instruments, SWAR waves
 *   snd_chan.c    the 16 DS channels: allocation, envelopes, LFO, sweep, and the software mixer
 *
 * Units are the DS driver's: volumes in 1/10 dB (-723 = silence), pitch in 1/64 semitone,
 * channel timers as the DS timer divisor (rate = SND_TIMER_CLOCK / timer).
 */
#ifndef KH_SND_INTERNAL_H
#define KH_SND_INTERNAL_H

#include "platform/kh_platform.h"
#include <tamtypes.h>

#define SND_CHANNEL_NUM   16
#define SND_PLAYER_NUM    16
#define SND_TRACK_NUM     16      /* per player */
#define SND_TIMER_CLOCK   16756991
#define SND_DB_MIN        (-723)
#define SND_ENV_MIN       (SND_DB_MIN << 7)
/* the ARM7 driver's update period: 2728 x 64 cycles of the 33.513982 MHz clock */
#define SND_UPDATE_HZ     (33513982.0f / (64.0f * 2728.0f))
#define SND_MIX_RATE      48000

/* DS file structures (SDAT), as laid out in memory */
typedef struct SndFileHeader { char sig[4]; u16 bom, ver; u32 file_size; u16 header_size, blocks; } SndFileHeader;
typedef struct SndWaveParam { u8 format, loop; u16 rate, timer, loop_start; u32 loop_len; } SndWaveParam;
typedef struct SndWaveArc { SndFileHeader fh; u32 kind, size; void *top_link; u32 reserved[7]; u32 count; u32 offset[1]; } SndWaveArc;
typedef struct SndWaveArcLink { SndWaveArc *arc; void *next; } SndWaveArcLink;
typedef struct SndBank { SndFileHeader fh; u32 kind, size; SndWaveArcLink link[4]; u32 count; u32 offset[1]; } SndBank;
typedef struct SndInstParam { u16 wave[2]; u8 key, attack, decay, sustain, release, pan; } SndInstParam;
typedef struct SndInst { u8 type; struct SndInstParam p; } SndInst;
enum { INST_PCM = 1, INST_PSG, INST_NOISE, INST_DIRECTPCM, INST_NULL, INST_DRUM = 0x10, INST_SPLIT };
enum { FMT_PCM8, FMT_PCM16, FMT_ADPCM, FMT_PSG };
enum { CH_PCM, CH_PSG, CH_NOISE };
enum { ENV_ATTACK, ENV_DECAY, ENV_SUSTAIN, ENV_RELEASE };

typedef struct SndLfo { u8 target, speed, depth, range; u16 delay, delay_counter, counter; } SndLfo;

struct SndTrack;

typedef struct SndChannel {
    /* driver state (the SDK's SNDExChannel) */
    u8 active, started, type, env_status;
    u8 locked;             /* reserved for direct use (streams, LOCK_CHANNEL) */
    u8 direct;             /* set up by SETUP_CHANNEL_*: registers given directly, no envelope */
    u8 prio, key, original_key, velocity, pan_range, auto_sweep;
    s8 init_pan, user_pan;
    s16 user_decay, user_decay2, user_pitch, sweep_pitch;
    s32 env_decay, sweep_counter, sweep_length, length;
    u8 attack, sustain;
    u16 decay, release;
    SndLfo lfo;
    SndWaveParam wave;
    const u8 *data;        /* samples (after the wave header) */
    u8 duty;
    struct SndTrack *track;
    struct SndChannel *next_in_track;
    /* hardware registers */
    u16 timer;             /* divisor */
    u8 hw_vol, hw_shift, hw_pan;
    /* mixer state */
    u32 pos, frac, step;   /* frac: Q16 for sequenced channels, sound-clock remainder for direct */
                           /* step: Q16 source samples per 48 kHz output sample */
    u32 len, loop_start;   /* samples */
    s32 sample;            /* current output sample (held between source samples) */
    s32 gain_l, gain_r;    /* 0..32768 */
    s32 adpcm_pred, adpcm_idx, adpcm_next;     /* ADPCM: decoder state at sample adpcm_next */
    s32 loop_pred, loop_idx; u8 loop_saved;
    u16 lfsr;
} SndChannel;

typedef struct SndTrack {
    u8 active, note_wait, mute, tie, note_finish_wait, porta, cmp;
    u8 pan_range, volume, volume2, bend_range, prio;
    s8 pitch_bend, pan, ext_pan, transpose;
    s16 ext_fader, ext_pitch, sweep_pitch;
    u8 attack, decay, sustain, release;
    u8 porta_key, porta_time;
    u16 prg, channel_mask;
    SndLfo mod;
    s32 wait;
    const u8 *base, *cur;
    const u8 *call_stack[3];
    u8 loop_count[3], depth;
    SndChannel *channels;
} SndTrack;

typedef struct SndPlayer {
    u8 active, prepared, paused, prio, volume;
    s16 ext_fader;
    u16 tempo, tempo_ratio, tempo_counter;
    const SndBank *bank;
    SndTrack track[SND_TRACK_NUM];
    u32 track_open;        /* bit per track opened */
    u32 ticks;
} SndPlayer;

extern SndChannel g_snd_ch[SND_CHANNEL_NUM];
extern SndPlayer g_snd_player[SND_PLAYER_NUM];
extern u32 g_snd_locked;          /* LOCK_CHANNEL mask */
extern u8 g_snd_master_volume;

/* snd_chan.c */
s16 snd_db_square(int v);                         /* 0..127 -> 1/10 dB (velocity / volume curve) */
SndChannel *snd_alloc_channel(int type, u32 mask, int prio, SndTrack *t);
void snd_channel_start(SndChannel *c);
void snd_channel_release(SndChannel *c, int release);
void snd_channel_stop(SndChannel *c);
void snd_direct_start(SndChannel *c);             /* start a register-driven PCM channel at sample 0 */
void snd_channels_update(void);                   /* envelopes, LFO, sweep -> registers (each update) */
void snd_mix(s16 *out, int frames);               /* the hardware: channels -> stereo samples */
void snd_hw_setup(SndChannel *c, const u8 *data, int fmt, int loop, u32 loop_start, u32 loop_len);
int snd_calc_attack(int a);
int snd_calc_release(int v);

/* snd_seq.c */
void snd_seq_prepare(int player, const u8 *base, u32 offset, const SndBank *bank);
void snd_seq_start(int player);
void snd_seq_stop(int player);
void snd_seq_update(void);                        /* one driver update: tempo -> ticks */
void snd_seq_skip(int player, u32 ticks);
void snd_seq_mute(int player, u32 mask, int flag);
void snd_seq_channel_freed(SndChannel *c);
s16 *snd_var(int player, int idx);                /* local (0-15) / global (16-31) variables */

/* snd_driver.c */
extern volatile u8 *g_snd_shared;                 /* SNDSharedWork (ARM9 memory) */
void snd_lock(void);
void snd_unlock(void);

#endif
