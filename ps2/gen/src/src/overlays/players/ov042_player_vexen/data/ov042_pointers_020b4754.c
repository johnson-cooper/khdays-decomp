/* PS2: mechanically prepared copy of src/overlays/players/ov042_player_vexen/data/ov042_pointers_020b4754.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov042 .data pointer tables, 0x020b4754-0x020b4768.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepStandingShot(void);

Ov_Fn data_ov042_020b4754[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    0,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};
