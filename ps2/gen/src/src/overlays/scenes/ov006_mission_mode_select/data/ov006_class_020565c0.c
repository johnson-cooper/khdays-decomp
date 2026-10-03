/* PS2: mechanically prepared copy of src/overlays/scenes/ov006_mission_mode_select/data/ov006_class_020565c0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov006 class descriptor gOv006MissionSelectSceneClass, 0x020565c0-0x020565e0 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the u16 class / group
 * ids, the constructor whose return is the first state function, the method slot, the size of
 * the zero-filled auxiliary block and the arena reference.  Constructor 020560cc, method
 * 02056118, 8-byte state.  The dsd symbol also covers the three zero words that pad the unit's
 * .data to its 32-byte end.
 */

extern void Ov006_CreateSubObject(void);
extern void Ov006_MissionBootWatchdog(void);

struct {
    GameClassDescriptor desc;
    int reserved[3];
} gOv006MissionSelectSceneClass __attribute__((aligned(4))) = {
    { 8, 14, Ov006_CreateSubObject, Ov006_MissionBootWatchdog, 8, 0 },
    { 0, 0, 0 },
};
