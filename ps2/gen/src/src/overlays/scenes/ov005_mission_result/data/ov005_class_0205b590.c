/* PS2: mechanically prepared copy of src/overlays/scenes/ov005_mission_result/data/ov005_class_0205b590.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov005 class descriptor data_ov005_0205b590, 0x0205b590-0x0205b5a4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 02056ca4, method 02056e24, 0x4c64-byte state.
 */

extern void Ov005_OpenResultScreen(void);
extern void Ov005_CloseResultScreen(void);

GameClassDescriptor data_ov005_0205b590 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    14,  /* nGroupId */
    Ov005_OpenResultScreen,  /* pfnCtor */
    Ov005_CloseResultScreen,  /* pfnMethod */
    19556,  /* nAuxSize */
    0,  /* pArena */
};
