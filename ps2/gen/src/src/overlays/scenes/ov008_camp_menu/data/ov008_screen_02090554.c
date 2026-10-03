/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_screen_02090554.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 screen hooks data_ov008_02090554, 0x02090554-0x02090598 (.data): the item ring screen (0206b494 .. 0206b540, 0xbc-byte state).
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

extern void Ov008_MenuPage_Init(void);
extern void Ov008_TeardownSimpleScene(void);
extern void Ov008_TickMenuTierLabel(void);

Ov008ScreenHooks data_ov008_02090554 __attribute__((aligned(__alignof__(Ov008ScreenHooks)))) = {
    Ov008_MenuPage_Init,  /* pfnOpen */
    Ov008_TeardownSimpleScene,  /* pfnStep */
    Ov008_TickMenuTierLabel,  /* pfnClose */
    0,  /* nFlags */
    188,  /* nStateSize */
    0,  /* pfnSelect */
    0,  /* pfnCancel */
    { 0, 0 },  /* apfnAux */
    0,  /* pfnDone */
    { 0, 0, 0, 0, 0, 0, 0 },  /* apfnHook */
};
