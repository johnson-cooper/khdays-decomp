/* PS2: mechanically prepared copy of src/overlays/players/ov047_player_donald/data/ov047_class_020b42f4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov047 class descriptor gOv047DonaldClass, 0x020b42f4-0x020b4308 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 020b3234, method 020b325c, 0x2ee0-byte state.
 */

extern void Ov047_stateCtorReturnHandler(void);
extern void Ov047_setupTriple(void);

GameClassDescriptor gOv047DonaldClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,  /* nClassId */
    6,  /* nGroupId */
    Ov047_stateCtorReturnHandler,  /* pfnCtor */
    Ov047_setupTriple,  /* pfnMethod */
    12000,  /* nAuxSize */
    0,  /* pArena */
};
