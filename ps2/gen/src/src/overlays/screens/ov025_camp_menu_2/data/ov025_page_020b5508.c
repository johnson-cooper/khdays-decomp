/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_page_020b5508.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 page hooks data_ov025_020b5508, 0x020b5508-0x020b5548 (.data): the transition page (020aed18 / 020aed20 / 020aed30, 4-byte state).
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

extern void func_ov025_020aed20(void);
extern void Ov025_PageOpenNoOp(void);
extern void Ov025_PageCloseNoOp(void);

Ov008PageHooks data_ov025_020b5508 __attribute__((aligned(__alignof__(Ov008PageHooks)))) = {
    Ov025_PageOpenNoOp,  /* pfnOpen */
    func_ov025_020aed20,  /* pfnStep */
    Ov025_PageCloseNoOp,  /* pfnClose */
    4,  /* nStateSize */
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  /* apfnHook */
};
