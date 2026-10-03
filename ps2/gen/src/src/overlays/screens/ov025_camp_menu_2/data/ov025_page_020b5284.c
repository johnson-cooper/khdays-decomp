/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_page_020b5284.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 page hooks data_ov025_020b5284, 0x020b5284-0x020b52c4 (.data): the scrolling menu page (Ov008_TickMenuState 020a3f0c / 020a4018 / Ov025_PanelUpdate 020a40cc, 0x240-byte Ov008ScrollMenu).
 */

typedef void (*Ov008HookFn)(void);

/* 0x40-byte page class: entered / stepped / left through the first three
 * slots by index (the ov008 page dispatcher 02051498 / 020514cc family via the
 * slot table data_ov025_020b4a78), then the state size and twelve page hooks. */
typedef struct Ov008PageHooks {
    Ov008HookFn pfnOpen;      /* 0x00 */
    Ov008HookFn pfnStep;      /* 0x04 */
    Ov008HookFn pfnClose;     /* 0x08 */
    int nStateSize;           /* 0x0c */
    Ov008HookFn apfnHook[12]; /* 0x10 .. 0x3c */
} Ov008PageHooks;

extern void Ov025_TickMenuState(void);
extern void Ov025_TeardownSubScene(void);
extern void Ov025_PanelUpdate(void);
extern void Ov025_SetupMenuButtons(void);

Ov008PageHooks data_ov025_020b5284 __attribute__((aligned(__alignof__(Ov008PageHooks)))) = {
    Ov025_TickMenuState,  /* pfnOpen */
    Ov025_TeardownSubScene,  /* pfnStep */
    Ov025_PanelUpdate,  /* pfnClose */
    576,  /* nStateSize */
    { 0, 0, 0, 0, 0, 0, Ov025_SetupMenuButtons, 0, 0, 0, 0, 0 },  /* apfnHook */
};
