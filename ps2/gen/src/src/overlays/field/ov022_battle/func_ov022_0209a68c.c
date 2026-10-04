/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/func_ov022_0209a68c.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/*
 * Set (arg1!=0) or clear (arg1==0) bit 0x200 of a 64-bit flag word at arg0[0..1], after notifying
 * Entity_SetVisible with a 0/1 flag. The operation is 64-bit: the low word gets `| 0x200` / `& ~0x200`,
 * and the high word the identity `| 0` / `& ~0` -- which is why the ROM keeps the otherwise-dead
 * arg0[1] store, and shares the `mvn` (~0) between the ~0x200 low mask and the ~0 high mask.
 */
extern void Entity_SetVisible(int index, int flag);

void func_ov022_0209a68c(unsigned int *arg0, int arg1) {
    if (arg1 != 0) {
        Entity_SetVisible((int)*(char *)((int)arg0 + 0x12f * 4), 0);
        *(kh_unaligned_u64 *)arg0 |= 0x200;
        return;
    }
    Entity_SetVisible((int)*(char *)((int)arg0 + 0x12f * 4), 1);
    *(kh_unaligned_u64 *)arg0 &= ~(unsigned long long)0x200;
}
