/* PS2: mechanically prepared copy of src/overlays/players/ov032_player_xigbar/data/ov032_class_020b57f4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov032 class descriptor gOv032XigbarClass, 0x020b57f4-0x020b5808 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b3234, method 020b328c, 0x3020-byte state.
 */

extern void Ov032_InitAndGetHandler(void);
extern void Ov032_UnloadEnemyOverlay(void);

GameClassDescriptor gOv032XigbarClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov032_InitAndGetHandler,  /* pfnCtor */
    Ov032_UnloadEnemyOverlay,  /* pfnMethod */
    12320,  /* nAuxSize */
    0,  /* pArena */
};
