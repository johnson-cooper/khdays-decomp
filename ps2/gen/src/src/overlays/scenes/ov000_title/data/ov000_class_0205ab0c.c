/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/data/ov000_class_0205ab0c.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov000 class descriptor data_ov000_0205ab0c, 0x0205ab0c-0x0205ab20 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 02053e40, method 02053f60, 0x4bcc-byte state.
 */

extern void Ov000_InitSubScene(void);
extern void Ov000_TeardownSubScene(void);

GameClassDescriptor data_ov000_0205ab0c __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    14,  /* nGroupId */
    Ov000_InitSubScene,  /* pfnCtor */
    Ov000_TeardownSubScene,  /* pfnMethod */
    19404,  /* nAuxSize */
    0,  /* pArena */
};
