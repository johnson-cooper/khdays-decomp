/* PS2: mechanically prepared copy of src/overlays/scenes/ov011/data/ov011_class_0205e8a0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov011 class descriptor data_ov011_0205e8a0, 0x0205e8a0-0x0205e8b4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 0205ac40, method 0205b034, 0x2cf84-byte state.
 */

extern void Ov011_CreateScene(void);
extern void Ov011_DestroyScene(void);

GameClassDescriptor data_ov011_0205e8a0 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    15,  /* nGroupId */
    Ov011_CreateScene,  /* pfnCtor */
    Ov011_DestroyScene,  /* pfnMethod */
    184196,  /* nAuxSize */
    0,  /* pArena */
};
