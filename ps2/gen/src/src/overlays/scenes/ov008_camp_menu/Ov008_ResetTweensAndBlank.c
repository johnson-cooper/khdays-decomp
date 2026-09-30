/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_ResetTweensAndBlank.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov008_ResetTweensAndBlank -- if a heap is active, reset the two title tween channels (-0x10) and
 * blank both screens' BG mode bits. */

#include "game/engine.h"

#define REG_DISPCNT     (*(volatile unsigned int *)((unsigned int)kh_ds_io + 0x0))
#define REG_DISPCNT_SUB (*(volatile unsigned int *)((unsigned int)kh_ds_io + 0x1000))
extern int  NNSi_FndGetCurrentRootHeap(void);

void Ov008_ResetTweensAndBlank(void) {
    if (NNSi_FndGetCurrentRootHeap() == 0) {
        return;
    }
    SetMasterBrightnessMain(-0x10);
    SetMasterBrightnessSub(-0x10);
    REG_DISPCNT &= 0xffffe0ff;
    REG_DISPCNT_SUB &= 0xffffe0ff;
}
