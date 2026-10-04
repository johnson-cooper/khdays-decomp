/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_ResolveGuardBreakState.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* After ToggleBit13ByMode(obj,0): when obj+0x24 bit 2 is set, if obj[0x120] is positive or
 * obj[0x58] != 0x80000000 sets bit 25 of the obj[0] flag word and advances state 5, else clears bit
 * 25 and advances state 4. When bit 2 is clear, sets/clears bit 25 by whether obj[0x120] is
 * non-zero (also writing 0x400 to obj+0x58 when zero) and advances state 5. Returns what
 * Ov022_ActorSetState returns. */

extern void Ov022_ToggleBit13ByMode(int obj, int mode);
extern int Ov022_ActorSetState(int obj, int mode);

int Ov022_ResolveGuardBreakState(int obj) {
    Ov022_ToggleBit13ByMode(obj, 0);
    if ((*(unsigned int *)(obj + 0x24) & 4) != 0) {
        if ((int)*(unsigned int *)(obj + 0x480) <= 0 &&
            *(unsigned int *)(obj + 0x58) == 0x80000000) {
            *(kh_unaligned_u64 *)obj &= ~0x2000000LL;
            Ov022_ActorSetState(obj, 4);
            return;
        }
        *(kh_unaligned_u64 *)obj |= 0x2000000LL;
        Ov022_ActorSetState(obj, 5);
        return;
    }
    if (*(unsigned int *)(obj + 0x480) != 0) {
        *(kh_unaligned_u64 *)obj |= 0x2000000LL;
    } else {
        *(kh_unaligned_u64 *)obj &= ~0x2000000LL;
        *(int *)(obj + 0x58) = 0x400;
    }
    return Ov022_ActorSetState(obj, 5);
}
