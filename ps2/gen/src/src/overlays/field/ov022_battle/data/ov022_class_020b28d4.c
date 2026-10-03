/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/data/ov022_class_020b28d4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov022 class descriptor data_ov022_020b28d4, 0x020b28d4-0x020b28e8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 02084048, method 020840c8, 0x234-byte state.
 */

extern void Ov022_ResetAndBuildFromSeqArchive(void);
extern void func_ov022_020840c8(void);

GameClassDescriptor data_ov022_020b28d4 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    12,  /* nClassId */
    15,  /* nGroupId */
    Ov022_ResetAndBuildFromSeqArchive,  /* pfnCtor */
    func_ov022_020840c8,  /* pfnMethod */
    564,  /* nAuxSize */
    0,  /* pArena */
};
