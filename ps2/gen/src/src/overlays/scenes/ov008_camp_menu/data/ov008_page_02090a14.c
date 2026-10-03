/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_page_02090a14.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 page hooks data_ov008_02090a14, 0x02090a14-0x02090a54 (.data): the mission detail page (02078df0 .. 020791e0, 0x214-byte state).
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

extern void Ov008_SetupShopScreen(void);
extern void Ov008_DestroyPageB(void);
extern void Ov008_PageB_StepSlideIfBusy(void);
extern void Ov008_MovePageBUp(void);
extern void Ov008_MovePageBDown(void);
extern void Ov008_ConfirmPageBSelection(void);

Ov008PageHooks data_ov008_02090a14 __attribute__((aligned(__alignof__(Ov008PageHooks)))) = {
    Ov008_SetupShopScreen,  /* pfnOpen */
    Ov008_DestroyPageB,  /* pfnStep */
    Ov008_PageB_StepSlideIfBusy,  /* pfnClose */
    532,  /* nStateSize */
    { 0, 0, 0, 0, 0, 0, 0, 0, Ov008_MovePageBUp, Ov008_MovePageBDown, 0, Ov008_ConfirmPageBSelection },  /* apfnHook */
};
