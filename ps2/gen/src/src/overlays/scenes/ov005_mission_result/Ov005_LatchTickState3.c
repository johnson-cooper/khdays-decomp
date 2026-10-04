/* PS2: mechanically prepared copy of src/overlays/scenes/ov005_mission_result/Ov005_LatchTickState3.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* When the gate word at +0x4b7c is set, samples the 64-bit timer through OS_GetTick into
 * the pair at +0x4b5c and moves the state word at +0x4b74 to 3.
 *
 * OS_GetTick returns a 64-bit value in r0/r1 -- writing it as one `long long` store is
 * what gives the ROM's two adjacent `str`s. */
extern long long OS_GetTick(void);
extern char *data_ov005_0205b810;

void Ov005_LatchTickState3(void) {
    char *b = data_ov005_0205b810;
    long long t;
    if (*(int *)(b + 0x4b7c) == 0) {
        return;
    }
    t = OS_GetTick();
    *(kh_unaligned_s64 *)(b + 0x4b5c) = t;
    *(int *)(b + 0x4b74) = 3;
}
