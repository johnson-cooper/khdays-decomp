/* PS2: mechanically prepared copy of src/overlays/players/ov088_player_axel_4/data/ov088_class_020bc260.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov088 class descriptor gOv088AxelClass, 0x020bc260-0x020bc274 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020ba7d4, method 020ba7fc, 0x2f40-byte state.
 */

extern void Ov088_ClassCtor(void);
extern void Ov088_ClassTeardown(void);

GameClassDescriptor gOv088AxelClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov088_ClassCtor,  /* pfnCtor */
    Ov088_ClassTeardown,  /* pfnMethod */
    12096,  /* nAuxSize */
    0,  /* pArena */
};
