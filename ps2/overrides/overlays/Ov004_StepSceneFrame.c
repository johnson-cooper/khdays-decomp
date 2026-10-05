/*
 * Faithful PS2 calendar frame with hardware breadcrumbs around each call.
 *
 * This is deliberately the same control flow as the canonical ov004 routine.
 * The marks do not alter calendar state; they let the watchdog identify the
 * exact callee that failed to return instead of merely reporting the phase of
 * the last frame that reached the display.
 *
 * Phase timing (verified against the canonical routine): the handler is picked
 * from the phase at the START of the frame.  On the frame where
 * Ov004_FadeOutTransition (phase 3) sees its time elapse it writes phase 4, and
 * the rest of that frame still scales, draws and is presented; handler 4
 * (Ov004_MarkTransitionComplete, +0x5550 = 1) first runs on the NEXT frame.  So
 * "in=3 ph=4 done=0" is the normal last fade-out frame, not corrupt state.  The
 * protected parent (class 8/group 15) is linked before this child (same class
 * key, Obj_LinkNode appends), so it reads done=1 one frame later still, requests
 * SCENE_FIELD and returns -2; BootTask (key 0, first in the list) tears ov004
 * down on the pass after that.
 */

#include "platform/kh_platform.h"

typedef void (*Ov004StateHandler)(void);

typedef struct Ov004StateHandlerTable {
    Ov004StateHandler handlers[5];
} Ov004StateHandlerTable;

extern const Ov004StateHandlerTable data_ov004_020510b8;
extern char *data_ov004_02051384;

extern void Ov004_StepLogoScale(void);
extern void Ov004_PrepareTransitionLabel(void);
extern void Ov004_StepLogoSlide(void);
extern void Ov004_StepFadeIn(void);
extern void Ov004_SubmitObjectSprites(void);
extern void Ov004_DrawRollingDigits(void);
extern void kh_debug_mark(const char *stage, int a, int b);
extern unsigned long long OS_GetTick(void);
extern void kh_debug_calendar_state(int phase, int gate, int position, int elapsed,
                                    int phase_frame, int complete);
extern void kh_debug_calendar_entry(int phase);

int Ov004_StepSceneFrame(void)
{
    Ov004StateHandlerTable table;
    int phase;
    int transitionState;

    table = data_ov004_020510b8;
    phase = *(int *)(data_ov004_02051384 + 0xaf8);
    transitionState = *(int *)(data_ov004_02051384 + 0x5584);

    kh_debug_calendar_entry(phase);
    kh_debug_mark("calendar call: phase handler", phase, transitionState);
    table.handlers[phase]();

    phase = *(int *)(data_ov004_02051384 + 0xaf8);
    kh_debug_mark("calendar call: logo scale", phase, transitionState);
    Ov004_StepLogoScale();

    phase = *(int *)(data_ov004_02051384 + 0xaf8);
    transitionState = *(int *)(data_ov004_02051384 + 0x5584);
    switch (phase) {
    case 1:
        if (transitionState == 0)
            break;
        /* fall through */
    case 2:
    case 3:
        kh_debug_mark("calendar call: label", phase, transitionState);
        Ov004_PrepareTransitionLabel();
        kh_debug_mark("calendar call: logo slide", phase, transitionState);
        Ov004_StepLogoSlide();
        kh_debug_mark("calendar call: label fade", phase, transitionState);
        Ov004_StepFadeIn();
        break;
    }

    kh_debug_mark("calendar call: sprites", phase, transitionState);
    Ov004_SubmitObjectSprites();
    kh_debug_mark("calendar call: digits", phase, transitionState);
    Ov004_DrawRollingDigits();
    phase = *(int *)(data_ov004_02051384 + 0xaf8);
    transitionState = *(int *)(data_ov004_02051384 + 0x5584);
    kh_debug_calendar_state(
        phase,
        transitionState,
        *(int *)(data_ov004_02051384 + 0x5588),
        (int)(OS_GetTick() - *(unsigned long long *)(data_ov004_02051384 + 0xb00)),
        *(int *)(data_ov004_02051384 + 0xafc),
        *(int *)(data_ov004_02051384 + 0x5550));
    kh_debug_mark("calendar frame: returned", phase,
                  transitionState);
    return 0;
}
