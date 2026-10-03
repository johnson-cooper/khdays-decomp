/* PS2: mechanically prepared copy of src/overlays/scenes/ov005_mission_result/data/ov005_class_0205b7cc.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov005 class descriptor data_ov005_0205b7cc, 0x0205b7cc-0x0205b7e0 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020586ac, method 0205873c, 0x3c-byte state.
 */

extern void Ov005_OpenExitTask(void);
extern void Ov005_ClassTeardown(void);

GameClassDescriptor data_ov005_0205b7cc __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    15,  /* nGroupId */
    Ov005_OpenExitTask,  /* pfnCtor */
    Ov005_ClassTeardown,  /* pfnMethod */
    60,  /* nAuxSize */
    0,  /* pArena */
};
