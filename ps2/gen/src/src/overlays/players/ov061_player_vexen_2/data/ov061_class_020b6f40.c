/* PS2: mechanically prepared copy of src/overlays/players/ov061_player_vexen_2/data/ov061_class_020b6f40.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov061 class descriptor gOv061VexenClass, 0x020b6f40-0x020b6f54 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b5a34, method 020b5a5c, 0x3490-byte state.
 */

extern void Ov061_stateCtorReturnHandler(void);
extern void Ov061_setupTriple(void);

GameClassDescriptor gOv061VexenClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov061_stateCtorReturnHandler,  /* pfnCtor */
    Ov061_setupTriple,  /* pfnMethod */
    13456,  /* nAuxSize */
    0,  /* pArena */
};
