/* PS2: mechanically prepared copy of src/overlays/players/ov066_player_donald_2/data/ov066_class_020b6af4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov066 class descriptor gOv066DonaldClass, 0x020b6af4-0x020b6b08 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b5a34, method 020b5a5c, 0x2ee0-byte state.
 */

extern void Ov066_stateCtorReturnHandler(void);
extern void Ov066_setupTriple(void);

GameClassDescriptor gOv066DonaldClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov066_stateCtorReturnHandler,  /* pfnCtor */
    Ov066_setupTriple,  /* pfnMethod */
    12000,  /* nAuxSize */
    0,  /* pArena */
};
