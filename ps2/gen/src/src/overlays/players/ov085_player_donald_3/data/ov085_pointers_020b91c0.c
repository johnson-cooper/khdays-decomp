/* PS2: mechanically prepared copy of src/overlays/players/ov085_player_donald_3/data/ov085_pointers_020b91c0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov085 .data pointer tables, 0x020b91c0-0x020b91d4.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepStandingShot(void);
extern void Ov085_PlayHitVoiceIfEnabled(void);

Ov_Fn data_ov085_020b91c0[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov085_PlayHitVoiceIfEnabled,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};
