/* PS2: mechanically prepared copy of src/overlays/players/ov091_player_sora_4/data/ov091_pointers_020bc1a8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov091 .data pointer tables, 0x020bc1a8-0x020bc1bc.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepStandingShot(void);
extern void Ov091_StepProjectile(void);

Ov_Fn data_ov091_020bc1a8[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov091_StepProjectile,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};
