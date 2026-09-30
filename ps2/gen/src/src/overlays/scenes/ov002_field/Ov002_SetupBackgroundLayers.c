/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_SetupBackgroundLayers.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/engine.h"

extern void GX_SetBankForBG(int nBank);
extern void GX_SetBankForBGExtPltt(int nBank);
extern void GX_SetBankForSubBG(int nBank);
extern void GX_SetGraphicsMode(int a, int b, int c);

/* Bring up the background layers for this scene: claim the VRAM banks and give
 * BG0 the lowest priority with BG1..3 in front of it. */
void Ov002_SetupBackgroundLayers(void)
{
    volatile unsigned short *reg_bg0cnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x8);
    volatile unsigned short *reg_bg1cnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0xa);
    volatile unsigned short *reg_bg2cnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0xc);
    volatile unsigned short *reg_bg3cnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0xe);

    if (GetMasterBrightnessMain() != 0x10) {
        SetMasterBrightnessMain(~0xf);
        SetMasterBrightnessSub(~0xf);
    }

    GX_SetBankForBG(0x10);
    GX_SetGraphicsMode(1, 0, 1);
    GX_SetBankForBGExtPltt(0);
    GX_SetBankForSubBG(6 << 6);

    *reg_bg0cnt = (*reg_bg0cnt & ~3) | 3;
    *reg_bg1cnt = *reg_bg1cnt & ~3;
    *reg_bg2cnt = (*reg_bg2cnt & ~3) | 1;
    *reg_bg3cnt = (*reg_bg3cnt & ~3) | 2;
}
