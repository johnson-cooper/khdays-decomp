/* PS2: mechanically prepared copy of src/overlays/players/ov074_player_sora_3/data/ov074_class_020b9ad4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov074 class descriptor gOv074SoraClass, 0x020b9ad4-0x020b9ae8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b8114, method 020b8144, 0x2e1c-byte state.
 */

extern void Ov074_stateCtorCondConfigReturnHandler(void);
extern void Ov074_initRegionCondClearGlobal(void);

GameClassDescriptor gOv074SoraClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov074_stateCtorCondConfigReturnHandler,  /* pfnCtor */
    Ov074_initRegionCondClearGlobal,  /* pfnMethod */
    11804,  /* nAuxSize */
    0,  /* pArena */
};
