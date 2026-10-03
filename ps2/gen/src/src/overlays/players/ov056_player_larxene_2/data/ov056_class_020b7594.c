/* PS2: mechanically prepared copy of src/overlays/players/ov056_player_larxene_2/data/ov056_class_020b7594.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov056 class descriptor gOv056LarxeneClass, 0x020b7594-0x020b75a8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b5a34, method 020b5a5c, 0x2e54-byte state.
 */

extern void Ov056_CreateTaggedObjectHandler46Cf(void);
extern void Ov056_initSubitemsClear(void);

GameClassDescriptor gOv056LarxeneClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov056_CreateTaggedObjectHandler46Cf,  /* pfnCtor */
    Ov056_initSubitemsClear,  /* pfnMethod */
    11860,  /* nAuxSize */
    0,  /* pArena */
};
