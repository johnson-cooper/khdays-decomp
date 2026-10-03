/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_page_020907a0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 page hooks data_ov008_020907a0, 0x020907a0-0x020907e0 (.data): the menu state page (Ov008_TickMenuState, 0x240-byte state).
 */

typedef void (*Ov008HookFn)(void);

/* 0x40-byte page class: entered / stepped / left through the first three
 * slots by index (Ov008_Fn_1458 and its +4 / +8 siblings), then the state
 * size and twelve page hooks. */
typedef struct Ov008PageHooks {
    Ov008HookFn pfnOpen;      /* 0x00 */
    Ov008HookFn pfnStep;      /* 0x04 */
    Ov008HookFn pfnClose;     /* 0x08 */
    int nStateSize;           /* 0x0c */
    Ov008HookFn apfnHook[12]; /* 0x10 .. 0x3c */
} Ov008PageHooks;

extern void Ov008_TickMenuState(void);
extern void Ov008_TeardownSubScene(void);
extern void Ov008_PanelUpdate(void);
extern void Ov008_SetupMenuButtons(void);

Ov008PageHooks data_ov008_020907a0 __attribute__((aligned(__alignof__(Ov008PageHooks)))) = {
    Ov008_TickMenuState,  /* pfnOpen */
    Ov008_TeardownSubScene,  /* pfnStep */
    Ov008_PanelUpdate,  /* pfnClose */
    576,  /* nStateSize */
    { 0, 0, 0, 0, 0, 0, Ov008_SetupMenuButtons, 0, 0, 0, 0, 0 },  /* apfnHook */
};
