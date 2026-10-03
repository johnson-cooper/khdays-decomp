/* PS2: mechanically prepared copy of src/overlays/scenes/ov009_camp_save/data/ov009_handlers_020562f4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov009 .data 0x020562f4-0x02056338: the ov009 menu's callback block -- three entry points
 * (02053c18, 02053e24, 02053ec8), a -1 slot and the 0x1f04 flag word -- followed by the twelve
 * per-state handlers of its state machine (02053770 .. 02053c14; 02053c14 is the shared no-op
 * state, 02053af0 appears twice). */

typedef void (*Ov009Fn)(void);

extern void Ov009_TickSlotScanState(void);
extern void Ov009_TeardownScene(void);
extern void Ov009_TickSaveCommitState(void);
extern void Ov009_StepSelectionBackward(void);
extern void Ov009_StepSelectionForward(void);
extern void Ov009_MenuElementRelease(void);
extern void Ov009_MenuElementPress(void);
extern void Ov009_TickSaveConfirmState(void);
extern void Ov009_TickSaveCancelState(void);
extern void Ov009_MenuStateNoOp(void);

struct {
    Ov009Fn entry[3];
    int nSlot;
    int nFlags;
    Ov009Fn state[12];
} data_ov009_020562f4 __attribute__((aligned(4))) = {
    { Ov009_TickSlotScanState, Ov009_TeardownScene, Ov009_TickSaveCommitState },
    -1,
    0x1f04,
    {
        Ov009_StepSelectionBackward, Ov009_StepSelectionForward, Ov009_MenuElementRelease, Ov009_MenuElementPress,
        Ov009_TickSaveConfirmState, Ov009_TickSaveCancelState, Ov009_MenuStateNoOp, Ov009_MenuStateNoOp,
        Ov009_MenuStateNoOp, Ov009_MenuStateNoOp, Ov009_MenuStateNoOp, Ov009_TickSaveCancelState,
    },
};
