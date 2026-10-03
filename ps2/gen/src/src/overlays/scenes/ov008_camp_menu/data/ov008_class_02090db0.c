/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_class_02090db0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov008 class descriptor data_ov008_02090db0, 0x02090db0-0x02090dc4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  This one is the sub-object helper class (Ov008_CreateSubObject, 8-byte state).
 */

extern void Ov008_CreateSubObject(void);
extern void Ov008_MissionScene_Release(void);

GameClassDescriptor data_ov008_02090db0 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    14,  /* nGroupId */
    Ov008_CreateSubObject,  /* pfnCtor */
    Ov008_MissionScene_Release,  /* pfnMethod */
    8,  /* nAuxSize */
    0,  /* pArena */
};
