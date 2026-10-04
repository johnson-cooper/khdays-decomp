/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_ArmChargeIfReady.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Host only: when a charge is requested and the actor is free to act, arms the charge with its
 * parameter; always clears the request. */

#include "game/engine.h"

struct Flags020ad358 { char pad0[0x464]; unsigned long long f464; };

void Ov022_ArmChargeIfReady(int obj, int param_2) {
    struct Flags020ad358 *fp = (struct Flags020ad358 *)obj;
    int ok = 1;
    if (Session_GetLocalPlayerIndex() != 0) return;
    if ((fp->f464 & 0x100LL) != 0 || (fp->f464 & 0x8000LL) != 0 ||
        (fp->f464 & 0x10000000LL) != 0)
        ok = 0;
    if ((*(kh_unaligned_u64 *)obj & 0x2000LL) != 0) ok = 0;
    if ((*(kh_unaligned_u64 *)obj & 0x200000000LL) == 0) ok = 0;
    if (ok) {
        *(kh_unaligned_u64 *)obj &= ~0x200000000LL;
        *(kh_unaligned_u64 *)obj |= 0x10000000LL;
        *(int *)(obj + 0x690) = param_2;
        return;
    }
    *(kh_unaligned_u64 *)obj &= ~0x200000000LL;
}
