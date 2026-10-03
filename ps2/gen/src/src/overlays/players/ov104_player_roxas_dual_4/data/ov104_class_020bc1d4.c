/* PS2: mechanically prepared copy of src/overlays/players/ov104_player_roxas_dual_4/data/ov104_class_020bc1d4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov104 class descriptor gOv104RoxasDualClass, 0x020bc1d4-0x020bc1e8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020ba7d4, method 020ba83c, 0x2f88-byte state.
 */

extern void Ov104_ClassCtor(void);
extern void Ov104_ClassTeardown(void);

GameClassDescriptor gOv104RoxasDualClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov104_ClassCtor,  /* pfnCtor */
    Ov104_ClassTeardown,  /* pfnMethod */
    12168,  /* nAuxSize */
    0,  /* pArena */
};
