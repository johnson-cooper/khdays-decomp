/* PS2 debug override: trace the new-game handoff after the difficulty UI. */
#include "platform/kh_platform.h"

extern void kh_debug_stage(const char *stage, int a, int b);
extern int  NNSi_FndGetCurrentRootHeap(void);
extern void PartyState_Reset(int heap);
extern int  Ov000_BootRunSelector(void);
extern int  Ov000_RequestTitleTransition(void);
extern void Ov000_LatchTick(void);

int Ov000_BootDispatch(void)
{
    int result = -2;
    int heap = NNSi_FndGetCurrentRootHeap();
    int mode = *(char *)(heap + 0x4c31);

    kh_debug_stage("boot dispatch: entered", mode, 0);
    kh_debug_stage("boot dispatch: PartyState_Reset", mode, 0);
    PartyState_Reset(heap);
    kh_debug_stage("boot dispatch: reset returned", mode, 0);

    switch (mode) {
    case 0:
        kh_debug_stage("boot dispatch: BootRunSelector", mode, 0);
        result = Ov000_BootRunSelector();
        kh_debug_stage("boot dispatch: selector returned", result, 0);
        break;
    case 1:
        kh_debug_stage("boot dispatch: RequestTitleTransition", mode, 0);
        result = Ov000_RequestTitleTransition();
        kh_debug_stage("boot dispatch: title transition returned", result, 0);
        break;
    }

    kh_debug_stage("boot dispatch: LatchTick", result, mode);
    Ov000_LatchTick();
    kh_debug_stage("boot dispatch: complete", result, mode);
    return result;
}
