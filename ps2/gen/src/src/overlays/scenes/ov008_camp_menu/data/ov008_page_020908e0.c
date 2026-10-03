/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_page_020908e0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 page hooks data_ov008_020908e0, 0x020908e0-0x02090920 (.data): the mission menu page (02077b28 .. 02078154, 0x570-byte state).
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

extern void func_ov008_02078154(void);
extern void Ov008_MissionMenuInitStep(void);
extern void Ov008_MissionMenuDestroy(void);
extern void Ov008_MissionMenu_TickPanels(void);
extern void Ov008_SetState1IfIdle(void);
extern void Ov008_SetState2IfIdle(void);
extern void Ov008_RefreshPageOrNotifyToggle(void);
extern void Ov008_RefreshPageOrNotifyToggle_2(void);
extern void Ov008_MissionMenuConfirm(void);
extern void Ov008_MissionMenuBack(void);
extern void Ov008_StepBackIfIdle(void);
extern void Ov008_StepForwardIfIdle(void);

Ov008PageHooks data_ov008_020908e0 __attribute__((aligned(__alignof__(Ov008PageHooks)))) = {
    Ov008_MissionMenuInitStep,  /* pfnOpen */
    Ov008_MissionMenuDestroy,  /* pfnStep */
    Ov008_MissionMenu_TickPanels,  /* pfnClose */
    1392,  /* nStateSize */
    { Ov008_SetState1IfIdle, Ov008_SetState2IfIdle, Ov008_RefreshPageOrNotifyToggle, Ov008_RefreshPageOrNotifyToggle_2, Ov008_MissionMenuConfirm, Ov008_MissionMenuBack, 0, 0, Ov008_StepBackIfIdle, Ov008_StepForwardIfIdle, 0, func_ov008_02078154 },  /* apfnHook */
};
