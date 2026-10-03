/* PS2: mechanically prepared copy of src/overlays/players/ov100_player_zexion_4/data/ov100_class_020bc120.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov100 class descriptor gOv100ZexionClass, 0x020bc120-0x020bc134 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020ba7d4, method 020ba7fc, 0x3074-byte state.
 */

extern void Ov100_InitAndReturnNextState(void);
extern void Ov100_stateDtorCleanup(void);

GameClassDescriptor gOv100ZexionClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov100_InitAndReturnNextState,  /* pfnCtor */
    Ov100_stateDtorCleanup,  /* pfnMethod */
    12404,  /* nAuxSize */
    0,  /* pArena */
};
