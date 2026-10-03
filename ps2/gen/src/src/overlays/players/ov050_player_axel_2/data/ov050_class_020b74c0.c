/* PS2: mechanically prepared copy of src/overlays/players/ov050_player_axel_2/data/ov050_class_020b74c0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov050 class descriptor gOv050AxelClass, 0x020b74c0-0x020b74d4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b5a34, method 020b5a5c, 0x2f40-byte state.
 */

extern void Ov050_ClassCtor(void);
extern void Ov050_ClassTeardown(void);

GameClassDescriptor gOv050AxelClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov050_ClassCtor,  /* pfnCtor */
    Ov050_ClassTeardown,  /* pfnMethod */
    12096,  /* nAuxSize */
    0,  /* pArena */
};
