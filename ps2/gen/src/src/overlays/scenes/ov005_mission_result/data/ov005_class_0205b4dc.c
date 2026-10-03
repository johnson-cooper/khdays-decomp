/* PS2: mechanically prepared copy of src/overlays/scenes/ov005_mission_result/data/ov005_class_0205b4dc.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov005 class descriptor gOv005MissionResultSceneClass, 0x0205b4dc-0x0205b4f0 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020515dc, method 020516b4, 0x14-byte state.
 */

extern void Ov005_EnterSceneWithMode(void);
extern void Ov005_TeardownMissionResult(void);

GameClassDescriptor gOv005MissionResultSceneClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    15,  /* nGroupId */
    Ov005_EnterSceneWithMode,  /* pfnCtor */
    Ov005_TeardownMissionResult,  /* pfnMethod */
    20,  /* nAuxSize */
    0,  /* pArena */
};
