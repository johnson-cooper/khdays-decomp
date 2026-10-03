/* PS2: mechanically prepared copy of src/overlays/players/ov084_player_mickey_3/data/ov084_pointers_020b9994.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov084 .data pointer tables, 0x020b9994-0x020b99a8.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov084_InitRadialBurst(void);
extern void Ov084_StepHeldProjectile(void);

Ov_Fn data_ov084_020b9994[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov084_InitRadialBurst,

    Ov022_NullStep,

    Ov084_StepHeldProjectile,

};
