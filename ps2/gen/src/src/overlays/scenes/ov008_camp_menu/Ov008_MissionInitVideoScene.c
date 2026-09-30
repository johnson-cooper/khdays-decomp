/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_MissionInitVideoScene.c (ps2/tools/prep_sources.py). Do not edit. */
/* Tears the mission scene's video down: clears the backdrops and layers, resets the 2D engines and
 * resources, waits for the wireless session to wind down, fades both screens to white and hands the
 * session state on. */

#include "nitro/types.h"
#include "game/engine.h"

extern void Ov105_RunScriptedStepState3(void);
extern void Ov105_WH_Finalize(void);
extern u16 *GXx_SetMasterBrightness_(u16 *reg, int brightness);
extern int Ov008_Link_GetField4F0(void);
extern u16 Ov105_GetState(void);
extern u16 Ov105_GetStatusLow(void);
extern void OS_ResetSystem(u32 value);

void *Ov008_MissionInitVideoScene(void) {
    u32 packed;

    *(volatile u16 *)((unsigned int)kh_ds_pal + 0x0) = 0;
    *(volatile u16 *)((unsigned int)kh_ds_pal + 0x400) = 0;
    *(volatile u32 *)((unsigned int)kh_ds_io + 0x0) &= ~0x1f00;
    *(volatile u32 *)((unsigned int)kh_ds_io + 0x1000) &= ~0x1f00;

    Gfx_Reset2DEngines();
    Res_TearDownBlock();

    if (Game_PollSceneAlive() != 0) {
        int waiting = 1;
        int stopped = 0;

        do {
            switch (Game_PollSceneAlive()) {
            case 1:
                Ov105_RunScriptedStepState3();
                break;
            case 0:
                waiting = stopped;
                break;
            case 3:
                break;
            default:
                Ov105_WH_Finalize();
                break;
            }
        } while (waiting != 0);
    }

    GXx_SetMasterBrightness_((u16 *)((unsigned int)kh_ds_io + 0x6c), 0x10);
    GXx_SetMasterBrightness_((u16 *)((unsigned int)kh_ds_io + 0x106c), 0x10);

    if (Ov008_Link_GetField4F0() != 0) {
        packed = (u32)-1;
    } else {
        u16 high = Ov105_GetState();
        u16 low = Ov105_GetStatusLow();

        packed = ((u32)high << 16) | low | 0x80000000;
    }

    Heap_SetCurrent(0);
    OS_ResetSystem(packed);
    return 0;
}
