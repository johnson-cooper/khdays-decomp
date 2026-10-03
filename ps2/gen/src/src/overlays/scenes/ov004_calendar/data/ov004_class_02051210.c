/* PS2: mechanically prepared copy of src/overlays/scenes/ov004_calendar/data/ov004_class_02051210.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov004 class descriptor gOv004CalendarSceneClass, 0x02051210-0x02051224 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 0204fa44, method 0204fc98, 0x8-byte state.
 */

extern void Ov004_CreateMissionSelectScene(void);
extern void Ov004_ClassTeardown(void);

GameClassDescriptor gOv004CalendarSceneClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    15,  /* nGroupId */
    Ov004_CreateMissionSelectScene,  /* pfnCtor */
    Ov004_ClassTeardown,  /* pfnMethod */
    8,  /* nAuxSize */
    0,  /* pArena */
};
