/* PS2: mechanically prepared copy of src/overlays/scenes/ov007_monologue/data/ov007_class_0204d3c4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov007 class descriptor gOv007MonologueSceneClass, 0x0204d3c4-0x0204d3d8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 0204cb80, method 0204ce30, 0x5ac4-byte state.
 */

extern void Ov007_SceneInit(void);
extern void Ov007_TeardownWorkArea(void);

GameClassDescriptor gOv007MonologueSceneClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    13,  /* nGroupId */
    Ov007_SceneInit,  /* pfnCtor */
    Ov007_TeardownWorkArea,  /* pfnMethod */
    23236,  /* nAuxSize */
    0,  /* pArena */
};
