/* PS2: mechanically prepared copy of src/overlays/players/ov100_player_zexion_4/data/ov100_pointers_020bc134.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov100 .data pointer tables, 0x020bc134-0x020bc148.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepShot_2(void);
extern void Ov022_StepStandingShot(void);

Ov_Fn data_ov100_020bc134[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov022_StepShot_2,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};
