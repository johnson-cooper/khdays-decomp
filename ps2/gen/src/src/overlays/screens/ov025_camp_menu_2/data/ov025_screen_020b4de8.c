/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_screen_020b4de8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 screen hooks data_ov025_020b4de8, 0x020b4de8-0x020b4e2c (.data): the transition screen (02099a78 / 02099a80 / 02099a90, flags 3, 4-byte state).
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

extern void func_ov025_02099a80(void);
extern void Ov025_ScreenOpenNoOp(void);
extern void Ov025_ScreenCloseNoOp(void);

Ov008ScreenHooks data_ov025_020b4de8 __attribute__((aligned(__alignof__(Ov008ScreenHooks)))) = {
    Ov025_ScreenOpenNoOp,  /* pfnOpen */
    func_ov025_02099a80,  /* pfnStep */
    Ov025_ScreenCloseNoOp,  /* pfnClose */
    3,  /* nFlags */
    4,  /* nStateSize */
    0,  /* pfnSelect */
    0,  /* pfnCancel */
    { 0, 0 },  /* apfnAux */
    0,  /* pfnDone */
    { 0, 0, 0, 0, 0, 0, 0 },  /* apfnHook */
};
