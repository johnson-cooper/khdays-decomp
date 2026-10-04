/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_EmitEventBlocks.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Gated on the byte at obj+0x26C4 and flags at obj+0x26BC (bits 0/2 clear). If bit 4 is set emits
 * event kind 7; else (bit 3 clear) emits kinds 0 and, when bit 1 is set, 2 -- all via
 * Ov022_MarshalNetworkRecord(size 0x1000) -- then raises bit 22 of the 64-bit flag word at obj[0].
 */

extern void Ov022_MarshalNetworkRecord(int obj, int kind, int *buf, int size, unsigned int a, int b);
void Ov022_EmitEventBlocks(int obj, int param_2, unsigned int param_3, int *param_4) {
    unsigned int f;
    if (*(unsigned char *)(obj + 0x26c4) == 0) return;
    f = *(unsigned int *)(obj + 0x26bc);
    if ((f & 5) != 0) return;
    if ((f & 0x10) != 0) {
        Ov022_MarshalNetworkRecord(obj, 7, param_4, 0x1000, param_3, 0);
        return;
    }
    if ((f & 8) == 0) {
        Ov022_MarshalNetworkRecord(obj, 0, param_4, 0x1000, param_3, param_2);
        if ((*(unsigned int *)(obj + 0x26bc) & 2) != 0)
            Ov022_MarshalNetworkRecord(obj, 2, param_4, 0x1000, param_3, param_2);
    }
    *(kh_unaligned_u64 *)obj |= 0x400000LL;
}
