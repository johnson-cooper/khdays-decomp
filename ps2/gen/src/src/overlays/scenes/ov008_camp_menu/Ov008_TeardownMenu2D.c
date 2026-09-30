/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_TeardownMenu2D.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov008_TeardownMenu2D -- tear down the menu's 2D display, ov008.
 * Releases the menu's object/graphics engine binding (Ov008_PageTeardown), restores the
 * two capture/blend engines (SetMasterBrightnessMain/3cc with -0x10), clears the BG-mode/screen-base
 * bits of both DISPCNT registers, and re-enables the LCD via POWCNT1. Returns 0. */

#include "nitro/types.h"
#include "game/engine.h"

extern void Ov008_PageTeardown(int a);

int Ov008_TeardownMenu2D(void) {
    Ov008_PageTeardown(0);
    SetMasterBrightnessMain(-0x10);
    SetMasterBrightnessSub(-0x10);
    *(vu32 *)((unsigned int)kh_ds_io + 0x0) &= ~0x1f00;
    *(vu32 *)((unsigned int)kh_ds_io + 0x1000) &= ~0x1f00;
    *(vu16 *)((unsigned int)kh_ds_io + 0x304) |= 0x8000;
    return 0;
}
