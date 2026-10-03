/* PS2: mechanically prepared copy of src/overlays/players/ov044_player_xion/data/ov044_class_020b5500.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov044 class descriptor gOv044XionClass, 0x020b5500-0x020b5514 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b3234, method 020b32b4, 0x3a60-byte state.
 */

extern void Ov044_InitSubsystemAndReturnTick(void);
extern void Ov044_ShutdownAndFree(void);

GameClassDescriptor gOv044XionClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov044_InitSubsystemAndReturnTick,  /* pfnCtor */
    Ov044_ShutdownAndFree,  /* pfnMethod */
    14944,  /* nAuxSize */
    0,  /* pArena */
};
