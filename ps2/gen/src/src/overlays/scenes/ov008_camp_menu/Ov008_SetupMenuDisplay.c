/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_SetupMenuDisplay.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov008_SetupMenuDisplay -- bring up the menu display, ov008. Resets the 2D/3D engines,
 * powers the LCD, assigns VRAM banks (BG A=3, sub-BG=4, OBJ=0x70, sub-OBJ=8), sets the
 * sub graphics mode, and programs the BG priorities on both engines (main BG0..3 = 1/3/2/-,
 * sub BG1..3 = 2/1/-). */

#include "nitro/types.h"
#include "game/engine.h"

#define reg_GX_DISPCNT (*(vu32 *)((unsigned int)kh_ds_io + 0x0))

extern void GX_SetBankForBG(int);
extern void GX_SetBankForSubBG(int);
extern void GX_SetBankForOBJ(int);
extern void GX_SetBankForSubOBJ(int);
extern void GXS_SetGraphicsMode(int);
extern void GX_SetGraphicsMode(int, int, int);

void Ov008_SetupMenuDisplay(void) {
    Gfx_Reset2DEngines();
    SetGameMode(0);
    *(vu16 *)((unsigned int)kh_ds_io + 0x304) |= 0x8000;
    reg_GX_DISPCNT &= ~0x38000000;
    reg_GX_DISPCNT &= ~0x7000000;
    GX_SetBankForBG(3);
    GX_SetBankForSubBG(4);
    GX_SetBankForOBJ(0x70);
    GX_SetBankForSubOBJ(8);
    GXS_SetGraphicsMode(0);
    GX_SetGraphicsMode(1, 0, 0);
    {
        vu16 *bg1 = (vu16 *)((unsigned int)kh_ds_io + 0xa);
        vu16 *bg0 = (vu16 *)((unsigned int)kh_ds_io + 0x8);
        vu16 *sub1 = (vu16 *)((unsigned int)kh_ds_io + 0x100a);
        vu16 *sub2 = (vu16 *)((unsigned int)kh_ds_io + 0x100c);
        bg1[0] = (bg1[0] & ~3) | 3;
        bg1[1] = (bg1[1] & ~3) | 2;
        *bg0 = (*bg0 & ~3) | 1;
        bg1[2] = bg1[2] & ~3;
        *sub1 = (*sub1 & ~3) | 2;
        *sub2 = (*sub2 & ~3) | 1;
        sub2[1] = sub2[1] & ~3;
    }
}
