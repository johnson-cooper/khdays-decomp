/* PS2: mechanically prepared copy of src/overlays/screens/ov027_game_over/data/ov027_class_02083ef8.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov027 class descriptor data_ov027_02083ef8, 0x02083ef8-0x02083f0c (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 02082bec, method 02082cd8, 0x630-byte state.
 */

extern void Ov027_InitScene(void);
extern void Ov027_ExitScene(void);

GameClassDescriptor data_ov027_02083ef8 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    0,  /* nClassId */
    15,  /* nGroupId */
    Ov027_InitScene,  /* pfnCtor */
    Ov027_ExitScene,  /* pfnMethod */
    1584,  /* nAuxSize */
    0,  /* pArena */
};
