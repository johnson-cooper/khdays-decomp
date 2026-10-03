/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/data/ov022_class_020b2930.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov022 class descriptor data_ov022_020b2930, 0x020b2930-0x020b2944 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 02088d18, method 02088db0, 0x24-byte state.
 */

extern void Ov022_AnimationRootCtor(void);
extern void Ov022_DestroyRootEntries(void);

GameClassDescriptor data_ov022_020b2930 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    11,  /* nClassId */
    6,  /* nGroupId */
    Ov022_AnimationRootCtor,  /* pfnCtor */
    Ov022_DestroyRootEntries,  /* pfnMethod */
    36,  /* nAuxSize */
    0,  /* pArena */
};
