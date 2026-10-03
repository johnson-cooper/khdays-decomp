/* PS2: mechanically prepared copy of src/overlays/players/ov031_player_axel/data/ov031_class_020b4cc0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov031 class descriptor gOv031AxelClass, 0x020b4cc0-0x020b4cd4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b3234, method 020b325c, 0x2f40-byte state.
 */

extern void Ov031_ClassCtor(void);
extern void Ov031_ClassTeardown(void);

GameClassDescriptor gOv031AxelClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov031_ClassCtor,  /* pfnCtor */
    Ov031_ClassTeardown,  /* pfnMethod */
    12096,  /* nAuxSize */
    0,  /* pArena */
};
