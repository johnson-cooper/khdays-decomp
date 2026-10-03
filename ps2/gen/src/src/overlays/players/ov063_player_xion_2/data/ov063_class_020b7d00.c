/* PS2: mechanically prepared copy of src/overlays/players/ov063_player_xion_2/data/ov063_class_020b7d00.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov063 class descriptor gOv063XionClass, 0x020b7d00-0x020b7d14 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b5a34, method 020b5ab4, 0x3a60-byte state.
 */

extern void Ov063_InitSubsystemAndReturnTick(void);
extern void Ov063_ShutdownAndFree(void);

GameClassDescriptor gOv063XionClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov063_InitSubsystemAndReturnTick,  /* pfnCtor */
    Ov063_ShutdownAndFree,  /* pfnMethod */
    14944,  /* nAuxSize */
    0,  /* pArena */
};
