/* PS2: mechanically prepared copy of src/overlays/players/ov035_player_sora/data/ov035_pointers_020b4c08.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov035 .data pointer tables, 0x020b4c08-0x020b4c1c.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepStandingShot(void);
extern void Ov035_StepProjectile(void);

Ov_Fn data_ov035_020b4c08[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov035_StepProjectile,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};
