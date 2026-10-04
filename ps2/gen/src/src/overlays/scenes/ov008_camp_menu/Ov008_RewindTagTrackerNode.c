/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_RewindTagTrackerNode.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Ov008_RewindTagTrackerNode -- (re)arm an animation node: zero its accumulator
 * (+0x14/+0x18), stamp the current time (+0x1c), rewind to frame 0 (+4) and hand
 * that frame to the resolver. See Ov008_TickTagTrackerNodes for the ticker. */
extern long long OS_GetTick(void);
extern void Ov008_TagTracker_InvokeCallback(int owner, int tag);

void Ov008_RewindTagTrackerNode(int param_1, int param_2) {
    *(int *)(param_2 + 0x14) = 0;
    *(int *)(param_2 + 0x18) = 0;
    *(kh_unaligned_s64 *)(param_2 + 0x1c) = OS_GetTick();
    *(unsigned short *)(param_2 + 4) = 0;
    Ov008_TagTracker_InvokeCallback(param_1,
        *(int *)(*(int *)(param_2 + 0x2c) + *(unsigned short *)(param_2 + 4) * 4));
}
