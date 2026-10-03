/* PS2: mechanically prepared copy of src/overlays/field/ov023_event_script/data/ov023_class_0208a748.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov023 class descriptor data_ov023_0208a748, 0x0208a748-0x0208a75c (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020896b8, method 020896f0, 0x84-byte state.
 */

extern void Ov023_SubSceneEnter(void);
extern void Ov023_ClassTeardown(void);

GameClassDescriptor data_ov023_0208a748 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    62,  /* nClassId */
    13,  /* nGroupId */
    Ov023_SubSceneEnter,  /* pfnCtor */
    Ov023_ClassTeardown,  /* pfnMethod */
    132,  /* nAuxSize */
    0,  /* pArena */
};
