/* PS2: mechanically prepared copy of src/overlays/players/ov047_player_donald/data/ov047_pointers_020b42e0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov047 .data pointer tables, 0x020b42e0-0x020b42f4.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepStandingShot(void);
extern void Ov047_PlayHitVoiceIfEnabled(void);

Ov_Fn data_ov047_020b42e0[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov047_PlayHitVoiceIfEnabled,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};
