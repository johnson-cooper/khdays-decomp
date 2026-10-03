/* PS2: mechanically prepared copy of src/overlays/scenes/ov006_mission_mode_select/data/ov006_class_020563e4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov006 class descriptor data_ov006_020563e4, 0x020563e4-0x020563f8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020500c4, method 0205028c, 0x70-byte state.
 */

extern void Ov006_CreateMissionMenu(void);
extern void Ov006_MissionSceneDtor(void);

GameClassDescriptor data_ov006_020563e4 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    14,  /* nGroupId */
    Ov006_CreateMissionMenu,  /* pfnCtor */
    Ov006_MissionSceneDtor,  /* pfnMethod */
    112,  /* nAuxSize */
    0,  /* pArena */
};
