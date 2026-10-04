/* PS2: mechanically prepared copy of src/overlays/field/ov017_field_deposits/Ov017_SetFlagBit62OnFour.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Sets flag bit 62 on the four players' actors; returns 1. */

extern int *GetEntryField20ByIndex(int i);

int Ov017_SetFlagBit62OnFour(void) {
    int i;
    for (i = 0; i < 4; i++) {
        int *p = GetEntryField20ByIndex(i);
        if (p) {
            *(kh_unaligned_u64 *)p |= 0x4000000000000000ULL;
        }
    }
    return 1;
}
