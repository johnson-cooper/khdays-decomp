/* PS2: mechanically prepared copy of src/overlays/players/ov087_player_roxas_dual_3/data/ov087_class_020b9b14.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov087 class descriptor gOv087RoxasDualClass, 0x020b9b14-0x020b9b28 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b8114, method 020b817c, 0x2f88-byte state.
 */

extern void Ov087_ClassCtor(void);
extern void Ov087_ClassTeardown(void);

GameClassDescriptor gOv087RoxasDualClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov087_ClassCtor,  /* pfnCtor */
    Ov087_ClassTeardown,  /* pfnMethod */
    12168,  /* nAuxSize */
    0,  /* pArena */
};
