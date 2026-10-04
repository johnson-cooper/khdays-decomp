/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_IsSlotActionAllowed.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Validates a slot action: false if idx is -1, if func_ov022_0209029c rejects the slot, or if the
 * 2D table lookup Load2DArrayU8(kind, idx) is <= 0. Then a jump table on the state byte obj[2]
 * allows states 0 and >=10 (result 1) and denies 1..9. A final gate forces 0 when bit 4 or bit 13
 * of the 64-bit flag word at *(obj+0x58) is set. */

#include "game/engine.h"

extern int func_ov022_0209029c(int obj, unsigned int idx);

int Ov022_IsSlotActionAllowed(int obj, unsigned int idx) {
    int *flags = *(int **)(obj + 0x58);
    int r = 1;
    if (idx == 0xffffffff) return 0;
    if (func_ov022_0209029c(obj, idx) == 0) return 0;
    if (Load2DArrayU8(*(unsigned char *)(*(int *)(obj + 0x58) + 9), idx) <= 0) return 0;
    switch (*(unsigned char *)(obj + 2)) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 9:
        r = 0;
        goto end;
    case 5:
    case 6:
    case 7:
    case 8:
        r = 0;
        break;
    case 0:
    default:
        break;
    }
end:
    if ((*(kh_unaligned_u64 *)flags & 0x10LL) != 0 ||
        (*(kh_unaligned_u64 *)flags & 0x2000LL) != 0)
        r = 0;
    return r;
}
