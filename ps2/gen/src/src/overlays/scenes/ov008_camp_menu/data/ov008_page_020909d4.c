/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_page_020909d4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 page hooks data_ov008_020909d4, 0x020909d4-0x02090a14 (.data): the mission menu transition page (020782c0, 4-byte state).
 */

typedef void (*Ov008HookFn)(void);

/* 0x40-byte page class: entered / stepped / left through the first three
 * slots by index (Ov008_Fn_1458 and its +4 / +8 siblings), then the state
 * size and twelve page hooks. */
typedef struct Ov008PageHooks {
    Ov008HookFn pfnOpen;      /* 0x00 */
    Ov008HookFn pfnStep;      /* 0x04 */
    Ov008HookFn pfnClose;     /* 0x08 */
    int nStateSize;           /* 0x0c */
    Ov008HookFn apfnHook[12]; /* 0x10 .. 0x3c */
} Ov008PageHooks;

extern void func_ov008_020782c8(void);
extern void Ov008_PageOpenNoOp(void);
extern void Ov008_PageCloseNoOp(void);

Ov008PageHooks data_ov008_020909d4 __attribute__((aligned(__alignof__(Ov008PageHooks)))) = {
    Ov008_PageOpenNoOp,  /* pfnOpen */
    func_ov008_020782c8,  /* pfnStep */
    Ov008_PageCloseNoOp,  /* pfnClose */
    4,  /* nStateSize */
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  /* apfnHook */
};
