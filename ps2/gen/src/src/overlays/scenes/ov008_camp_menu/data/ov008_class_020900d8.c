/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_class_020900d8.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov008 class descriptor data_ov008_020900d8, 0x020900d8-0x020900ec (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  This one is the tag tracker helper class (Ov008_Set_5d98, 8-byte state).
 */

extern void Ov008_CaptureRootHeap(void);
extern void Ov008_RootClassMethodNoOp(void);

GameClassDescriptor data_ov008_020900d8 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    0,  /* nClassId */
    14,  /* nGroupId */
    Ov008_CaptureRootHeap,  /* pfnCtor */
    Ov008_RootClassMethodNoOp,  /* pfnMethod */
    8,  /* nAuxSize */
    0,  /* pArena */
};
