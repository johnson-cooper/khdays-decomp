/* PS2: mechanically prepared copy of src/overlays/scenes/ov009_camp_save/data/ov009_pointers_02055f88.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov009 .rodata pointer tables, 0x02055f88-0x02055fb0.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov009_Menu_PreparePage(void);
extern void Ov009_UpdateInputAndEnterMode3(void);
extern void Ov009_InitThenDispatchTwoHandlers(void);
extern void Ov009_WaitLoadAdvancePhase(void);
extern void Ov009_AllocWorkBufferInit(void);
extern void Ov009_UpdateInputAndEnableHalves(void);
extern void Ov009_UpdateInputAndEnterMode8(void);
extern void Ov009_FullScreenTeardown(void);
extern void Ov009_ScreenTeardown(void);

const Ov_Fn data_ov009_02055f88[10] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov009_Menu_PreparePage,

    Ov009_UpdateInputAndEnterMode3,

    Ov009_InitThenDispatchTwoHandlers,

    Ov009_WaitLoadAdvancePhase,

    Ov009_AllocWorkBufferInit,

    Ov009_UpdateInputAndEnableHalves,

    Ov009_UpdateInputAndEnterMode8,

    Ov009_ScreenTeardown,

    Ov009_FullScreenTeardown,

};
