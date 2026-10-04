/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_ExpireTimerThenNextStep.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Re-enables actor input, and once the context's timer has run for its duration returns to the hub
 * step (clearing the timer); does nothing during replay. */

extern void Ov022_UpdateCameraAndViews(int a);
extern unsigned long long OS_GetTick(void);
extern void Ov022_StateReturnToHub(void);
extern int data_0204be04;
extern int data_ov022_020b2e60;

int Ov022_ExpireTimerThenNextStep(void) {
    int g = data_ov022_020b2e60;
    int r = 0;
    if (*(unsigned char *)&data_0204be04 != 0) return r;
    Ov022_UpdateCameraAndViews(1);
    if (*(kh_unaligned_u64 *)(g + 0x24) + 0x17f898 <= OS_GetTick()) {
        *(int *)(g + 0x1c) = 0;
        r = (int)Ov022_StateReturnToHub;
        *(int *)(g + 0x20) = 0;
    }
    return r;
}
