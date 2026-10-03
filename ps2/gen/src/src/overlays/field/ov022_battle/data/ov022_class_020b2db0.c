/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/data/ov022_class_020b2db0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov022 class descriptor data_ov022_020b2db0, 0x020b2db0-0x020b2dc4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b0f2c, method 020b0ff8, 0x2bc-byte state.
 */

extern void Ov022_InitSyncSession(void);
extern void func_ov022_020b0ff8(void);

GameClassDescriptor data_ov022_020b2db0 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    6,  /* nGroupId */
    Ov022_InitSyncSession,  /* pfnCtor */
    func_ov022_020b0ff8,  /* pfnMethod */
    700,  /* nAuxSize */
    0,  /* pArena */
};
