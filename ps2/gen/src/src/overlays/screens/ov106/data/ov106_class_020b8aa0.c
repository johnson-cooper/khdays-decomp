/* PS2: mechanically prepared copy of src/overlays/screens/ov106/data/ov106_class_020b8aa0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov106 class descriptor data_ov106_020b8aa0, 0x020b8aa0-0x020b8ab4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b74c0, method 020b768c, 0x8eb4-byte state.
 */

extern void Ov106_StartScene(void);
extern void Ov106_TeardownScene(void);

GameClassDescriptor data_ov106_020b8aa0 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    0,  /* nClassId */
    13,  /* nGroupId */
    Ov106_StartScene,  /* pfnCtor */
    Ov106_TeardownScene,  /* pfnMethod */
    36532,  /* nAuxSize */
    0,  /* pArena */
};
