/* PS2: mechanically prepared copy of src/overlays/players/ov030_player_roxas/data/ov030_class_020b58f4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov030 class descriptor gOv030RoxasClass, 0x020b58f4-0x020b5908 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b3234, method 020b3310, 0x3a68-byte state.
 */

extern void Ov030_SetUpScene(void);
extern void Ov030_TeardownScene(void);

GameClassDescriptor gOv030RoxasClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov030_SetUpScene,  /* pfnCtor */
    Ov030_TeardownScene,  /* pfnMethod */
    14952,  /* nAuxSize */
    0,  /* pArena */
};
