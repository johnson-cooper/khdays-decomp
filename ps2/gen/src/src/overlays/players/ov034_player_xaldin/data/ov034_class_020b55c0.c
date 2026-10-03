/* PS2: mechanically prepared copy of src/overlays/players/ov034_player_xaldin/data/ov034_class_020b55c0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov034 class descriptor gOv034XaldinClass, 0x020b55c0-0x020b55d4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b3234, method 020b3268, 0x2e20-byte state.
 */

extern void Ov034_stateCtorConfigReturnHandler(void);
extern void Ov034_initTwoRegionsClearGlobal(void);

GameClassDescriptor gOv034XaldinClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov034_stateCtorConfigReturnHandler,  /* pfnCtor */
    Ov034_initTwoRegionsClearGlobal,  /* pfnMethod */
    11808,  /* nAuxSize */
    0,  /* pArena */
};
