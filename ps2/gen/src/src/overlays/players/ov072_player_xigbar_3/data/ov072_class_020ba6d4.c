/* PS2: mechanically prepared copy of src/overlays/players/ov072_player_xigbar_3/data/ov072_class_020ba6d4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov072 class descriptor gOv072XigbarClass, 0x020ba6d4-0x020ba6e8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b8114, method 020b816c, 0x3020-byte state.
 */

extern void Ov072_InitAndGetHandler(void);
extern void Ov072_UnloadEnemyOverlay(void);

GameClassDescriptor gOv072XigbarClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov072_InitAndGetHandler,  /* pfnCtor */
    Ov072_UnloadEnemyOverlay,  /* pfnMethod */
    12320,  /* nAuxSize */
    0,  /* pArena */
};
