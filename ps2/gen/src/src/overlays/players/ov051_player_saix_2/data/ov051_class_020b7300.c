/* PS2: mechanically prepared copy of src/overlays/players/ov051_player_saix_2/data/ov051_class_020b7300.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov051 class descriptor gOv051SaixClass, 0x020b7300-0x020b7314 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b5a34, method 020b5a5c, 0x33a4-byte state.
 */

extern void Ov051_ClassCtor(void);
extern void Ov051_ClassTeardown(void);

GameClassDescriptor gOv051SaixClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov051_ClassCtor,  /* pfnCtor */
    Ov051_ClassTeardown,  /* pfnMethod */
    13220,  /* nAuxSize */
    0,  /* pArena */
};
