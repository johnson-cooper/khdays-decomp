/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_screen_02090460.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 screen hooks data_ov008_02090460, 0x02090460-0x020904a4 (.data): the grid screen (02068904 .. 02068ba8, 0x1f00-byte state).
 */

typedef void (*Ov008HookFn)(void);

/* 0x44-byte screen class: entered / stepped / left through the first three
 * slots by index (Ov008_Fn_137c and its +4 / +8 siblings); the list menus
 * read pfnSelect / pfnCancel / pfnDone (Ov008_FinishMenuModeSwitch). */
typedef struct Ov008ScreenHooks {
    Ov008HookFn pfnOpen;      /* 0x00 */
    Ov008HookFn pfnStep;      /* 0x04 */
    Ov008HookFn pfnClose;     /* 0x08 */
    int nFlags;               /* 0x0c */
    int nStateSize;           /* 0x10 */
    Ov008HookFn pfnSelect;    /* 0x14 */
    Ov008HookFn pfnCancel;    /* 0x18 */
    Ov008HookFn apfnAux[2];   /* 0x1c */
    Ov008HookFn pfnDone;      /* 0x24 */
    Ov008HookFn apfnHook[7];  /* 0x28 .. 0x40 */
} Ov008ScreenHooks;

extern void Ov008_StepSelectionBackward(void);
extern void Ov008_StepSelectionForward(void);
extern void Ov008_HandleElementTap(void);
extern void Ov008_HandleElementRelease(void);
extern void Ov008_SaveMenuConfirm(void);
extern void Ov008_SaveMenu_OnBack(void);
extern void Ov008_SaveMenuInitStep(void);
extern void Ov008_TeardownListScene(void);
extern void Ov008_SaveMenuTick(void);

Ov008ScreenHooks data_ov008_02090460 __attribute__((aligned(__alignof__(Ov008ScreenHooks)))) = {
    Ov008_SaveMenuInitStep,  /* pfnOpen */
    Ov008_TeardownListScene,  /* pfnStep */
    Ov008_SaveMenuTick,  /* pfnClose */
    0,  /* nFlags */
    7936,  /* nStateSize */
    Ov008_StepSelectionBackward,  /* pfnSelect */
    Ov008_StepSelectionForward,  /* pfnCancel */
    { Ov008_HandleElementRelease, Ov008_HandleElementTap },  /* apfnAux */
    Ov008_SaveMenuConfirm,  /* pfnDone */
    { Ov008_SaveMenu_OnBack, 0, 0, 0, 0, 0, Ov008_SaveMenu_OnBack },  /* apfnHook */
};
