/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_RunPendingHitCallback.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Returns while bit 9 of the 64-bit flag word at obj[0] is set. Otherwise, when bits 2 and 0 of the
 * byte at obj+0x694 are both set, runs func_ov022_0209c9fc(obj). Always clears bit 2 of that byte
 * on the way out. */

extern void func_ov022_0209c9fc(int obj);

struct Bits020a06bc { unsigned char b0 : 1; unsigned char b1 : 1; unsigned char b2 : 1; };

void Ov022_RunPendingHitCallback(int obj) {
    if ((*(kh_unaligned_u64 *)obj & 0x200LL) != 0) return;
    if (((struct Bits020a06bc *)(obj + 0x694))->b2 &&
        ((struct Bits020a06bc *)(obj + 0x694))->b0)
        func_ov022_0209c9fc(obj);
    *(unsigned char *)(obj + 0x694) &= ~4;
}
