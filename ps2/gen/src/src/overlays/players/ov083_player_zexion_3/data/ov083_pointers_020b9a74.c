/* PS2: mechanically prepared copy of src/overlays/players/ov083_player_zexion_3/data/ov083_pointers_020b9a74.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov083 .data pointer tables, 0x020b9a74-0x020b9a88.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepShot_2(void);
extern void Ov022_StepStandingShot(void);

Ov_Fn data_ov083_020b9a74[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov022_StepShot_2,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};
