/* PS2: mechanically prepared copy of src/overlays/players/ov098_player_vexen_4/data/ov098_pointers_020bbcf4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov098 .data pointer tables, 0x020bbcf4-0x020bbd08.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepStandingShot(void);

Ov_Fn data_ov098_020bbcf4[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    0,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};
