/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/Ov000_BeginSaveCheck.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Ov000_BeginSaveCheck -- poll the logo intro sequence, ov000. While the intro step
 * counter (scene block @+0x4b08) is below 3, advances Ov000_PollSaveCheck; on its
 * result 3 it sets the "done" flag (@+0x6a48). Otherwise (or on completion) it stamps
 * the current tick (@+0x4ae4) and moves to phase 8 (@+0x4ad0). */
extern char *data_ov000_0205ac24;
extern int  Ov000_PollSaveCheck(void);
extern unsigned long long OS_GetTick(void);
void Ov000_BeginSaveCheck(void) {
    if (*(int *)(data_ov000_0205ac24 + 0x4b08) < 3) {
        int r = Ov000_PollSaveCheck();
        if (r < 0) {
            return;
        }
        if (r == 3) {
            *(int *)(data_ov000_0205ac24 + 0x6a48) =
                (*(int *)(data_ov000_0205ac24 + 0x6a48) & -0x10000) | 1;
            return;
        }
    }
    kh_write_u64_le_unaligned(data_ov000_0205ac24 + 0x4ae4, OS_GetTick());
    *(int *)(data_ov000_0205ac24 + 0x4ad0) = 8;
}
