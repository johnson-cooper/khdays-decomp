/* PS2: mechanically prepared copy of src/overlays/scenes/ov005_mission_result/Ov005_LatchTickState7.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* When the gate word at +0x4c38 is set, samples the 64-bit timer through OS_GetTick into
 * the pair at +0x4bf4 and moves the state word at +0x4bf0 to 7.
 *
 * OS_GetTick returns a 64-bit value in r0/r1 -- writing it as one `long long` store is
 * what gives the ROM's two adjacent `str`s. */
extern long long OS_GetTick(void);
extern char *data_ov005_0205b80c;

void Ov005_LatchTickState7(void) {
    char *b = data_ov005_0205b80c;
    long long t;
    if (*(int *)(b + 0x4c38) == 0) {
        return;
    }
    t = OS_GetTick();
    *(kh_unaligned_s64 *)(b + 0x4bf4) = t;
    *(int *)(b + 0x4bf0) = 7;
}
