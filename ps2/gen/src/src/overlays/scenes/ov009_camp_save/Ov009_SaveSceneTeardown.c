/* PS2: mechanically prepared copy of src/overlays/scenes/ov009_camp_save/Ov009_SaveSceneTeardown.c (ps2/tools/prep_sources.py). Do not edit. */
/* Tears the save scene down: blanks both screens and turns the displays off. */

#include "nitro/types.h"
#include "game/engine.h"

extern void *data_ov009_020563e0;
extern void Ov009_PageTeardown(int mode);

#define REG_DISPCNT (*(volatile u32 *)((unsigned int)kh_ds_io + 0x0))
#define REG_DISPCNT_SUB (*(volatile u32 *)((unsigned int)kh_ds_io + 0x1000))
#define REG_POWER_CNT (*(volatile u16 *)((unsigned int)kh_ds_io + 0x304))

void Ov009_SaveSceneTeardown(void)
{
    Ov009_PageTeardown(0);
    SetMasterBrightnessMain(-0x10);
    SetMasterBrightnessSub(-0x10);
    REG_DISPCNT &= 0xffffe0ff;
    REG_DISPCNT_SUB &= 0xffffe0ff;
    REG_POWER_CNT |= 0x8000;
    data_ov009_020563e0 = 0;
}
