/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/data/ov022_class_020b2894.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov022 class descriptor data_ov022_020b2894, 0x020b2894-0x020b28a8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 02082a40, method 02082b84, 0x40-byte state.
 */

extern void Ov022_BeginScene(void);
extern void func_ov022_02082b84(void);

GameClassDescriptor data_ov022_020b2894 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    15,  /* nGroupId */
    Ov022_BeginScene,  /* pfnCtor */
    func_ov022_02082b84,  /* pfnMethod */
    64,  /* nAuxSize */
    0,  /* pArena */
};
