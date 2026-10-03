/* PS2: mechanically prepared copy of src/overlays/players/ov087_player_roxas_dual_3/data/ov087_pointers_020b9b28.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov087 .data pointer tables, 0x020b9b28-0x020b9b3c.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepStandingShot(void);
extern void Ov087_TickThrowArcThenLand(void);

Ov_Fn data_ov087_020b9b28[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov087_TickThrowArcThenLand,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};
