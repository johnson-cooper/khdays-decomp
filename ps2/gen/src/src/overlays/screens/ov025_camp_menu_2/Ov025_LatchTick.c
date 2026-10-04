/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/Ov025_LatchTick.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Records the current OS tick in a global. */

extern long long OS_GetTick();
extern int data_0204be1c;

void Ov025_LatchTick(void) {
    *(kh_unaligned_s64 *)&data_0204be1c = OS_GetTick();
}
