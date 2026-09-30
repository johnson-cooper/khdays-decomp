/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_CommitDisplayState.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Ov002_CommitDisplayState - push the pending display changes to the hardware.
 *
 * Skipped entirely while the context is busy. Otherwise the frame's own update
 * runs first, then two things are committed: the blend level, packed into the
 * main engine's alpha coefficients as the pair (level, 16 - level); and a
 * pending request to show or hide a layer, which rewrites the display control's
 * enable field and then clears itself so it only takes effect once.
 *
 * ARM. The enable field is read out into its own variable, edited and put
 * back, which is why the control register is read twice - the two reads are
 * volatile and keep their source order. Folding the extraction into the write
 * expression gives the same instructions but swaps the two independent ones:
 * the ROM computes the untouched bits between the field's shift and its edit,
 * and that only happens when the field is a real variable.
 */

#include "nitro/types.h"

#define REG_DISPCNT   (*(vu32 *)((unsigned int)kh_ds_io + 0x0))
#define REG_BLDALPHA  (*(vu16 *)((unsigned int)kh_ds_io + 0x52))

extern void Ov002_SampleBlendTween(void);

extern int data_ov002_0207f60c;

void Ov002_CommitDisplayState(void)
{
    unsigned char *pCtx;
    unsigned int nLevel;
    unsigned int nLayers;

    pCtx = *(unsigned char **)&data_ov002_0207f60c;
    if (*(int *)(pCtx + 0xc) != 0) {
        return;
    }
    Ov002_SampleBlendTween();

    if (*(int *)(pCtx + 8) != 0) {
        nLevel = pCtx[0x11];
        REG_BLDALPHA = (unsigned short)(nLevel | (0x10 - nLevel) * 0x100);
    }

    switch (pCtx[0x10]) {
    case 1:
        nLayers = (REG_DISPCNT & 0x1f00) >> 8;
        REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | ((nLayers | 2) << 8);
        pCtx[0x10] = 0;
        break;

    case 2:
        nLayers = (REG_DISPCNT & 0x1f00) >> 8;
        REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | ((nLayers & ~2) << 8);
        pCtx[0x10] = 0;
        break;
    }
}
