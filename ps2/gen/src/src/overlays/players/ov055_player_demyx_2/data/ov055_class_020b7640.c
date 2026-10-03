/* PS2: mechanically prepared copy of src/overlays/players/ov055_player_demyx_2/data/ov055_class_020b7640.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov055 class descriptor gOv055DemyxClass, 0x020b7640-0x020b7654 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b5a34, method 020b5a68, 0x30c0-byte state.
 */

extern void Ov055_stateCtorMultiInitReturnHandler(void);
extern void Ov055_stateDtorCleanupMulti(void);

GameClassDescriptor gOv055DemyxClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov055_stateCtorMultiInitReturnHandler,  /* pfnCtor */
    Ov055_stateDtorCleanupMulti,  /* pfnMethod */
    12480,  /* nAuxSize */
    0,  /* pArena */
};
