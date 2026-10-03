/* PS2: mechanically prepared copy of src/overlays/players/ov064_player_zexion_2/data/ov064_class_020b7380.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov064 class descriptor gOv064ZexionClass, 0x020b7380-0x020b7394 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b5a34, method 020b5a5c, 0x3074-byte state.
 */

extern void Ov064_InitAndReturnNextState(void);
extern void Ov064_stateDtorCleanup(void);

GameClassDescriptor gOv064ZexionClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov064_InitAndReturnNextState,  /* pfnCtor */
    Ov064_stateDtorCleanup,  /* pfnMethod */
    12404,  /* nAuxSize */
    0,  /* pArena */
};
