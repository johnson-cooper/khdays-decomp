/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/Ov000_IsSelectionMoveCollisionFree.c (ps2/tools/prep_sources.py). Do not edit. */
/* Whether moving the selected list entry by the offset keeps it clear of every other entry. */

#include "nitro/types.h"

typedef struct {
    u8 pad_0000[0x4c];
    u8 object[1];
    u8 pad_004d[0x4a7f];
    u8 itemCountMinus3;
} Ov000SceneContext;

extern Ov000SceneContext *data_ov000_0205ac24;
extern int Ov000_FindEntryById(int object, int id);
extern int Ov000_GetEntryPosition(int object, int entry);

int Ov000_IsSelectionMoveCollisionFree(int selectedIndex, int movementXArg,
                        int movementYArg, ...) {
    /* PS2 R14: the DS argument block `&movementYArg` pointed at */
    unsigned int __kh_va[1 + 12];
    { __builtin_va_list __kh_ap; int __kh_i; __builtin_memcpy(__kh_va, &movementYArg, 4); __builtin_va_start(__kh_ap, movementYArg); for (__kh_i = 1; __kh_i <= 12; __kh_i++) __kh_va[__kh_i] = __builtin_va_arg(__kh_ap, unsigned int); __builtin_va_end(__kh_ap); }

    int movementX;
    int context;
    int i;
    int *selectedPosition;
    int movementY;

    movementY = *(int *)((__typeof__(movementYArg) *)__kh_va);
    context = (int)data_ov000_0205ac24;
    selectedPosition = (int *)Ov000_GetEntryPosition(
        context + 0x4c,
        Ov000_FindEntryById(context + 0x4c, selectedIndex + 1));
    i = 0;

    if (i < data_ov000_0205ac24->itemCountMinus3 + 3) {
        movementX = *(int *)&movementXArg;
        do {
            int *position = (int *)Ov000_GetEntryPosition(
                context + 0x4c,
                Ov000_FindEntryById(context + 0x4c, i + 1));

            if (selectedPosition[0] - 0x2000 - movementX <
                    position[0] + 0xc4000 &&
                position[0] - 0x2000 <
                    selectedPosition[0] + 0xc4000 - movementX &&
                selectedPosition[1] + 0x2000 - movementY <
                    position[1] + 0x24000 &&
                ((volatile int *)position)[1] <
                    selectedPosition[1] + 0x24000 - movementY &&
                i != selectedIndex) {
                return 0;
            }

            i++;
        } while (i < data_ov000_0205ac24->itemCountMinus3 + 3);
    }

    return 1;
}
