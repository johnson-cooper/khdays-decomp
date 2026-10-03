/* PS2: mechanically prepared copy of src/overlays/players/ov079_player_marluxia_3/data/ov079_class_020b9954.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov079 class descriptor gOv079MarluxiaClass, 0x020b9954-0x020b9968 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b8114, method 020b813c, 0x2f90-byte state.
 */

extern void Ov079_stateCtorReturnHandler(void);
extern void Ov079_setupTriple(void);

GameClassDescriptor gOv079MarluxiaClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov079_stateCtorReturnHandler,  /* pfnCtor */
    Ov079_setupTriple,  /* pfnMethod */
    12176,  /* nAuxSize */
    0,  /* pArena */
};
