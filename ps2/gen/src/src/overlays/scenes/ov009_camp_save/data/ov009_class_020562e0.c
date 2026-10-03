/* PS2: mechanically prepared copy of src/overlays/scenes/ov009_camp_save/data/ov009_class_020562e0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov009 class descriptor data_ov009_020562e0, 0x020562e0-0x020562f4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 02052834, method 02052854, 0x8-byte state.
 */

extern void Ov009_CaptureRootHeap(void);
extern void Ov009_RootClassMethodNoOp(void);

GameClassDescriptor data_ov009_020562e0 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    0,  /* nClassId */
    14,  /* nGroupId */
    Ov009_CaptureRootHeap,  /* pfnCtor */
    Ov009_RootClassMethodNoOp,  /* pfnMethod */
    8,  /* nAuxSize */
    0,  /* pArena */
};
