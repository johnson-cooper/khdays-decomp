/* PS2: mechanically prepared copy of src/overlays/players/ov038_player_lexaeus/data/ov038_class_020b4bd4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov038 class descriptor gOv038LexaeusClass, 0x020b4bd4-0x020b4be8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b3234, method 020b325c, 0x3180-byte state.
 */

extern void Ov038_initSubitemsClear(void);
extern void Ov038_setupTriple(void);

GameClassDescriptor gOv038LexaeusClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov038_initSubitemsClear,  /* pfnCtor */
    Ov038_setupTriple,  /* pfnMethod */
    12672,  /* nAuxSize */
    0,  /* pArena */
};
