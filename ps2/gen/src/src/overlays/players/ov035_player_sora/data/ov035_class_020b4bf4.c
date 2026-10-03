/* PS2: mechanically prepared copy of src/overlays/players/ov035_player_sora/data/ov035_class_020b4bf4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov035 class descriptor gOv035SoraClass, 0x020b4bf4-0x020b4c08 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b3234, method 020b3264, 0x2e1c-byte state.
 */

extern void Ov035_stateCtorCondConfigReturnHandler(void);
extern void Ov035_initRegionCondClearGlobal(void);

GameClassDescriptor gOv035SoraClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov035_stateCtorCondConfigReturnHandler,  /* pfnCtor */
    Ov035_initRegionCondClearGlobal,  /* pfnMethod */
    11804,  /* nAuxSize */
    0,  /* pArena */
};
