/* PS2: mechanically prepared copy of src/overlays/players/ov091_player_sora_4/data/ov091_class_020bc194.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov091 class descriptor gOv091SoraClass, 0x020bc194-0x020bc1a8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020ba7d4, method 020ba804, 0x2e1c-byte state.
 */

extern void Ov091_stateCtorCondConfigReturnHandler(void);
extern void Ov091_initRegionCondClearGlobal(void);

GameClassDescriptor gOv091SoraClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov091_stateCtorCondConfigReturnHandler,  /* pfnCtor */
    Ov091_initRegionCondClearGlobal,  /* pfnMethod */
    11804,  /* nAuxSize */
    0,  /* pArena */
};
