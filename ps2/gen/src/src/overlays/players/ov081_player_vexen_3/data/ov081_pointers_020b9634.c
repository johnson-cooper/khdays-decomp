/* PS2: mechanically prepared copy of src/overlays/players/ov081_player_vexen_3/data/ov081_pointers_020b9634.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov081 .data pointer tables, 0x020b9634-0x020b9648.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepStandingShot(void);

Ov_Fn data_ov081_020b9634[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    0,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};
