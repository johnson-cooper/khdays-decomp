/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/data/ov000_class_0205ab94.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov000 class descriptor data_ov000_0205ab94, 0x0205ab94-0x0205aba8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 02059e00, method 02059f50, 0xd18c-byte state.
 */

extern void Ov000_EnterSceneAndLoadResource(void);
extern void Ov000_TeardownTitle(void);

GameClassDescriptor data_ov000_0205ab94 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    14,  /* nGroupId */
    Ov000_EnterSceneAndLoadResource,  /* pfnCtor */
    Ov000_TeardownTitle,  /* pfnMethod */
    53644,  /* nAuxSize */
    0,  /* pArena */
};
