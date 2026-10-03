/* PS2: mechanically prepared copy of src/overlays/screens/ov106/data/ov106_class_020b8b20.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov106 class descriptor data_ov106_020b8b20, 0x020b8b20-0x020b8b34 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b8410, method 020b8448, 0x84-byte state.
 */

extern void Ov106_SubSceneEnter(void);
extern void Ov106_ClassTeardown(void);

GameClassDescriptor data_ov106_020b8b20 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    62,  /* nClassId */
    13,  /* nGroupId */
    Ov106_SubSceneEnter,  /* pfnCtor */
    Ov106_ClassTeardown,  /* pfnMethod */
    132,  /* nAuxSize */
    0,  /* pArena */
};
