/* PS2: mechanically prepared copy of src/overlays/players/ov061_player_vexen_2/data/ov061_pointers_020b6f54.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov061 .data pointer tables, 0x020b6f54-0x020b6f68.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepStandingShot(void);

Ov_Fn data_ov061_020b6f54[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    0,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};
