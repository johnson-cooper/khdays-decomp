/* PS2: mechanically prepared copy of src/engine/Obj_CommitAlphaBlend.c (ps2/tools/prep_sources.py). Do not edit. */
/* Obj_CommitAlphaBlend -- commit an object's alpha-blend level to the display MMIO, MAIN.
 * entry = &objTable[idx] (0x8c-byte stride, +4 header). When the shared blend counter
 * base[0x4628] is below full (0x10), packs it into a BLDALPHA coefficient pair
 * (EVA=count, EVB=0x10-count) and writes it to the sub engine (0x04001052) when
 * base[0x4604]==2, else the main engine (0x04000052). Then, per the entry's flag bits at
 * +0x78: bit2 -> DispObj_WriteOam(base,entry); bit1 -> NNS_G2dTickCellAnimation(&entry[0x14], 0x1000). */

#include "nitro/types.h"

struct Flags { unsigned f0 : 1, f1 : 1, f2 : 1; };
extern void DispObj_WriteOam(int base, int entry);
extern void NNS_G2dTickCellAnimation(unsigned int *p, int size);

void Obj_CommitAlphaBlend(void *pBase, int idx) {
    int base = (int)pBase;
    int base4 = base + 4;
    int entry = base4 + idx * 0x8c;
    unsigned int count;
    if (idx < 0) {
        return;
    }
    count = *(unsigned int *)(base + 0x4628);
    if ((int)count < 0x10) {
        if (*(int *)(base + 0x4604) == 2) {
            *(vu16 *)((unsigned int)kh_ds_io + 0x1052) = (unsigned short)(count | (0x10 - count) * 0x100);
        } else {
            *(vu16 *)((unsigned int)kh_ds_io + 0x52) = (unsigned short)(count | (0x10 - count) * 0x100);
        }
    }
    if (((struct Flags *)(entry + 0x78))->f2) {
        DispObj_WriteOam(base, entry);
    }
    if (((struct Flags *)(entry + 0x78))->f1) {
        NNS_G2dTickCellAnimation((unsigned int *)(entry + 0x14), 0x1000);
    }
}
