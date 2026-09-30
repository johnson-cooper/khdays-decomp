/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_MissionInitDisplayResources.c (ps2/tools/prep_sources.py). Do not edit. */
/* Sets up the mission menu's display: blacks out both screens, lays out the backgrounds, text
 * cells, sprites and screen cells, clears the backdrop colours, restores brightness and requests
 * state 0xb. */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct {
    u8 pad_0000[8];
    u8 display_state[0x94ec];
    void *scene_resources;
} Ov006RootContext;

extern Ov006RootContext *data_ov008_02090fa4;

extern void Ov008_SetBgLayout(void *resources);
extern void Ov008_UploadTextCells(void *resources);
extern void Ov008_RebindBgLayers(void *resources);
extern void Ov008_Menu_SetupSprites(void *resources);
extern void Ov008_SweepElements(void *display_state);
extern void Ov008_MissionBuildScreenCells(void *resources);
extern void Ov008_MissionRequestStateChange(int state, int arg1, int arg2, int arg3, int arg4);

static inline void SetMainVisiblePlanes(int planes) {
    volatile u32 *display_control = (volatile u32 *)((unsigned int)kh_ds_io + 0x0);
    *display_control = (*display_control & ~0x1f00) | (planes << 8);
}

static inline void SetSubVisiblePlanes(int planes) {
    volatile u32 *display_control = (volatile u32 *)((unsigned int)kh_ds_io + 0x1000);
    *display_control = (*display_control & ~0x1f00) | (planes << 8);
}

void Ov008_MissionInitDisplayResources(void) {
    volatile u16 *main_palette = (volatile u16 *)((unsigned int)kh_ds_pal + 0x0);

    SetMasterBrightnessMain(-16);
    SetMasterBrightnessSub(-16);
    SetMainVisiblePlanes(0);
    SetSubVisiblePlanes(0);
    Ov008_SetBgLayout(data_ov008_02090fa4->scene_resources);
    Ov008_UploadTextCells(data_ov008_02090fa4->scene_resources);
    Ov008_RebindBgLayers(data_ov008_02090fa4->scene_resources);
    Ov008_Menu_SetupSprites(data_ov008_02090fa4->scene_resources);
    Ov008_SweepElements(data_ov008_02090fa4->display_state);
    Ov008_MissionBuildScreenCells(data_ov008_02090fa4->scene_resources);

    main_palette[0] = 0;
    main_palette[0x200] = 0;
    SetMasterBrightnessMain(0);
    SetMasterBrightnessSub(0);
    Ov008_MissionRequestStateChange(0xb, 0, 0, 0, 0);
}
