/* PS2: mechanically prepared copy of src/overlays/players/ov099_player_xion_4/data/ov099_class_020bcaa0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov099 class descriptor gOv099XionClass, 0x020bcaa0-0x020bcab4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020ba7d4, method 020ba854, 0x3a60-byte state.
 */

extern void Ov099_InitSubsystemAndReturnTick(void);
extern void Ov099_ShutdownAndFree(void);

GameClassDescriptor gOv099XionClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov099_InitSubsystemAndReturnTick,  /* pfnCtor */
    Ov099_ShutdownAndFree,  /* pfnMethod */
    14944,  /* nAuxSize */
    0,  /* pArena */
};
