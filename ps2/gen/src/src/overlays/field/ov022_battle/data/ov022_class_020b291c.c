/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/data/ov022_class_020b291c.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov022 class descriptor data_ov022_020b291c, 0x020b291c-0x020b2930 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 02086f44, method 020871fc, 0xe4-byte state.
 */

extern void Ov022_ResetCameraState(void);
extern void Ov022_UnloadParty(void);

GameClassDescriptor data_ov022_020b291c __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    9,  /* nClassId */
    6,  /* nGroupId */
    Ov022_ResetCameraState,  /* pfnCtor */
    Ov022_UnloadParty,  /* pfnMethod */
    228,  /* nAuxSize */
    0,  /* pArena */
};
