/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_NotifyRowsAndFlag.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Raise bit 2 of the header's flag byte, then walk its rows (0xc apart, count at +0x34) and for
 * each one hand the row's object to Ov022_SetAnimSpeed together with the animation step at +0x2aba,
 * and set bit 10 of the object's 64-bit flag word.
 *
 * Parked as a "base-coalescing tie": the original keeps the header pointer in its own register
 * and re-reads the loop bound through it, while mwcc folded the header into the walking pointer
 * and hoisted the invariant address. Two levers, both already established:
 *   - reach the fields through STRUCTS, which is what stops the coalescing;
 *   - the loop-body temp `p` is declared FIRST, at the top of the function. Declaration order
 *     hands out the callee-saved registers, and a temp left inside the loop body is allocated
 *     after the outer locals -- hoisting it moves the header pointer from r4 up to r7, which is
 *     where the original keeps it. */
extern int data_ov022_020b2e78[];
extern void Ov022_SetAnimSpeed(int obj, int arg);

struct Ov022Hdr {
    char _pad0[0x34];
    unsigned char bCount;       /* +0x34 */
    char _pad35[7];
    unsigned char bFlags;       /* +0x3c */
};
struct Ov022Row {
    int _a;
    int pObj;                   /* +4 */
    int _c;
};

void Ov022_NotifyRowsAndFlag(void) {
    unsigned int *p;
    int i;
    struct Ov022Row *row;
    struct Ov022Hdr *hdr;

    hdr = (struct Ov022Hdr *)data_ov022_020b2e78[1];
    if (hdr == 0) {
        return;
    }
    i = 0;
    hdr->bFlags |= 4;
    if (0 < (int)hdr->bCount) {
        row = (struct Ov022Row *)hdr;
        do {
            p = *(unsigned int **)(row->pObj + 0x20);
            Ov022_SetAnimSpeed((int)p, *(short *)((int)p + 0x2aba));
            *(kh_unaligned_u64 *)p |= 0x400;
            i = i + 1;
            row = row + 1;
        } while (i < (int)hdr->bCount);
    }
}
