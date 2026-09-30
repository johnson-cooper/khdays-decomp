/* PS2: mechanically prepared copy of src/overlays/scenes/ov007_monologue/Ov007_SetupSubDisplay.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/engine.h"

extern void GX_SetGraphicsMode(int a, int b, int c);
extern void GX_SetBankForBG(int bank);
extern void GX_SetBankForBGExtPltt(int bank);
extern void GX_SetBankForOBJ(int bank);

/* Configure the sub-screen display for the ov007 text scene: reset brightness,
 * assign VRAM banks, then set DISPCNT (BG mode + ext-palette), BG1/BG2 control
 * (priority + char base) and enable POWCNT. */
void Ov007_SetupSubDisplay(void) {
    SetMasterBrightnessMain(-0x10);
    SetMasterBrightnessSub(-0x10);
    Gfx_Reset2DEngines();
    GX_SetGraphicsMode(1, 0, 1);
    GX_SetBankForBG(1);
    GX_SetBankForBGExtPltt(0x20);
    GX_SetBankForOBJ(2);
    {
        volatile unsigned int *reg_dispcnt = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x0);

        reg_dispcnt[0] = reg_dispcnt[0] & 0xffcfffef | 0x10 | 0x200000;
        *(volatile unsigned short *)((char *)reg_dispcnt + 0xa) =
            *(volatile unsigned short *)((char *)reg_dispcnt + 0xa) & 0x43 | 0x90;
        *(volatile unsigned short *)((char *)reg_dispcnt + 0xc) =
            *(volatile unsigned short *)((char *)reg_dispcnt + 0xc) & 0x43 | 0x104;
        /* POWCNT1: bit15 = swap top/bottom LCD */
        *(volatile unsigned short *)((unsigned int)kh_ds_io + 0x304) =
            *(volatile unsigned short *)((unsigned int)kh_ds_io + 0x304) | 0x8000;
        reg_dispcnt[0] = reg_dispcnt[0] & ~0x1f00 | 0x1600;
        *(volatile unsigned short *)((char *)reg_dispcnt + 0xa) =
            *(volatile unsigned short *)((char *)reg_dispcnt + 0xa) & ~3 | 1;
        *(volatile unsigned short *)((char *)reg_dispcnt + 0xc) =
            *(volatile unsigned short *)((char *)reg_dispcnt + 0xc) & ~3;
    }
}
