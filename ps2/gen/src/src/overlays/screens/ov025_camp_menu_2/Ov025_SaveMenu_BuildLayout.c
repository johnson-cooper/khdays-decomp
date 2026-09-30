/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/Ov025_SaveMenu_BuildLayout.c (ps2/tools/prep_sources.py). Do not edit. */
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
    u8 pad_00c[0x68 - 0x0c];
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

extern const Ov009ObjectConfig data_ov025_020b4060;
extern const char gOv025UiCmCmSavePath[];
extern const int data_ov025_020b40e8[3][8];
extern void func_ov025_0209bccc(void);

extern int         Ov025_PackHandleTagB(int index);
extern int         Ov025_GetContext(void);
extern void        Ov025_InitFromDescAndMark(int object,
                                      const Ov009ObjectConfig *config);
extern void        Ov025_LoadBlockProcessAndFree(int object, const void *resource,
                                      int value);
extern void        Ov025_StoreWordAt0x4a50(int object, int value);
extern void        G2x_SetBlendAlpha_(volatile void *reg, int firstTarget,
                                     int secondTarget, int eva, int evb);
extern int        *Ov025_FindEntryById(int object, int id);
extern void        Ov025_ReleaseTwoSlots(int object, int *entry);
extern Ov009Pair  *Ov025_ApplyFirstValidSlot(int object, int *entry);
extern void        Ov025_ReleaseTwoSlotsEx(int object, int *entry,
                                      const Ov009Pair *position);
extern void        Ov025_ReleaseTwoSlotsEx_3(int object, int *entry, int value);
extern void        Ov025_SetMenuEntriesVisible(int enabled, int mode);
extern void        Ov025_SetEntrySlotsVisible(int object, int *entry, int visible);
extern void        Ov025_SaveMenu_RefreshRows(Ov009PageContext *context);
extern Ov009Pair  *Ov025_GetEntryBlock2c(int object, int *entry);

void Ov025_SaveMenu_BuildLayout(Ov009PageContext *context)
{
    Ov009ObjectConfig config = data_ov025_020b4060;
    Ov009Pair shiftedPosition;
    Ov009Pair finalPosition;
    int pageIndex;
    u32 itemIndex;
    int object;
    int *entry;
    Ov009Pair *position;

    config.resource = Ov025_PackHandleTagB(3);
    object = Ov025_GetContext();
    Ov025_InitFromDescAndMark(object, &config);
    Ov025_LoadBlockProcessAndFree(object, gOv025UiCmCmSavePath, 0x24);
    Ov025_StoreWordAt0x4a50(object, (int)func_ov025_0209bccc);
    ClampToRange0to16At0x4628(object, 8);
    G2x_SetBlendAlpha_((volatile void *)((unsigned int)kh_ds_io + 0x50), 4, 0x10, 8, 8);

    entry = Ov025_FindEntryById(object, 0x0b);
    Ov025_ReleaseTwoSlots(object, entry);
    entry = Ov025_FindEntryById(object, 0x0d);
    Ov025_ReleaseTwoSlots(object, entry);

    context->pageTarget[0].x = 0x100000;
    context->pageTarget[1].x = 0x138000;
    context->pageTarget[2].x = 0x170000;

    for (pageIndex = 0; pageIndex < 3; pageIndex++) {
        for (itemIndex = 0; itemIndex < 8; itemIndex++) {
            entry = Ov025_FindEntryById(
                object, data_ov025_020b40e8[pageIndex][itemIndex]);
            position = Ov025_ApplyFirstValidSlot(object, entry);
            context->cellOffset[pageIndex][itemIndex].x = position->x;
            context->cellOffset[pageIndex][itemIndex].y = position->y;
            shiftedPosition = *position;
            shiftedPosition.x += 0x100000;
            Ov025_ReleaseTwoSlotsEx(object, entry, &shiftedPosition);
            Ov025_ReleaseTwoSlots(object, entry);
        }
    }

    entry = Ov025_FindEntryById(object, 0x14);
    Ov025_ReleaseTwoSlotsEx_3(object, entry, 2);
    entry = Ov025_FindEntryById(object, 0x15);
    Ov025_ReleaseTwoSlotsEx_3(object, entry, 2);

    Ov025_SetMenuEntriesVisible(1, 0);

    entry = Ov025_FindEntryById(object, 0x3c);
    Ov025_ReleaseTwoSlotsEx_3(object, entry, 2);
    Ov025_SetEntrySlotsVisible(object, entry, 0);

    Ov025_SaveMenu_RefreshRows(context);

    entry = Ov025_FindEntryById(object, 0x11);
    Ov025_SetEntrySlotsVisible(object, entry, 0);

    entry = Ov025_FindEntryById(object, 0x10);
    position = Ov025_GetEntryBlock2c(object, entry);
    finalPosition = *position;
    finalPosition.x += 0x8000;
    Ov025_ReleaseTwoSlotsEx(object, entry, &finalPosition);
}
