/* PS2: mechanically prepared copy of src/overlays/scenes/ov009_camp_save/Ov009_GetLatchedTick.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Return the 64-bit value stored at data_0204be1c. */
extern int data_0204be1c;
long long Ov009_GetLatchedTick(void) {
    return *(kh_unaligned_s64 *)&data_0204be1c;
}
