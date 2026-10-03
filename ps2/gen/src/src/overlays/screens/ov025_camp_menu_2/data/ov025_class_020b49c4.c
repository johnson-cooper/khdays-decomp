/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_class_020b49c4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov025 class descriptor data_ov025_020b49c4, 0x020b49c4-0x020b49d8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  This one is the records scene task (class 8 / group 0xd, constructor Ov025_AllocContextAndGetHandler 02082960, method 02082998, 4-byte state); registered by the scene hook 02082b1c (02023930) with its handle kept in data_ov025_020b49c0.
 */

extern void Ov025_AllocContextAndGetHandler(void);
extern void Ov025_MenuExit(void);

GameClassDescriptor data_ov025_020b49c4 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    13,  /* nGroupId */
    Ov025_AllocContextAndGetHandler,  /* pfnCtor */
    Ov025_MenuExit,  /* pfnMethod */
    4,  /* nAuxSize */
    0,  /* pArena */
};
