/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_TryAdvanceAndClearBit11.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* When Session_GetLocalPlayerIndex reports idle, raises bit 33 then bit 38 of the 64-bit flag word
 * at obj+0x464, and writes 0x3000 to obj+0x4B4. Unless bit 11 of the obj[0] flag word is set,
 * advances the state via Ov022_ActorSetState(obj, 0); on success clears bit 11 and returns the
 * result. */

#include "game/engine.h"

extern int Ov022_ActorSetState(int obj, int mode);

int Ov022_TryAdvanceAndClearBit11(int obj) {
    int r = 0;
    if (Session_GetLocalPlayerIndex() == 0)
        *(kh_unaligned_u64 *)(obj + 0x464) |= 0x200000000LL;
    if (Session_GetLocalPlayerIndex() == 0)
        *(kh_unaligned_u64 *)(obj + 0x464) |= 0x4000000000LL;
    *(int *)(obj + 0x4b4) = 0x3000;
    if ((*(kh_unaligned_u64 *)obj & 0x800LL) == 0) r = Ov022_ActorSetState(obj, 0);
    if (r != 0)
        *(kh_unaligned_u64 *)obj &= ~0x800LL;
    return r;
}
