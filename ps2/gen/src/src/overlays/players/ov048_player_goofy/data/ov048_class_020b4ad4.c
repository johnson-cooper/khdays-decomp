/* PS2: mechanically prepared copy of src/overlays/players/ov048_player_goofy/data/ov048_class_020b4ad4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov048 class descriptor gOv048GoofyClass, 0x020b4ad4-0x020b4ae8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b3234, method 020b325c, 0x2fa0-byte state.
 */

extern void Ov048_stateCtorReturnHandler(void);
extern void Ov048_setupTriple(void);

GameClassDescriptor gOv048GoofyClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov048_stateCtorReturnHandler,  /* pfnCtor */
    Ov048_setupTriple,  /* pfnMethod */
    12192,  /* nAuxSize */
    0,  /* pArena */
};
