/* PS2: mechanically prepared copy of src/overlays/players/ov083_player_zexion_3/data/ov083_class_020b9a60.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov083 class descriptor gOv083ZexionClass, 0x020b9a60-0x020b9a74 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b8114, method 020b813c, 0x3074-byte state.
 */

extern void Ov083_InitAndReturnNextState(void);
extern void Ov083_stateDtorCleanup(void);

GameClassDescriptor gOv083ZexionClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov083_InitAndReturnNextState,  /* pfnCtor */
    Ov083_stateDtorCleanup,  /* pfnMethod */
    12404,  /* nAuxSize */
    0,  /* pArena */
};
