/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_MarshalEntrySnapshot.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Copies a snapshot of entry `src` (from GetEntryField20ByIndex) into the idx'th 0x44-byte row of
 * the table at data_0204c3d8. Bit 0 of the row's flag byte at +0x24 takes bit 38 of src's 64-bit
 * flag word, bit 1 takes bit 37; the bytes at +0x25/+0x26 and the two words at +0x28/+0x2C are
 * copied straight from src+0x2BB0/+0x2770/+0x2778/+0x2774. */

#include "game/engine.h"

extern int data_0204c3d8;

struct Row02087630 {
    char pad0[0x24];
    char b0 : 1;
    char b1 : 1;
    char brest : 6;
    unsigned char b25;
    signed char b26;
    char pad27;
    int w28;
    int w2c;
    char pad30[0x14];
};

void Ov022_MarshalEntrySnapshot(int idx) {
    int src = GetEntryField20ByIndex(idx);
    struct Row02087630 *e = (struct Row02087630 *)&data_0204c3d8 + idx;
    e->b0 = (*(kh_unaligned_u64 *)(src) & 0x4000000000LL) != 0;
    e->b1 = (*(kh_unaligned_u64 *)(src) & 0x2000000000LL) != 0;
    e->b25 = *(unsigned char *)(src + 0x2bb0);
    e->b26 = *(signed char *)(src + 0x2770);
    e->w28 = *(int *)(src + 0x2778);
    e->w2c = *(int *)(src + 0x2774);
}
