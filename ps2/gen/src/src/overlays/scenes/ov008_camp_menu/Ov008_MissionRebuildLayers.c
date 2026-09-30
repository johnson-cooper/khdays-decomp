/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_MissionRebuildLayers.c (ps2/tools/prep_sources.py). Do not edit. */
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

extern Ov006RootContext *data_ov008_02090fa4;
extern u32 gOv008UiMltMltPath;

extern void Ov008_SetBgLayout(int state);
extern void Ov008_UploadTextCells(int state);
extern void Ov008_RebindBgLayers(int state);
extern void Ov008_Menu_SetupSprites(int state);
extern void Ov008_SweepElements(void *display_state);
extern void Ov008_LoadLayoutResource(void *display_state, void *descriptor);
extern void Ov008_MissionRetargetCellByTag(int layer, int x, int y);
extern void Ov008_MissionBuildScreenCells(int state);
extern void Ov008_MissionInitCells(void);
extern void Ov008_MissionRequestStateChange(int state, int duration, int start, int end,
                                int payload);

void Ov008_MissionRebuildLayers(void) {
    int i;
    volatile u32 *main_display_control = (volatile u32 *)((unsigned int)kh_ds_io + 0x0);
    volatile u32 *sub_display_control = (volatile u32 *)((unsigned int)kh_ds_io + 0x1000);

    if (data_ov008_02090fa4 == 0) {
        return;
    }

    Ov008_SetBgLayout(data_ov008_02090fa4->scene_state);
    Ov008_UploadTextCells(data_ov008_02090fa4->scene_state);
    Ov008_RebindBgLayers(data_ov008_02090fa4->scene_state);
    Ov008_Menu_SetupSprites(data_ov008_02090fa4->scene_state);
    Ov008_SweepElements(&data_ov008_02090fa4->display_state);
    Ov008_LoadLayoutResource(&data_ov008_02090fa4->display_state,
                        (void *)&gOv008UiMltMltPath);

    Ov008_MissionRetargetCellByTag(2, 0, 0);
    Ov008_MissionRetargetCellByTag(3, 0, 0);
    Ov008_MissionRetargetCellByTag(4, 0, 4);
    Ov008_MissionRetargetCellByTag(5, 0, 0x12);
    Ov008_MissionRetargetCellByTag(0, 0, 0);
    Ov008_MissionRetargetCellByTag(1, 0, 0x16);

    for (i = 0; i < data_ov008_02090fa4->entry_count; i++) {
        data_ov008_02090fa4->reset_value = -1;
    }

    Ov008_MissionBuildScreenCells(data_ov008_02090fa4->scene_state);
    Ov008_MissionInitCells();

    *sub_display_control =
        (*sub_display_control & ~0x1f00) | 0x1e00;
    *main_display_control =
        (*main_display_control & ~0x1f00) | 0x1f00;

    Ov008_MissionInitCells();
    Ov008_MissionRequestStateChange(5, 0, 0, 0, 0);
}
