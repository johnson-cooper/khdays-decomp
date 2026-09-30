/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_ResetDisplayForPageList.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov008_ResetDisplayForPageList -- reset the menu display to the page-list layout, ov008.
 * Restores the two capture/blend engines (SetMasterBrightnessMain/3cc -0x10), clears the BG-mode bits of
 * both DISPCNT registers and re-enables the LCD (POWCNT1), then reprograms the BG priorities:
 * main BG0..3 = 3/0/1/2, sub BG0..3 = 0/1/2/3 (preserving each register's char/screen bits). */

#include "nitro/types.h"
#include "game/engine.h"

void Ov008_ResetDisplayForPageList(void) {
    vu16 *sub = (vu16 *)((unsigned int)kh_ds_io + 0x1008);
    SetMasterBrightnessMain(-0x10);
    SetMasterBrightnessSub(-0x10);
    *(vu32 *)((unsigned int)kh_ds_io + 0x0) &= ~0x1f00;
    *(vu32 *)((unsigned int)kh_ds_io + 0x1000) &= ~0x1f00;
    *(vu16 *)((unsigned int)kh_ds_io + 0x304) |= 0x8000;
    *(vu16 *)((unsigned int)kh_ds_io + 0x8) = *(vu16 *)((unsigned int)kh_ds_io + 0x8) & ~3 | 3;
    *(vu16 *)((unsigned int)kh_ds_io + 0xa) = *(vu16 *)((unsigned int)kh_ds_io + 0xa) & ~3;
    *(vu16 *)((unsigned int)kh_ds_io + 0xc) = *(vu16 *)((unsigned int)kh_ds_io + 0xc) & ~3 | 1;
    *(vu16 *)((unsigned int)kh_ds_io + 0xe) = *(vu16 *)((unsigned int)kh_ds_io + 0xe) & ~3 | 2;
    sub[0] = sub[0] & ~3;
    sub[1] = sub[1] & ~3 | 1;
    sub[2] = sub[2] & ~3 | 2;
    sub[3] = sub[3] & ~3 | 3;
}
