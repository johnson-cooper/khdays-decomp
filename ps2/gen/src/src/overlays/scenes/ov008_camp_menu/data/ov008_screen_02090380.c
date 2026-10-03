/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_screen_02090380.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 screen hooks data_ov008_02090380, 0x02090380-0x020903c4 (.data): the list menu screen (020659c0 .. 02066b10, 0x20a0-byte state).
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

extern void Ov008_GridMenuInitStep(void);
extern void Ov008_DestroyGridMenu(void);
extern void Ov008_TickGridMenu(void);
extern void Ov008_MenuKeyUp(void);
extern void Ov008_MenuKeyDown(void);
extern void Ov008_MenuKeyLeft(void);
extern void Ov008_MenuKeyRight(void);
extern void Ov008_GridMenuConfirm(void);
extern void Ov008_GridMenuBack(void);
extern void Ov008_GridConfirmOnPenUp(void);
extern void Ov008_GridConfirmOnPenUp2(void);
extern void Ov008_TickIdleTouchSample(void);
extern void Ov008_SettleGridIfIdle(void);
extern void Ov008_MenuKeyCancel(void);

Ov008ScreenHooks data_ov008_02090380 __attribute__((aligned(__alignof__(Ov008ScreenHooks)))) = {
    Ov008_GridMenuInitStep,  /* pfnOpen */
    Ov008_DestroyGridMenu,  /* pfnStep */
    Ov008_TickGridMenu,  /* pfnClose */
    0,  /* nFlags */
    8352,  /* nStateSize */
    Ov008_MenuKeyUp,  /* pfnSelect */
    Ov008_MenuKeyDown,  /* pfnCancel */
    { Ov008_MenuKeyLeft, Ov008_MenuKeyRight },  /* apfnAux */
    Ov008_GridMenuConfirm,  /* pfnDone */
    { Ov008_GridMenuBack, 0, Ov008_TickIdleTouchSample, Ov008_GridConfirmOnPenUp, Ov008_GridConfirmOnPenUp2, Ov008_SettleGridIfIdle, Ov008_MenuKeyCancel },  /* apfnHook */
};
