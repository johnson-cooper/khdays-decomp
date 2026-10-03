/* PS2: mechanically prepared copy of src/overlays/players/ov043_player_xemnas/data/ov043_class_020b5800.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov043 class descriptor gOv043XemnasClass, 0x020b5800-0x020b5814 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b3234, method 020b3264, 0x332c-byte state.
 */

extern void Ov043_ClassCtor(void);
extern void Ov043_ClassTeardown(void);

GameClassDescriptor gOv043XemnasClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov043_ClassCtor,  /* pfnCtor */
    Ov043_ClassTeardown,  /* pfnMethod */
    13100,  /* nAuxSize */
    0,  /* pArena */
};
