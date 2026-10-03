/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/data/ov022_pointers_020b2b48.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov022 .data pointer tables, 0x020b2b48-0x020b2b6c.
 *
 * 2 tables, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_StepShot(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepShot_2(void);
extern void Ov022_StepStandingShot(void);

Ov_Fn data_ov022_020b2b48[4] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov022_StepShot,

    Ov022_NullStep,

};

Ov_Fn data_ov022_020b2b58[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov022_StepShot_2,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};
