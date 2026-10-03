/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_screen_020b51f8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 screen hooks data_ov025_020b51f8, 0x020b51f8-0x020b523c (.data): the reports page (020a086c / Ov025_Reports_Teardown 020a08c0 / Ov025_Reports_HandleInput 020a0a9c, flags 6, 0x278-byte Ov025ReportsPage; cursor 0209fa88 / 0209fc64, pages 0209fe48 / 0209ffd0).
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

extern void Ov025_Reports_CursorUp(void);
extern void Ov025_Reports_CursorDown(void);
extern void Ov025_Reports_PageUp(void);
extern void Ov025_Reports_PageDown(void);
extern void Ov025_Reports_Leave(void);
extern void Ov025_Reports_ToggleView(void);
extern void Ov025_SetupPanelScreen(void);
extern void Ov025_Reports_Teardown(void);
extern void Ov025_Reports_HandleInput(void);

Ov008ScreenHooks data_ov025_020b51f8 __attribute__((aligned(__alignof__(Ov008ScreenHooks)))) = {
    Ov025_SetupPanelScreen,  /* pfnOpen */
    Ov025_Reports_Teardown,  /* pfnStep */
    Ov025_Reports_HandleInput,  /* pfnClose */
    6,  /* nFlags */
    632,  /* nStateSize */
    Ov025_Reports_CursorUp,  /* pfnSelect */
    Ov025_Reports_CursorDown,  /* pfnCancel */
    { Ov025_Reports_PageUp, Ov025_Reports_PageDown },  /* apfnAux */
    0,  /* pfnDone */
    { Ov025_Reports_Leave, 0, 0, Ov025_Reports_ToggleView, Ov025_Reports_ToggleView, 0, Ov025_Reports_Leave },  /* apfnHook */
};
