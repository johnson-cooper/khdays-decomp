/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_page_0209082c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 page hooks data_ov008_0209082c, 0x0209082c-0x0209086c (.data): the mission list page (Ov008_MissionListInitStep .. 02073df0, 0x508-byte state).
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

extern void Ov008_MissionListInitStep(void);
extern void Ov008_MissionListDestroy(void);
extern void Ov008_PanelRebuild(void);
extern void Ov008_MissionListKeyUp(void);
extern void Ov008_MissionListKeyDown(void);
extern void Ov008_TickIdleReaction_Down(void);
extern void Ov008_TickIdleReaction_Up(void);
extern void Ov008_MissionListConfirm(void);
extern void Ov008_MissionListBack(void);
extern void Ov008_RefreshHoveredCell(void);
extern void Ov008_RefreshHoveredRow(void);

Ov008PageHooks data_ov008_0209082c __attribute__((aligned(__alignof__(Ov008PageHooks)))) = {
    Ov008_MissionListInitStep,  /* pfnOpen */
    Ov008_MissionListDestroy,  /* pfnStep */
    Ov008_PanelRebuild,  /* pfnClose */
    1288,  /* nStateSize */
    { Ov008_MissionListKeyUp, Ov008_MissionListKeyDown, Ov008_TickIdleReaction_Down, Ov008_TickIdleReaction_Up, Ov008_MissionListConfirm, Ov008_MissionListBack, 0, 0, Ov008_RefreshHoveredCell, Ov008_RefreshHoveredRow, 0, Ov008_MissionListBack },  /* apfnHook */
};
