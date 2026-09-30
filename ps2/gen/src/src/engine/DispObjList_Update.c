/* PS2: mechanically prepared copy of src/engine/DispObjList_Update.c (ps2/tools/prep_sources.py). Do not edit. */
/* DispObjList_Update -- per-frame update of an object list, MAIN. When the shared blend is enabled
 * (base[0x4624]), commits the blend counter base[0x4628] as a BLDALPHA pair (EVA=count,
 * EVB=0x10-count) to the sub engine (0x04001052) when base[0x4604]==2, else to the main engine
 * (0x04000052). Then walks the object chain from base[0] (next at +4): flag bit2 at +0x78 ->
 * DispObj_WriteOam(base,node); bit1 -> NNS_G2dTickCellAnimation(&node[0x14], 0x1000). Finally clears
 * base[0x4634] and hands over to OamBuffer_Flush(base, arg). */

#include "nitro/types.h"

#define REG_BLDALPHA    (*(vu16 *)((unsigned int)kh_ds_io + 0x52))
#define REG_DB_BLDALPHA (*(vu16 *)((unsigned int)kh_ds_io + 0x1052))

static inline void G2_ChangeBlendAlpha(int eva, int evb)
{
    REG_BLDALPHA = (unsigned short)(eva | evb << 8);
}

static inline void G2S_ChangeBlendAlpha(int eva, int evb)
{
    REG_DB_BLDALPHA = (unsigned short)(eva | evb << 8);
}
struct Flags { unsigned f0 : 1, f1 : 1, f2 : 1; };
extern void DispObj_WriteOam(int base, int entry);
extern void NNS_G2dTickCellAnimation(unsigned int *p, int size);
extern void OamBuffer_Flush(void *p, int x);

void DispObjList_Update(int base, int arg)
{
    int node;

    if (*(int *)(base + 0x4624) != 0) {
        if (*(int *)(base + 0x4604) == 2) {
            G2S_ChangeBlendAlpha(*(int *)(base + 0x4628), 0x10 - *(int *)(base + 0x4628));
        } else {
            G2_ChangeBlendAlpha(*(int *)(base + 0x4628), 0x10 - *(int *)(base + 0x4628));
        }
    }
    for (node = *(int *)base; node != 0; node = *(int *)(node + 4)) {
        if (((struct Flags *)(node + 0x78))->f2) {
            DispObj_WriteOam(base, node);
        }
        if (((struct Flags *)(node + 0x78))->f1) {
            NNS_G2dTickCellAnimation((unsigned int *)(node + 0x14), 0x1000);
        }
    }
    *(int *)(base + 0x4634) = 0;
    OamBuffer_Flush((void *)base, arg);
}
