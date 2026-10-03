/* PS2: mechanically prepared copy of src/overlays/players/ov085_player_donald_3/data/ov085_class_020b91d4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov085 class descriptor gOv085DonaldClass, 0x020b91d4-0x020b91e8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b8114, method 020b813c, 0x2ee0-byte state.
 */

extern void Ov085_stateCtorReturnHandler(void);
extern void Ov085_setupTriple(void);

GameClassDescriptor gOv085DonaldClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov085_stateCtorReturnHandler,  /* pfnCtor */
    Ov085_setupTriple,  /* pfnMethod */
    12000,  /* nAuxSize */
    0,  /* pArena */
};
