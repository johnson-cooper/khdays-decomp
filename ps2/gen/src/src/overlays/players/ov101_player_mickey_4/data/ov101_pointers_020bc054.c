/* PS2: mechanically prepared copy of src/overlays/players/ov101_player_mickey_4/data/ov101_pointers_020bc054.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov101 .data pointer tables, 0x020bc054-0x020bc068.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov101_InitRadialBurst(void);
extern void Ov101_StepHeldProjectile(void);

Ov_Fn data_ov101_020bc054[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov101_InitRadialBurst,

    Ov022_NullStep,

    Ov101_StepHeldProjectile,

};
