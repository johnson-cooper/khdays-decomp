/* PS2: mechanically prepared copy of src/overlays/players/ov082_player_xion_3/data/ov082_class_020ba3e0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov082 class descriptor gOv082XionClass, 0x020ba3e0-0x020ba3f4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b8114, method 020b8194, 0x3a60-byte state.
 */

extern void Ov082_InitSubsystemAndReturnTick(void);
extern void Ov082_ShutdownAndFree(void);

GameClassDescriptor gOv082XionClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov082_InitSubsystemAndReturnTick,  /* pfnCtor */
    Ov082_ShutdownAndFree,  /* pfnMethod */
    14944,  /* nAuxSize */
    0,  /* pArena */
};
