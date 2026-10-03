/* PS2: mechanically prepared copy of src/overlays/players/ov095_player_luxord_4/data/ov095_class_020bcaf4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov095 class descriptor gOv095LuxordClass, 0x020bcaf4-0x020bcb08 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020ba7d4, method 020ba7fc, 0x3010-byte state.
 */

extern void Ov095_stateCtorReturnHandler(void);
extern void Ov095_stateDtorCleanup(void);

GameClassDescriptor gOv095LuxordClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov095_stateCtorReturnHandler,  /* pfnCtor */
    Ov095_stateDtorCleanup,  /* pfnMethod */
    12304,  /* nAuxSize */
    0,  /* pArena */
};
