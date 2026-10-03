/* PS2: mechanically prepared copy of src/overlays/players/ov064_player_zexion_2/data/ov064_pointers_020b7394.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov064 .data pointer tables, 0x020b7394-0x020b73a8.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepShot_2(void);
extern void Ov022_StepStandingShot(void);

Ov_Fn data_ov064_020b7394[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov022_StepShot_2,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};
