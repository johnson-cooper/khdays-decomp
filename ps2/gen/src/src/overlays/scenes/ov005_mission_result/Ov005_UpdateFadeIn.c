/* PS2: mechanically prepared copy of src/overlays/scenes/ov005_mission_result/Ov005_UpdateFadeIn.c (ps2/tools/prep_sources.py). Do not edit. */
/* Fades the reward menu in over time and moves to state 1 when done. */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct Ov005Context { char opaque00[0x4bf0]; int menuState; u64 startTick; } Ov005Context;
extern Ov005Context *data_ov005_0205b80c;
extern u64 OS_GetTick(void);
extern u64 func_02020368(u64,u64);
#define REG_DISPCNT (*(volatile unsigned int *)((unsigned int)kh_ds_io + 0x0))
void Ov005_UpdateFadeIn(void) {
    u64 elapsed=OS_GetTick()-data_ov005_0205b80c->startTick;
    int step=(int)func_02020368(elapsed,0x7fd8);
    if(step>1) {
        REG_DISPCNT=(REG_DISPCNT&~0x1f00)|0x1f00;
        SetMasterBrightnessMain(step-16);
    }
    if(elapsed>0x7fd88) {
        data_ov005_0205b80c->startTick=OS_GetTick();
        SetMasterBrightnessMain(0);
        data_ov005_0205b80c->menuState=1;
    }
}
