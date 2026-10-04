/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_CanTriggerFinisher.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
extern int func_ov022_02095450(int a);
extern int Ov022_IsState9Or6WithFlag200(int a);

/* The struct-field form keeps the direct `ldr [obj,#0x468]` for the high half. */
struct Flags02095d10 { char pad0[0x464]; unsigned long long flags; };

int Ov022_CanTriggerFinisher(int obj) {
    unsigned long long f = ((struct Flags02095d10 *)obj)->flags;
    int ok = ((f & 8LL) != 0);
    if (ok && (*(kh_unaligned_u64 *)obj & 0x210002010LL) != 0) ok = 0;
    if (ok && (f & 0x10100LL) != 0) ok = 0;
    if (ok && func_ov022_02095450(obj + 0x22f8) != 0 &&
        Ov022_IsState9Or6WithFlag200(obj + 0x22f8) == 0) ok = 0;
    return ok;
}
