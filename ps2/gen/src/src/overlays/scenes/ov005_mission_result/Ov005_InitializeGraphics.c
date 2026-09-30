/* PS2: mechanically prepared copy of src/overlays/scenes/ov005_mission_result/Ov005_InitializeGraphics.c (ps2/tools/prep_sources.py). Do not edit. */
/* Black both screens, clear VRAM, and configure the overlay's graphics banks. */

#include "game/engine.h"

extern void GX_SetBankForLCDC(int);
extern void MIi_CpuClearFast(unsigned int,void *,unsigned int);
extern void GX_DisableBankForLCDC(void);
extern void Ov005_SetVramBankPlan(void);
extern void Ov005_ConfigureBackgroundControls(void);
extern void Ov005_ConfigureBackgroundPriorities(void);
void Ov005_InitializeGraphics(void) {
    SetMasterBrightnessMain(-16);
    SetMasterBrightnessSub(-16);
    Gfx_Reset2DEngines();
    GX_SetBankForLCDC(0x1ff);
    MIi_CpuClearFast(0,(void *)((unsigned int)kh_ds_vram + 0x0),0xa4000);
    GX_DisableBankForLCDC();
    Ov005_SetVramBankPlan();
    Ov005_ConfigureBackgroundControls();
    Ov005_ConfigureBackgroundPriorities();
    *(volatile unsigned short *)((unsigned int)kh_ds_io + 0x304) &= ~0x8000;
    *(volatile unsigned int *)((unsigned int)kh_ds_io + 0x0) &= ~0x1f00;
    *(volatile unsigned int *)((unsigned int)kh_ds_io + 0x1000) &= ~0x1f00;
}
