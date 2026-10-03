/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_class_02090bb0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov008 class descriptor data_ov008_02090bb0, 0x02090bb0-0x02090bc4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  This one is the mission lobby (Ov008_MissionLobbyInit, 0x500-byte state).
 */

extern void Ov008_MissionLobbyInit(void);
extern void Ov008_FireExitMessage(void);

GameClassDescriptor data_ov008_02090bb0 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    14,  /* nGroupId */
    Ov008_MissionLobbyInit,  /* pfnCtor */
    Ov008_FireExitMessage,  /* pfnMethod */
    1280,  /* nAuxSize */
    0,  /* pArena */
};
