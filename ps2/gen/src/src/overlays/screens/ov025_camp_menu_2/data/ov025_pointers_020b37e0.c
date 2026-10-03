/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_pointers_020b37e0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 .rodata pointer tables, 0x020b37e0-0x020b3808.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov025_UpdateCampaignMenuManager(void);
extern void Ov025_UpdateInputAndEnterMode3(void);
extern void Ov025_InitThenDispatchTwoHandlers(void);
extern void Ov025_WaitLoadAdvancePhase(void);
extern void Ov025_AllocWorkBufferInit(void);
extern void Ov025_UpdateInputAndEnableHalves(void);
extern void Ov025_UpdateInputAndEnterMode8(void);
extern void Ov025_FullScreenTeardown(void);
extern void Ov025_ScreenTeardown(void);

const Ov_Fn data_ov025_020b37e0[10] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov025_UpdateCampaignMenuManager,

    Ov025_UpdateInputAndEnterMode3,

    Ov025_InitThenDispatchTwoHandlers,

    Ov025_WaitLoadAdvancePhase,

    Ov025_AllocWorkBufferInit,

    Ov025_UpdateInputAndEnableHalves,

    Ov025_UpdateInputAndEnterMode8,

    Ov025_ScreenTeardown,

    Ov025_FullScreenTeardown,

};
