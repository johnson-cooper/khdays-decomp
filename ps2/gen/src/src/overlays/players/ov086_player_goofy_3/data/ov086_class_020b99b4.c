/* PS2: mechanically prepared copy of src/overlays/players/ov086_player_goofy_3/data/ov086_class_020b99b4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov086 class descriptor gOv086GoofyClass, 0x020b99b4-0x020b99c8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b8114, method 020b813c, 0x2fa0-byte state.
 */

extern void Ov086_stateCtorReturnHandler(void);
extern void Ov086_setupTriple(void);

GameClassDescriptor gOv086GoofyClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov086_stateCtorReturnHandler,  /* pfnCtor */
    Ov086_setupTriple,  /* pfnMethod */
    12192,  /* nAuxSize */
    0,  /* pArena */
};
