/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_RequestGuardBreak.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Raises bit 23 of the 64-bit flag word at obj[0] -- the guard-break request -- only when
 * Ov022_IsInputAllowedForActiveSlot reports the slot idle, none of bits 24, 8, 17, 23 or 13 are
 * already set, and the state byte at obj+0x2770 is not 3, 11 or 2. The whole flag word is loaded
 * once and reused by all five mask tests. */

extern int Ov022_IsInputAllowedForActiveSlot(void);

void Ov022_RequestGuardBreak(int obj) {
    unsigned long long f;
    int state;
    if (Ov022_IsInputAllowedForActiveSlot() != 0) return;
    f = *(kh_unaligned_u64 *)obj;
    if ((f & 0x1000000LL) != 0) return;
    if ((f & 0x100LL) != 0) return;
    if ((f & 0x20000LL) != 0) return;
    if ((f & 0x800000LL) != 0) return;
    if ((f & 0x2000LL) != 0) return;
    state = *(signed char *)(obj + 0x2770);
    if (state == 3 || state == 0xb || state == 2) return;
    *(kh_unaligned_u64 *)obj = f | 0x800000LL;
}
