/* PS2: mechanically prepared copy of src/overlays/players/ov102_player_donald_4/data/ov102_pointers_020bb880.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov102 .data pointer tables, 0x020bb880-0x020bb894.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepStandingShot(void);
extern void Ov102_PlayHitVoiceIfEnabled(void);

Ov_Fn data_ov102_020bb880[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov102_PlayHitVoiceIfEnabled,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};
