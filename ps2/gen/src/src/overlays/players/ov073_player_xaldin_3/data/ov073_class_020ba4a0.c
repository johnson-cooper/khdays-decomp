/* PS2: mechanically prepared copy of src/overlays/players/ov073_player_xaldin_3/data/ov073_class_020ba4a0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov073 class descriptor gOv073XaldinClass, 0x020ba4a0-0x020ba4b4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b8114, method 020b8148, 0x2e20-byte state.
 */

extern void Ov073_stateCtorConfigReturnHandler(void);
extern void Ov073_initTwoRegionsClearGlobal(void);

GameClassDescriptor gOv073XaldinClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov073_stateCtorConfigReturnHandler,  /* pfnCtor */
    Ov073_initTwoRegionsClearGlobal,  /* pfnMethod */
    11808,  /* nAuxSize */
    0,  /* pArena */
};
