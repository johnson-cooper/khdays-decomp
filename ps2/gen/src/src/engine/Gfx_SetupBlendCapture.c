/* PS2: mechanically prepared copy of src/engine/Gfx_SetupBlendCapture.c (ps2/tools/prep_sources.py). Do not edit. */
/* Sets up the display capture used by a screen effect: both screens are swapped back (POWCNT bit 15),
 * the capture mode is chosen (GX_SetGraphicsMode 1/0/1 for a full blend of 16, else 0xa/4/1), the VRAM
 * banks are laid out (sub OBJ bank D, LCDC bank C) and DISPCAPCNT is programmed: with blending
 * (flag 1) capture of A+B into bank +4 with EVA = +0xc and EVB = 16 - EVA, otherwise a plain 3D
 * capture into bank +4. The main screen's layers take the +8 mask and the sub screen shows OBJ. */

#include "nitro/types.h"

typedef struct {
    u32 flags;          /* 0x00 */
    u32 bank;           /* 0x04 */
    u32 layers;         /* 0x08 */
    u32 eva;            /* 0x0c */
} CaptureCfg;

extern void GX_SetGraphicsMode(int a, int b, int c);
extern void GX_ResetBankForSubBG(void);
extern void GX_SetBankForSubOBJ(int bank);
extern void GX_SetBankForLCDC(int bank);

void Gfx_SetupBlendCapture(CaptureCfg *cfg)
{
    *(volatile u16 *)((unsigned int)kh_ds_io + 0x304) &= ~0x8000;
    if (cfg->eva != 0x10) {
        GX_SetGraphicsMode(0xa, 4, 1);
    } else {
        GX_SetGraphicsMode(1, 0, 1);
    }
    GX_ResetBankForSubBG();
    GX_SetBankForSubOBJ(8);
    GX_SetBankForLCDC(4);
    if (cfg->flags & 1) {
        /* built in two steps: the single expression schedules the EVB term differently */
        u32 dispcapcnt = 0xc0320000 | (cfg->bank << 24);
        dispcapcnt = cfg->eva | (dispcapcnt | ((0x10 - cfg->eva) << 8));
        *(volatile u32 *)((unsigned int)kh_ds_io + 0x64) = dispcapcnt;
    } else {
        *(volatile u32 *)((unsigned int)kh_ds_io + 0x64) = 0x80360010 | (cfg->bank << 24);
    }
    *(volatile u32 *)((unsigned int)kh_ds_io + 0x0) = (*(volatile u32 *)((unsigned int)kh_ds_io + 0x0) & ~0x1f00) | (cfg->layers << 8);
    *(volatile u32 *)((unsigned int)kh_ds_io + 0x1000) = (*(volatile u32 *)((unsigned int)kh_ds_io + 0x1000) & ~0x1f00) | 0x1000;
}
