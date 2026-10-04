/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_SetTagTrackerNodeArmed.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Ov008_SetTagTrackerNodeArmed -- arm or disarm an animation node (see Ov008_TickTagTrackerNodes
 * for the ticker that consumes these). Arming a node that was not already armed
 * resets its accumulator (+0x14/+0x18), stamps the current time (+0x1c) and
 * re-resolves its selected frame. Bit 0 of node+0x24 records the armed state. */
extern long long OS_GetTick(void);
extern void Ov008_TagTracker_InvokeCallback(int owner, int tag);

struct NodeFlags { unsigned char armed : 1, visible : 1; };

void Ov008_SetTagTrackerNodeArmed(int param_1, int param_2, int param_3) {
    char armed = 0;
    if (param_3 != 0) {
        armed = 1;
        if (((struct NodeFlags *)(param_2 + 0x24))->armed != 1) {
            *(int *)(param_2 + 0x14) = 0;
            *(int *)(param_2 + 0x18) = 0;
            *(kh_unaligned_s64 *)(param_2 + 0x1c) = OS_GetTick();
            Ov008_TagTracker_InvokeCallback(param_1,
                *(int *)(*(int *)(param_2 + 0x2c) + *(unsigned short *)(param_2 + 4) * 4));
        }
    }
    ((struct NodeFlags *)(param_2 + 0x24))->armed = armed;
}
