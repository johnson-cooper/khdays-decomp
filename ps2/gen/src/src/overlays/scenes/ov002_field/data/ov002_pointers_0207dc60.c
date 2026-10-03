/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/data/ov002_pointers_0207dc60.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov002 .rodata pointer tables, 0x0207dc60-0x0207dc90.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov002_GetPanelField012c(void);
extern void Ov002_ConfirmSelection(void);
extern void Ov002_SetPanelField018c(void);
extern void Ov002_BeginTextCrawl(void);
extern void Ov002_StepTextCrawl(void);
extern void Ov002_HoldAfterTextCrawl(void);
extern void Ov002_FinishPanelFadeIn(void);
extern void Ov002_ReleasePanelSurface(void);
extern void Ov002_ClosePanel(void);
extern void Ov002_LeavePanelForMap(void);
extern void Ov002_ReturnFromMapToPanel(void);
extern void Ov002_FlushPanelDisplayList(void);

const Ov_Fn data_ov002_0207dc60[12] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov002_GetPanelField012c,

    Ov002_ConfirmSelection,

    Ov002_SetPanelField018c,

    Ov002_BeginTextCrawl,

    Ov002_StepTextCrawl,

    Ov002_HoldAfterTextCrawl,

    Ov002_FinishPanelFadeIn,

    Ov002_ReleasePanelSurface,

    Ov002_ClosePanel,

    Ov002_LeavePanelForMap,

    Ov002_ReturnFromMapToPanel,

    Ov002_FlushPanelDisplayList,

};
