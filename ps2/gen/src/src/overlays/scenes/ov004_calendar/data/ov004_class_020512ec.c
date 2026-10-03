/* PS2: mechanically prepared copy of src/overlays/scenes/ov004_calendar/data/ov004_class_020512ec.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov004 class descriptor data_ov004_020512ec, 0x020512ec-0x02051300 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 02050174, method 0205023c, 0x5618-byte state.
 */

extern void Ov004_CreateSceneObjects(void);
extern void Ov004_ReleaseSceneObjects(void);

GameClassDescriptor data_ov004_020512ec __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    14,  /* nGroupId */
    Ov004_CreateSceneObjects,  /* pfnCtor */
    Ov004_ReleaseSceneObjects,  /* pfnMethod */
    22040,  /* nAuxSize */
    0,  /* pArena */
};
