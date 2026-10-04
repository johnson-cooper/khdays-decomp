/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_FindFreeSlotId.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Scan the 64-bit "slot in use" bitmap at +0x58 for the first free slot (bit clear); return its
 * id (index + 0x65), or -1 if all 40 slots are taken. */
int Ov008_FindFreeSlotId(int obj) {
    unsigned int i = 0;
    do {
        if ((*(kh_unaligned_s64 *)(obj + 0x58) & (1LL << i)) == 0) {
            return i + 0x65;
        }
        i++;
    } while ((int)i < 0x28);
    return -1;
}
