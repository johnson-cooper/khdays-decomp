/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_class_02090bd4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov008 class descriptor data_ov008_02090bd4, 0x02090bd4-0x02090be8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  This one is the mission menu (Ov008_MissionMenuCreate, 0x70-byte state).
 */

extern void Ov008_MissionMenuCreate(void);
extern void Ov008_MissionSceneDtor(void);

GameClassDescriptor data_ov008_02090bd4 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    14,  /* nGroupId */
    Ov008_MissionMenuCreate,  /* pfnCtor */
    Ov008_MissionSceneDtor,  /* pfnMethod */
    112,  /* nAuxSize */
    0,  /* pArena */
};
