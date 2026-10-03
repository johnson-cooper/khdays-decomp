/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_page_020b5310.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 page hooks data_ov025_020b5310, 0x020b5310-0x020b5350 (.data): the mission list page (020a8b28 / 020a8d50 / 020a8f58, 0x508-byte Ov008MissionList; back Ov025_MissionList_Back 020a92b0, cursor Ov025_MissionList_CursorPrev / Next 020a93a0 / 020a93fc).
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

extern void Ov025_MissionListInitStep(void);
extern void Ov025_MissionListDestroy(void);
extern void Ov025_PanelRebuild(void);
extern void Ov025_MissionListKeyUp(void);
extern void Ov025_MissionListKeyDown(void);
extern void Ov025_TickIdleReaction_Down(void);
extern void Ov025_TickIdleReaction_Up(void);
extern void Ov025_MissionListConfirm(void);
extern void Ov025_MissionList_Back(void);
extern void Ov025_MissionList_CursorPrev(void);
extern void Ov025_MissionList_CursorNext(void);

Ov008PageHooks data_ov025_020b5310 __attribute__((aligned(__alignof__(Ov008PageHooks)))) = {
    Ov025_MissionListInitStep,  /* pfnOpen */
    Ov025_MissionListDestroy,  /* pfnStep */
    Ov025_PanelRebuild,  /* pfnClose */
    1288,  /* nStateSize */
    { Ov025_MissionListKeyUp, Ov025_MissionListKeyDown, Ov025_TickIdleReaction_Down, Ov025_TickIdleReaction_Up, Ov025_MissionListConfirm, Ov025_MissionList_Back, 0, 0, Ov025_MissionList_CursorPrev, Ov025_MissionList_CursorNext, 0, Ov025_MissionList_Back },  /* apfnHook */
};
