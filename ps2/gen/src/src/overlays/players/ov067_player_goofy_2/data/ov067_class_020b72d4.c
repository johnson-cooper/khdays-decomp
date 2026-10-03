/* PS2: mechanically prepared copy of src/overlays/players/ov067_player_goofy_2/data/ov067_class_020b72d4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov067 class descriptor gOv067GoofyClass, 0x020b72d4-0x020b72e8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b5a34, method 020b5a5c, 0x2fa0-byte state.
 */

extern void Ov067_stateCtorReturnHandler(void);
extern void Ov067_setupTriple(void);

GameClassDescriptor gOv067GoofyClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov067_stateCtorReturnHandler,  /* pfnCtor */
    Ov067_setupTriple,  /* pfnMethod */
    12192,  /* nAuxSize */
    0,  /* pArena */
};
