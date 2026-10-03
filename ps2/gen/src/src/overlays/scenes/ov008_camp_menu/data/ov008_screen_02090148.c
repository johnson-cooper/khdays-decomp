/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_screen_02090148.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 screen hooks data_ov008_02090148, 0x02090148-0x0209018c (.data): the scene screen (Ov008_SetupScene, 0x5d4-byte state).
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

extern void func_ov008_020594c4(void);
extern void func_ov008_0205968c(void);
extern void Ov008_MenuScreenHook(void);
extern void Ov008_SetupScene(void);
extern void Ov008_TeardownScene(void);

Ov008ScreenHooks data_ov008_02090148 __attribute__((aligned(__alignof__(Ov008ScreenHooks)))) = {
    Ov008_SetupScene,  /* pfnOpen */
    Ov008_TeardownScene,  /* pfnStep */
    func_ov008_0205968c,  /* pfnClose */
    0,  /* nFlags */
    1492,  /* nStateSize */
    0,  /* pfnSelect */
    0,  /* pfnCancel */
    { 0, 0 },  /* apfnAux */
    0,  /* pfnDone */
    { func_ov008_020594c4, 0, 0, 0, 0, 0, Ov008_MenuScreenHook },  /* apfnHook */
};
