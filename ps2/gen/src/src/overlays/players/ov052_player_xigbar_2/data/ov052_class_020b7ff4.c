/* PS2: mechanically prepared copy of src/overlays/players/ov052_player_xigbar_2/data/ov052_class_020b7ff4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov052 class descriptor gOv052XigbarClass, 0x020b7ff4-0x020b8008 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b5a34, method 020b5a8c, 0x3020-byte state.
 */

extern void Ov052_InitAndGetHandler(void);
extern void Ov052_UnloadEnemyOverlay(void);

GameClassDescriptor gOv052XigbarClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov052_InitAndGetHandler,  /* pfnCtor */
    Ov052_UnloadEnemyOverlay,  /* pfnMethod */
    12320,  /* nAuxSize */
    0,  /* pArena */
};
