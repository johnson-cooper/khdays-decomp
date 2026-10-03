/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_page_020b54b8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 page hooks data_ov025_020b54b8, 0x020b54b8-0x020b54f8 (.data): the day list of page B (Ov025_ScrollList_Open 020ae6f0 / Release 020ae998 / per-frame 020aea2c, 0x2e8-byte Ov025ScrollList; leave Ov025_ScrollList_Leave 020aebc8).
 */

typedef void (*Ov008HookFn)(void);

/* 0x40-byte page class: entered / stepped / left through the first three
 * slots by index (the ov008 page dispatcher 02051498 / 020514cc family via the
 * slot table data_ov025_020b4a78), then the state size and twelve page hooks. */
typedef struct Ov008PageHooks {
    Ov008HookFn pfnOpen;      /* 0x00 */
    Ov008HookFn pfnStep;      /* 0x04 */
    Ov008HookFn pfnClose;     /* 0x08 */
    int nStateSize;           /* 0x0c */
    Ov008HookFn apfnHook[12]; /* 0x10 .. 0x3c */
} Ov008PageHooks;

extern void func_ov025_020aebbc(void);
extern void Ov025_ScrollList_Open(void);
extern void Ov025_ScrollList_Release(void);
extern void Ov025_ListViewUpdate(void);
extern void Ov025_StepBackIfState4(void);
extern void Ov025_StepForwardIfState8(void);
extern void Ov025_ScrollList_PageUpOnKey(void);
extern void Ov025_ScrollList_PageDownOnKey(void);
extern void Ov025_ScrollList_Leave(void);

Ov008PageHooks data_ov025_020b54b8 __attribute__((aligned(__alignof__(Ov008PageHooks)))) = {
    Ov025_ScrollList_Open,  /* pfnOpen */
    Ov025_ScrollList_Release,  /* pfnStep */
    Ov025_ListViewUpdate,  /* pfnClose */
    744,  /* nStateSize */
    { Ov025_StepBackIfState4, Ov025_StepForwardIfState8, Ov025_ScrollList_PageUpOnKey, Ov025_ScrollList_PageDownOnKey, func_ov025_020aebbc, Ov025_ScrollList_Leave, 0, 0, 0, 0, 0, Ov025_ScrollList_Leave },  /* apfnHook */
};
