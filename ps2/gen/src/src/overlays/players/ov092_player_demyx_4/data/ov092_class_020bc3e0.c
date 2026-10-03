/* PS2: mechanically prepared copy of src/overlays/players/ov092_player_demyx_4/data/ov092_class_020bc3e0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov092 class descriptor gOv092DemyxClass, 0x020bc3e0-0x020bc3f4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020ba7d4, method 020ba808, 0x30c0-byte state.
 */

extern void Ov092_stateCtorMultiInitReturnHandler(void);
extern void Ov092_stateDtorCleanupMulti(void);

GameClassDescriptor gOv092DemyxClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov092_stateCtorMultiInitReturnHandler,  /* pfnCtor */
    Ov092_stateDtorCleanupMulti,  /* pfnMethod */
    12480,  /* nAuxSize */
    0,  /* pArena */
};
