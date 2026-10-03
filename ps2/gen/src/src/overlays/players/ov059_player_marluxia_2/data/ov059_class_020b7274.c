/* PS2: mechanically prepared copy of src/overlays/players/ov059_player_marluxia_2/data/ov059_class_020b7274.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov059 class descriptor gOv059MarluxiaClass, 0x020b7274-0x020b7288 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b5a34, method 020b5a5c, 0x2f90-byte state.
 */

extern void Ov059_stateCtorReturnHandler(void);
extern void Ov059_setupTriple(void);

GameClassDescriptor gOv059MarluxiaClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov059_stateCtorReturnHandler,  /* pfnCtor */
    Ov059_setupTriple,  /* pfnMethod */
    12176,  /* nAuxSize */
    0,  /* pArena */
};
