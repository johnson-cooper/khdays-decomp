/* PS2: mechanically prepared copy of src/overlays/players/ov102_player_donald_4/data/ov102_class_020bb894.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov102 class descriptor gOv102DonaldClass, 0x020bb894-0x020bb8a8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020ba7d4, method 020ba7fc, 0x2ee0-byte state.
 */

extern void Ov102_stateCtorReturnHandler(void);
extern void Ov102_setupTriple(void);

GameClassDescriptor gOv102DonaldClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov102_stateCtorReturnHandler,  /* pfnCtor */
    Ov102_setupTriple,  /* pfnMethod */
    12000,  /* nAuxSize */
    0,  /* pArena */
};
