/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_screen_0209041c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 screen hooks data_ov008_0209041c, 0x0209041c-0x02090460 (.data): the transition screen (02067698, flags 3, 4-byte state).
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

extern void func_ov008_020676a0(void);
extern void Ov008_ScreenOpenNoOp(void);
extern void Ov008_ScreenCloseNoOp(void);

Ov008ScreenHooks data_ov008_0209041c __attribute__((aligned(__alignof__(Ov008ScreenHooks)))) = {
    Ov008_ScreenOpenNoOp,  /* pfnOpen */
    func_ov008_020676a0,  /* pfnStep */
    Ov008_ScreenCloseNoOp,  /* pfnClose */
    3,  /* nFlags */
    4,  /* nStateSize */
    0,  /* pfnSelect */
    0,  /* pfnCancel */
    { 0, 0 },  /* apfnAux */
    0,  /* pfnDone */
    { 0, 0, 0, 0, 0, 0, 0 },  /* apfnHook */
};
