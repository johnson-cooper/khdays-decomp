/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_screen_020b4d4c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 screen hooks data_ov025_020b4d4c, 0x020b4d4c-0x020b4d90 (.data): the grid / panel list menu screen (02097eec .. 02098f60, 0x20a0-byte state: select 020984ac, cancel 020985b8, done 020988c0, the B handler 02098ad4).
 */

typedef void (*Ov008HookFn)(void);

/* 0x44-byte screen class: entered / stepped / left through the first three
 * slots by index (the ov008 screen dispatcher 020513bc / 020513f0 family via the
 * slot table data_ov025_020b4ab0); the list menus read pfnSelect / pfnCancel /
 * pfnDone (data_ov025_020b4d4c is rebound by 02099558 / 020995cc). */
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

extern void Ov025_GridMenuInitStep(void);
extern void Ov025_DestroyGridMenu(void);
extern void Ov025_TickGridMenu(void);
extern void Ov025_MenuKeyUp(void);
extern void Ov025_MenuKeyDown(void);
extern void Ov025_MenuKeyLeft(void);
extern void Ov025_MenuKeyRight(void);
extern void Ov025_GridMenuConfirm(void);
extern void Ov025_GridMenuBack(void);
extern void Ov025_GridConfirmOnPenUp(void);
extern void Ov025_GridConfirmOnPenUp_2(void);
extern void Ov025_TickIdleTouchSample(void);
extern void Ov025_SettleGridIfIdle(void);
extern void Ov025_MenuKeyCancel(void);

Ov008ScreenHooks data_ov025_020b4d4c __attribute__((aligned(__alignof__(Ov008ScreenHooks)))) = {
    Ov025_GridMenuInitStep,  /* pfnOpen */
    Ov025_DestroyGridMenu,  /* pfnStep */
    Ov025_TickGridMenu,  /* pfnClose */
    0,  /* nFlags */
    8352,  /* nStateSize */
    Ov025_MenuKeyUp,  /* pfnSelect */
    Ov025_MenuKeyDown,  /* pfnCancel */
    { Ov025_MenuKeyLeft, Ov025_MenuKeyRight },  /* apfnAux */
    Ov025_GridMenuConfirm,  /* pfnDone */
    { Ov025_GridMenuBack, 0, Ov025_TickIdleTouchSample, Ov025_GridConfirmOnPenUp, Ov025_GridConfirmOnPenUp_2, Ov025_SettleGridIfIdle, Ov025_MenuKeyCancel },  /* apfnHook */
};
