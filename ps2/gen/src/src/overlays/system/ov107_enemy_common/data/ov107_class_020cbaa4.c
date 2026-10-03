/* PS2: mechanically prepared copy of src/overlays/system/ov107_enemy_common/data/ov107_class_020cbaa4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov107 class descriptor data_ov107_020cbaa4, 0x020cbaa4-0x020cbab8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020c9960, method 020c9a10, 0x90-byte state.
 */

extern void Ov107_SetupActorManager(void);
extern void Ov107_FieldClassTeardown(void);

GameClassDescriptor data_ov107_020cbaa4 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    16,  /* nClassId */
    11,  /* nGroupId */
    Ov107_SetupActorManager,  /* pfnCtor */
    Ov107_FieldClassTeardown,  /* pfnMethod */
    144,  /* nAuxSize */
    0,  /* pArena */
};
