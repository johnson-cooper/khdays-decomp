/* PS2: mechanically prepared copy of src/overlays/players/ov033_player_saix/data/ov033_class_020b4b00.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov033 class descriptor gOv033SaixClass, 0x020b4b00-0x020b4b14 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b3234, method 020b325c, 0x33a4-byte state.
 */

extern void Ov033_ClassCtor(void);
extern void Ov033_ClassTeardown(void);

GameClassDescriptor gOv033SaixClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov033_ClassCtor,  /* pfnCtor */
    Ov033_ClassTeardown,  /* pfnMethod */
    13220,  /* nAuxSize */
    0,  /* pArena */
};
