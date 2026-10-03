/* PS2: mechanically prepared copy of src/overlays/players/ov031_player_axel/data/ov031_pointers_020b4cd4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov031 .data pointer tables, 0x020b4cd4-0x020b4cfc.
 *
 * 2 tables, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov022_StepStandingShot(void);
extern void Ov031_HomingApproachTriggerStep(void);
extern void Ov031_HomingApproachStep(void);
extern void Ov031_advanceProjectileAndSteer(void);

Ov_Fn data_ov031_020b4cd4[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov031_HomingApproachTriggerStep,

    Ov022_NullStep,

    Ov022_StepStandingShot,

};

Ov_Fn data_ov031_020b4ce8[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov031_HomingApproachStep,

    Ov022_NullStep,

    Ov031_advanceProjectileAndSteer,

};
