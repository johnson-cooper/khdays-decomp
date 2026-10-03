/* PS2: mechanically prepared copy of src/overlays/players/ov037_player_larxene/data/ov037_class_020b4d94.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov037 class descriptor gOv037LarxeneClass, 0x020b4d94-0x020b4da8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b3234, method 020b325c, 0x2e54-byte state.
 */

extern void Ov037_CreateTaggedObjectHandler46Cf(void);
extern void Ov037_initSubitemsClear(void);

GameClassDescriptor gOv037LarxeneClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov037_CreateTaggedObjectHandler46Cf,  /* pfnCtor */
    Ov037_initSubitemsClear,  /* pfnMethod */
    11860,  /* nAuxSize */
    0,  /* pArena */
};
