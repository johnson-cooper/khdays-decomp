/* PS2: mechanically prepared copy of src/overlays/scenes/ov003/data/ov003_class_0204f8e4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov003 class descriptor data_ov003_0204f8e4, 0x0204f8e4-0x0204f8f8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 0204d98c, method 0204e384, 0x1e18-byte state.
 */

extern void Ov003_SceneInit(void);
extern void Ov003_Teardown(void);

GameClassDescriptor data_ov003_0204f8e4 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    15,  /* nGroupId */
    Ov003_SceneInit,  /* pfnCtor */
    Ov003_Teardown,  /* pfnMethod */
    7704,  /* nAuxSize */
    0,  /* pArena */
};
