/* PS2: mechanically prepared copy of src/overlays/screens/ov026_shop/Ov026_BlankScreens.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov026_BlankScreens -- restore the capture/blend engines (SetMasterBrightnessMain/3cc with -0x10),
 * clear the BG-mode/screen-base bits of both DISPCNT registers, and switch the main engine
 * to the top physical LCD via Ov002_SetDisplaySwap. */

#include "nitro/types.h"
#include "game/engine.h"

extern void Ov002_SetDisplaySwap(int top);

void Ov026_BlankScreens(void) {
    SetMasterBrightnessMain(-0x10);
    SetMasterBrightnessSub(-0x10);
    *(vu32 *)((unsigned int)kh_ds_io + 0x0) &= ~0x1f00;
    *(vu32 *)((unsigned int)kh_ds_io + 0x1000) &= ~0x1f00;
    Ov002_SetDisplaySwap(1);
}
