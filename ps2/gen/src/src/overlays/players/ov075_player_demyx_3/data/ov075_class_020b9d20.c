/* PS2: mechanically prepared copy of src/overlays/players/ov075_player_demyx_3/data/ov075_class_020b9d20.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov075 class descriptor gOv075DemyxClass, 0x020b9d20-0x020b9d34 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b8114, method 020b8148, 0x30c0-byte state.
 */

extern void Ov075_stateCtorMultiInitReturnHandler(void);
extern void Ov075_stateDtorCleanupMulti(void);

GameClassDescriptor gOv075DemyxClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov075_stateCtorMultiInitReturnHandler,  /* pfnCtor */
    Ov075_stateDtorCleanupMulti,  /* pfnMethod */
    12480,  /* nAuxSize */
    0,  /* pArena */
};
