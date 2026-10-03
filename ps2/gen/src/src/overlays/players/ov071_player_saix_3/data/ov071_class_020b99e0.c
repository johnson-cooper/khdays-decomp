/* PS2: mechanically prepared copy of src/overlays/players/ov071_player_saix_3/data/ov071_class_020b99e0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov071 class descriptor gOv071SaixClass, 0x020b99e0-0x020b99f4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b8114, method 020b813c, 0x33a4-byte state.
 */

extern void Ov071_ClassCtor(void);
extern void Ov071_ClassTeardown(void);

GameClassDescriptor gOv071SaixClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov071_ClassCtor,  /* pfnCtor */
    Ov071_ClassTeardown,  /* pfnMethod */
    13220,  /* nAuxSize */
    0,  /* pArena */
};
