/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_page_020b53c4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 page hooks data_ov025_020b53c4, 0x020b53c4-0x020b5404 (.data): the mission menu page (020ad180 / 020ad3bc / 020ad50c, 0x570-byte state).
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

extern void func_ov025_020ad7ac(void);
extern void Ov025_MissionMenuInitStep(void);
extern void Ov025_MissionMenuDestroy(void);
extern void Ov025_MissionMenu_TickPanels(void);
extern void Ov025_SetState1IfIdle(void);
extern void Ov025_SetState2IfIdle(void);
extern void Ov025_RefreshPageOrNotifyToggle(void);
extern void Ov025_RefreshPageOrNotifyToggle_2(void);
extern void Ov025_MissionMenuConfirm(void);
extern void Ov025_TeardownOrInit(void);
extern void Ov025_StepBackIfIdle(void);
extern void Ov025_StepForwardIfIdle(void);

Ov008PageHooks data_ov025_020b53c4 __attribute__((aligned(__alignof__(Ov008PageHooks)))) = {
    Ov025_MissionMenuInitStep,  /* pfnOpen */
    Ov025_MissionMenuDestroy,  /* pfnStep */
    Ov025_MissionMenu_TickPanels,  /* pfnClose */
    1392,  /* nStateSize */
    { Ov025_SetState1IfIdle, Ov025_SetState2IfIdle, Ov025_RefreshPageOrNotifyToggle, Ov025_RefreshPageOrNotifyToggle_2, Ov025_MissionMenuConfirm, Ov025_TeardownOrInit, 0, 0, Ov025_StepBackIfIdle, Ov025_StepForwardIfIdle, 0, func_ov025_020ad7ac },  /* apfnHook */
};
