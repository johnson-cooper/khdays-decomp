/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/data/ov000_class_0205aa34.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov000 class descriptor data_ov000_0205aa34, 0x0205aa34-0x0205aa48 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 02051c48, method 02051f94, 0x6a64-byte state.
 */

extern void Ov000_LoadScreenInit(void);
extern void Ov000_DestroyScene(void);

GameClassDescriptor data_ov000_0205aa34 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    14,  /* nGroupId */
    Ov000_LoadScreenInit,  /* pfnCtor */
    Ov000_DestroyScene,  /* pfnMethod */
    27236,  /* nAuxSize */
    0,  /* pArena */
};
