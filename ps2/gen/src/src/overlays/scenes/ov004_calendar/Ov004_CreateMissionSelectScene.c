/* PS2: mechanically prepared copy of src/overlays/scenes/ov004_calendar/Ov004_CreateMissionSelectScene.c (ps2/tools/prep_sources.py). Do not edit. */
/* Creates the calendar scene for a day: records the day, awards the all-missions flags, fixes the
 * equipment, sets up the graphics and starts the calendar animation. */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct Ov004SceneArgs {
    int currentDay;
    int selectedDay;
} Ov004SceneArgs;

extern const Ov004SceneArgs data_ov004_0205109c;
extern char *data_ov004_02051380;
extern u8 data_0204c300[];
extern char data_ov004_020512ec[];

extern char *NNSi_FndGetCurrentRootHeap(void);
extern void *MI_CpuFill8(void *dst, int value, u32 size);
extern int Ov004_MapMissionToRankSlot(int value);
extern void Ov004_TryAwardAllMissionsPlayed(void);
extern void Ov004_TryAwardAllMissionsCleared(void);
extern void Ov004_BuildMissionObjectLists(void);
extern void Ov004_ConfigureGraphics(void);
extern void *InstantiateClass(void *config, void *args);
extern void *Ov004_StepMissionSelectScene(void);

void *Ov004_CreateMissionSelectScene(int requestedDay) {
    Ov004SceneArgs args;
    int currentDay;
    int index;
    int notify;
    u32 field;
    u32 value;
    int enabled;
    u8 playerFlag;
    u8 combinedFlag;

    currentDay = GameState_GetField(0, 9);
    args = data_ov004_0205109c;

    NNSi_FndGetCurrentRootHeap();
    data_ov004_02051380 = NNSi_FndGetCurrentRootHeap();
    MI_CpuFill8(data_ov004_02051380, 0, 8);

    switch (requestedDay) {
    case 0x190:
        currentDay = 0x190;
        *(int *)(data_ov004_02051380 + 4) = currentDay;
        break;
    case 0x191:
        *(int *)(data_ov004_02051380 + 4) = 7;
        currentDay = 0xff;
        break;
    default:
        *(int *)(data_ov004_02051380 + 4) = requestedDay;
        break;
    case 0:
        *(int *)(data_ov004_02051380 + 4) = Ov004_MapMissionToRankSlot(currentDay);
        break;
    }

    if ((u32)(currentDay - 0x165) <= 1) {
        index = currentDay == 0x165 ? 0x5c : 0x5d;

        field = index * 4 + 0x92b;
        playerFlag = data_0204c300[0x4e];
        combinedFlag = playerFlag | GameState_GetField(field, 4);
        notify = 0;
        GameState_SetField(field, 4, combinedFlag);

        if (index == 0x5d) {
            enabled = GameState_GetField(index * 3 + 0x28e4, 3) == 3;
            if (enabled == 0) {
                notify = 1;
            }
        }

        field = index * 3 + 0x28e4;
        value = (u16)GameState_GetField(field, 3);
        if (value < 1) {
            GameState_SetField(field, 3, 1);
        }
        value = (u16)GameState_GetField(field, 3);
        if (value < 2) {
            GameState_SetField(field, 3, 2);
        }
        GameState_GetField(field, 3);
        GameState_SetField(field, 3, 3);

        if (notify != 0) {
            Ov004_TryAwardAllMissionsPlayed();
            Ov004_TryAwardAllMissionsCleared();
        }
    }

    if (currentDay == 0x165) {
        Ov004_BuildMissionObjectLists();
    }

    args.currentDay = currentDay;
    args.selectedDay = *(int *)(data_ov004_02051380 + 4);

    data_ov004_02051380 = NNSi_FndGetCurrentRootHeap();
    Ov004_ConfigureGraphics();
    *(void **)data_ov004_02051380 = InstantiateClass(data_ov004_020512ec, &args);

    *(volatile u32 *)((unsigned int)kh_ds_io + 0x0) =
        (*(volatile u32 *)((unsigned int)kh_ds_io + 0x0) & ~0x1f00) | 0x1f00;

    return (void *)Ov004_StepMissionSelectScene;
}
