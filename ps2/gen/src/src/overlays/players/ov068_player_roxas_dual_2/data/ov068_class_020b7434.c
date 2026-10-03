/* PS2: mechanically prepared copy of src/overlays/players/ov068_player_roxas_dual_2/data/ov068_class_020b7434.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov068 class descriptor gOv068RoxasDualClass, 0x020b7434-0x020b7448 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b5a34, method 020b5a9c, 0x2f88-byte state.
 */

extern void Ov068_ClassCtor(void);
extern void Ov068_ClassTeardown(void);

GameClassDescriptor gOv068RoxasDualClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov068_ClassCtor,  /* pfnCtor */
    Ov068_ClassTeardown,  /* pfnMethod */
    12168,  /* nAuxSize */
    0,  /* pArena */
};
