/* PS2: mechanically prepared copy of src/overlays/players/ov065_player_mickey_2/data/ov065_class_020b72a0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov065 class descriptor gOv065MickeyClass, 0x020b72a0-0x020b72b4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b5a34, method 020b5a5c, 0x34b0-byte state.
 */

extern void Ov065_stateCtorReturnHandler(void);
extern void Ov065_stateDtorCleanup(void);

GameClassDescriptor gOv065MickeyClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov065_stateCtorReturnHandler,  /* pfnCtor */
    Ov065_stateDtorCleanup,  /* pfnMethod */
    13488,  /* nAuxSize */
    0,  /* pArena */
};
