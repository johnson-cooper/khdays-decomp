/* PS2: mechanically prepared copy of src/overlays/players/ov036_player_demyx/data/ov036_class_020b4e40.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov036 class descriptor gOv036DemyxClass, 0x020b4e40-0x020b4e54 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b3234, method 020b3268, 0x30c0-byte state.
 */

extern void Ov036_stateCtorMultiInitReturnHandler(void);
extern void Ov036_stateDtorCleanupMulti(void);

GameClassDescriptor gOv036DemyxClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov036_stateCtorMultiInitReturnHandler,  /* pfnCtor */
    Ov036_stateDtorCleanupMulti,  /* pfnMethod */
    12480,  /* nAuxSize */
    0,  /* pArena */
};
