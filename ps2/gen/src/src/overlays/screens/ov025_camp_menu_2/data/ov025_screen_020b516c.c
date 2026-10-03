/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_screen_020b516c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 screen hooks data_ov025_020b516c, 0x020b516c-0x020b51b0 (.data): the tutorial page (Ov025_Tutorial_Setup 0209e41c / teardown 0209e490 / input 0209e5e8, flags 4, 0x2b0-byte Ov025TutorialPage; cursor 0209de5c / 0209df14, pages 0209dfbc / 0209e060).
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

extern void Ov025_Tutorial_CursorUp(void);
extern void Ov025_Tutorial_CursorDown(void);
extern void Ov025_Tutorial_PageUp(void);
extern void Ov025_Tutorial_PageDown(void);
extern void Ov025_ResetIfNotBusy(void);
extern void Ov025_Dma0HookA(void);
extern void Ov025_Dma0HookB(void);
extern void Ov025_SetupListScreen(void);
extern void Ov025_TeardownListScreen(void);
extern void Ov025_Tutorial_HandleTouch(void);

Ov008ScreenHooks data_ov025_020b516c __attribute__((aligned(__alignof__(Ov008ScreenHooks)))) = {
    Ov025_SetupListScreen,  /* pfnOpen */
    Ov025_TeardownListScreen,  /* pfnStep */
    Ov025_Tutorial_HandleTouch,  /* pfnClose */
    4,  /* nFlags */
    688,  /* nStateSize */
    Ov025_Tutorial_CursorUp,  /* pfnSelect */
    Ov025_Tutorial_CursorDown,  /* pfnCancel */
    { Ov025_Tutorial_PageUp, Ov025_Tutorial_PageDown },  /* apfnAux */
    Ov025_Dma0HookB,  /* pfnDone */
    { Ov025_ResetIfNotBusy, 0, 0, Ov025_Dma0HookA, Ov025_Dma0HookB, 0, Ov025_ResetIfNotBusy },  /* apfnHook */
};
