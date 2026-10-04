/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_ToggleBit13ByMode.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
extern int Ov022_IsState9Or6WithFlag200(int a);

/* Struct-field form keeps the direct `ldr [obj,#0x468]` for the high half. */
struct Flags020ad8e0 { char pad0[0x464]; unsigned long long f464; };

void Ov022_ToggleBit13ByMode(int obj, int mode) {
    if (mode != 0) {
        if (*(int *)(obj + 0x4b4) != 0) return;
        if ((*(kh_unaligned_u64 *)obj & 0x80000LL) != 0) return;
        if ((*(kh_unaligned_u64 *)obj & 0x1000000LL) != 0) return;
        if ((((struct Flags020ad8e0 *)obj)->f464 & 0x10000LL) != 0) return;
        if ((((struct Flags020ad8e0 *)obj)->f464 & 0x8000000000LL) != 0) return;
        if (Ov022_IsState9Or6WithFlag200(obj + 0x22f8) != 0) return;
        *(kh_unaligned_u64 *)obj |= 0x2000LL;
        return;
    }
    *(kh_unaligned_u64 *)obj &= ~0x2000LL;
}
