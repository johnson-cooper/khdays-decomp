/* PS2: mechanically prepared copy of src/overlays/system/ov024_mobiclip/Ov024_MobiClip_RestoreDisplay.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov024_MobiClip_RestoreDisplay -- MobiClip player: restore the fades to a known state and re-arm the
 * display. Any fade level other than the two extremes (+/-0x10) is forced to -0x10 on both
 * screens; then the display is brought back up and POWCNT1 bit 15 (LCD enable) set. */

#include "game/engine.h"

extern void Ov024_RunDisplayTeardownSteps(int a, int b);

void Ov024_MobiClip_RestoreDisplay(void) {
    int dark = -0x10;
    int level;

    level = GetMasterBrightnessMain();
    if (level != dark && level != 0x10) {
        level = dark;
    }
    SetMasterBrightnessMain(level);

    level = GetMasterBrightnessSub();
    if (level != dark && level != 0x10) {
        level = dark;
    }
    SetMasterBrightnessSub(level);

    Gfx_Reset2DEngines();
    Ov024_RunDisplayTeardownSteps(1, 1);
    *(volatile unsigned short *)((unsigned int)kh_ds_io + 0x304) = *(volatile unsigned short *)((unsigned int)kh_ds_io + 0x304) | 0x8000;
}
