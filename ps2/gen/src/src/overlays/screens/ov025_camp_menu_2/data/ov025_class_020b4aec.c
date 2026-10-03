/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_class_020b4aec.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov025 class descriptor data_ov025_020b4aec, 0x020b4aec-0x020b4b00 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  This one is the tag tracker helper class (class 0 / group 0xe, 020899a8 / 020899c8, 8-byte state), instantiated by Ov025_SetupContext 02083e84.
 */

extern void Ov025_CaptureRootHeap(void);
extern void Ov025_RootClassMethodNoOp(void);

GameClassDescriptor data_ov025_020b4aec __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    0,  /* nClassId */
    14,  /* nGroupId */
    Ov025_CaptureRootHeap,  /* pfnCtor */
    Ov025_RootClassMethodNoOp,  /* pfnMethod */
    8,  /* nAuxSize */
    0,  /* pArena */
};
