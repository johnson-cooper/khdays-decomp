/* PS2: mechanically prepared copy of src/overlays/scenes/ov006_mission_mode_select/Ov006_MissionRebuildLayers.c (ps2/tools/prep_sources.py). Do not edit. */
/* Rebuild the layers and cells for the current state: reset the input entries, configure which
 * planes are visible, and arm the opening tween. */

#include "nitro/types.h"

typedef struct {
    u8 pad_0000[8];
    u8 display_state;
    u8 pad_0009[0x94e3];
    int reset_value;
    u8 pad_94f0[4];
    int scene_state;
    u8 pad_94f8[0x10];
    int entry_count;
} Ov006RootContext;

extern Ov006RootContext *data_ov006_02056664;
extern u32 gOv006UiMltMltPath;

extern void Ov006_SetBgLayout(int state);
extern void Ov006_UploadTextCells(int state);
extern void Ov006_RebindBgLayers(int state);
extern void Ov006_Menu_SetupSprites(int state);
extern void Ov006_SweepElements(void *display_state);
extern void Ov006_LoadAndInitResourceSections(void *display_state, void *descriptor);
extern void Ov006_MissionRetargetCellByTag(int layer, int x, int y);
extern void Ov006_MissionBuildScreenCells(int state);
extern void Ov006_MissionInitCells(void);
extern void Ov006_MissionRequestStateChange(int state, int duration, int start, int end,
                                int payload);

void Ov006_MissionRebuildLayers(void) {
    int i;
    volatile u32 *main_display_control = (volatile u32 *)((unsigned int)kh_ds_io + 0x0);
    volatile u32 *sub_display_control = (volatile u32 *)((unsigned int)kh_ds_io + 0x1000);

    if (data_ov006_02056664 == 0) {
        return;
    }

    Ov006_SetBgLayout(data_ov006_02056664->scene_state);
    Ov006_UploadTextCells(data_ov006_02056664->scene_state);
    Ov006_RebindBgLayers(data_ov006_02056664->scene_state);
    Ov006_Menu_SetupSprites(data_ov006_02056664->scene_state);
    Ov006_SweepElements(&data_ov006_02056664->display_state);
    Ov006_LoadAndInitResourceSections(&data_ov006_02056664->display_state,
                        (void *)&gOv006UiMltMltPath);

    Ov006_MissionRetargetCellByTag(2, 0, 0);
    Ov006_MissionRetargetCellByTag(3, 0, 0);
    Ov006_MissionRetargetCellByTag(4, 0, 4);
    Ov006_MissionRetargetCellByTag(5, 0, 0x12);
    Ov006_MissionRetargetCellByTag(0, 0, 0);
    Ov006_MissionRetargetCellByTag(1, 0, 0x16);

    for (i = 0; i < data_ov006_02056664->entry_count; i++) {
        data_ov006_02056664->reset_value = -1;
    }

    Ov006_MissionBuildScreenCells(data_ov006_02056664->scene_state);
    Ov006_MissionInitCells();

    *sub_display_control =
        (*sub_display_control & ~0x1f00) | 0x1e00;
    *main_display_control =
        (*main_display_control & ~0x1f00) | 0x1f00;

    Ov006_MissionInitCells();
    Ov006_MissionRequestStateChange(5, 0, 0, 0, 0);
}
