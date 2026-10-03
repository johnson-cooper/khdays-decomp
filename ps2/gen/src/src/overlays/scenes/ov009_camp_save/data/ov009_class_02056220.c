/* PS2: mechanically prepared copy of src/overlays/scenes/ov009_camp_save/data/ov009_class_02056220.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov009 class descriptor data_ov009_02056220, 0x02056220-0x02056234 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 0204cac0, method 0204caec, 0x4-byte state.
 */

extern void Ov009_ClassCtor(void);
extern void Ov009_SaveSceneTeardown(void);

GameClassDescriptor data_ov009_02056220 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    14,  /* nGroupId */
    Ov009_ClassCtor,  /* pfnCtor */
    Ov009_SaveSceneTeardown,  /* pfnMethod */
    4,  /* nAuxSize */
    0,  /* pArena */
};
