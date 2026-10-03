/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_class_02090dc4.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov008 class descriptor data_ov008_02090dc4, 0x02090dc4-0x02090dd8 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  This one is the shop (Ov008_ShopCreate, class 8 / group 0xf, 0xc608-byte state).
 */

extern void Ov008_ShopCreate(void);
extern void Ov008_ClosePanel(void);

GameClassDescriptor data_ov008_02090dc4 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    8,  /* nClassId */
    15,  /* nGroupId */
    Ov008_ShopCreate,  /* pfnCtor */
    Ov008_ClosePanel,  /* pfnMethod */
    50696,  /* nAuxSize */
    0,  /* pArena */
};
