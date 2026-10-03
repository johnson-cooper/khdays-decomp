/* PS2: mechanically prepared copy of src/overlays/scenes/ov006_mission_mode_select/data/ov006_class_0205652c.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov006 class descriptor data_ov006_0205652c, 0x0205652c-0x02056540 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 0205454c, method 0205487c, 0x97f8-byte state.
 */

extern void Ov006_MissionSceneCtor(void);
extern void Ov006_MissionDestroyContext(void);

GameClassDescriptor data_ov006_0205652c __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    14,  /* nGroupId */
    Ov006_MissionSceneCtor,  /* pfnCtor */
    Ov006_MissionDestroyContext,  /* pfnMethod */
    38904,  /* nAuxSize */
    0,  /* pArena */
};
