/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/Ov025_GetLatchedTick.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Return the 64-bit value stored at data_0204be1c. */

extern int data_0204be1c;

long long Ov025_GetLatchedTick(void) {
    return *(kh_unaligned_s64 *)&data_0204be1c;
}
