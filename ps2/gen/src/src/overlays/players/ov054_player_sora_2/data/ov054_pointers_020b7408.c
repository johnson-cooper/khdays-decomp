/* PS2: mechanically prepared copy of src/overlays/players/ov054_player_sora_2/data/ov054_pointers_020b7408.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov054 .data pointer tables, 0x020b7408-0x020b741c.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepStandingShot(void);
extern void Ov054_StepProjectile(void);

Ov_Fn data_ov054_020b7408[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov054_StepProjectile,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};
