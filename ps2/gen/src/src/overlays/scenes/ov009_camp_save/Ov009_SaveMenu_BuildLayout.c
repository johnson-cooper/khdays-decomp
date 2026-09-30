/* PS2: mechanically prepared copy of src/overlays/scenes/ov009_camp_save/Ov009_SaveMenu_BuildLayout.c (ps2/tools/prep_sources.py). Do not edit. */
/* Builds the save-page widget layout: three pages of eight cells slid in from the right, arrows and
 * the summary rows. */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct Ov009Pair {
    int x;
    int y;
} Ov009Pair;

typedef struct Ov009PageContext {
    int pageIndex;
    int fallbackPage;
    int activeState;
    u8 pad_00c[0x6c - 0x0c];
    Ov009Pair cellOffset[3][8];
    Ov009Pair pageTarget[3];
    Ov009Pair pageCurrent[3];
} Ov009PageContext;

typedef struct Ov009ObjectConfig {
    int resource;
    int field_04;
    int field_08;
    int field_0c;
} Ov009ObjectConfig;

extern const Ov009ObjectConfig data_ov009_02056020;
extern const char gOv009UiCmCmSavePath[];
extern const int data_ov009_020560a8[3][8];
extern void func_ov009_02054ca0(void);

extern int         Ov009_PackHandleTagB(int index);
extern int         Ov009_GetContext(void);
extern void        Ov009_InitFromDescAndMark(int object,
                                      const Ov009ObjectConfig *config);
extern void        Ov009_LoadBlockProcessAndFree(int object, const void *resource,
                                      int value);
extern void        Ov009_StoreWordAt0x4a50(int object, int value);
extern void        G2x_SetBlendAlpha_(volatile void *reg, int firstTarget,
                                     int secondTarget, int eva, int evb);
extern int        *Ov009_FindEntryById(int object, int id);
extern void        Ov009_ReleaseTwoSlots(int object, int *entry);
extern Ov009Pair  *Ov009_ApplyFirstValidSlot(int object, int *entry);
extern void        Ov009_ReleaseTwoSlotsEx(int object, int *entry,
                                      const Ov009Pair *position);
extern void        Ov009_ReleaseTwoSlotsEx_3(int object, int *entry, int value);
extern void        Ov009_SetMenuEntriesVisible(int enabled, int mode);
extern void        Ov009_SetEntrySlotsVisible(int object, int *entry, int visible);
extern void        Ov009_SaveMenu_RefreshRows(Ov009PageContext *context);
extern Ov009Pair  *Ov009_GetEntryBlock2c(int object, int *entry);

void Ov009_SaveMenu_BuildLayout(Ov009PageContext *context)
{
    Ov009ObjectConfig config = data_ov009_02056020;
    Ov009Pair shiftedPosition;
    Ov009Pair finalPosition;
    int pageIndex;
    u32 itemIndex;
    int object;
    int *entry;
    Ov009Pair *position;

    config.resource = Ov009_PackHandleTagB(3);
    object = Ov009_GetContext();
    Ov009_InitFromDescAndMark(object, &config);
    Ov009_LoadBlockProcessAndFree(object, gOv009UiCmCmSavePath, 0x24);
    Ov009_StoreWordAt0x4a50(object, (int)func_ov009_02054ca0);
    ClampToRange0to16At0x4628(object, 8);
    G2x_SetBlendAlpha_((volatile void *)((unsigned int)kh_ds_io + 0x50), 4, 0x10, 8, 8);

    entry = Ov009_FindEntryById(object, 0x0b);
    Ov009_ReleaseTwoSlots(object, entry);
    entry = Ov009_FindEntryById(object, 0x0d);
    Ov009_ReleaseTwoSlots(object, entry);

    context->pageTarget[0].x = 0x100000;
    context->pageTarget[1].x = 0x138000;
    context->pageTarget[2].x = 0x170000;

    for (pageIndex = 0; pageIndex < 3; pageIndex++) {
        for (itemIndex = 0; itemIndex < 8; itemIndex++) {
            entry = Ov009_FindEntryById(
                object, data_ov009_020560a8[pageIndex][itemIndex]);
            position = Ov009_ApplyFirstValidSlot(object, entry);
            context->cellOffset[pageIndex][itemIndex].x = position->x;
            context->cellOffset[pageIndex][itemIndex].y = position->y;
            shiftedPosition = *position;
            shiftedPosition.x += 0x100000;
            Ov009_ReleaseTwoSlotsEx(object, entry, &shiftedPosition);
            Ov009_ReleaseTwoSlots(object, entry);
        }
    }

    entry = Ov009_FindEntryById(object, 0x14);
    Ov009_ReleaseTwoSlotsEx_3(object, entry, 2);
    entry = Ov009_FindEntryById(object, 0x15);
    Ov009_ReleaseTwoSlotsEx_3(object, entry, 2);

    Ov009_SetMenuEntriesVisible(1, 0);

    entry = Ov009_FindEntryById(object, 0x3c);
    Ov009_ReleaseTwoSlotsEx_3(object, entry, 2);
    Ov009_SetEntrySlotsVisible(object, entry, 0);

    Ov009_SaveMenu_RefreshRows(context);

    entry = Ov009_FindEntryById(object, 0x11);
    Ov009_SetEntrySlotsVisible(object, entry, 0);

    entry = Ov009_FindEntryById(object, 0x10);
    position = Ov009_GetEntryBlock2c(object, entry);
    finalPosition = *position;
    finalPosition.x += 0x8000;
    Ov009_ReleaseTwoSlotsEx(object, entry, &finalPosition);
}
