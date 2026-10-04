/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_TryAdvanceAndClearBit13.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* When Session_GetLocalPlayerIndex reports idle, raises bit 15 of the 64-bit flag word at
 * obj+0x464. Unless bit 13 of the flag word at obj[0] is set, advances the state via
 * Ov022_ActorSetState with mode 0 or 2 depending on bit 2 of obj+0x24. On success clears bit 13 of
 * the obj[0] flag word and returns the Ov022_ActorSetState result. */

#include "game/engine.h"

extern int Ov022_ActorSetState(int obj, int mode);

int Ov022_TryAdvanceAndClearBit13(int obj) {
    int r = 0;
    if (Session_GetLocalPlayerIndex() == 0)
        *(kh_unaligned_u64 *)(obj + 0x464) |= 0x8000LL;
    if ((*(kh_unaligned_u64 *)obj & 0x2000LL) == 0) {
        if ((*(unsigned int *)(obj + 0x24) & 4) != 0) r = Ov022_ActorSetState(obj, 0);
        else r = Ov022_ActorSetState(obj, 2);
    }
    if (r != 0)
        *(kh_unaligned_u64 *)obj &= ~0x2000LL;
    return r;
}
