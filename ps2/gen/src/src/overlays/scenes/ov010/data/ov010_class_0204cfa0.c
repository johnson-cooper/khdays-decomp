/* PS2: mechanically prepared copy of src/overlays/scenes/ov010/data/ov010_class_0204cfa0.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov010 class descriptor gOv010ConnectionErrorSceneClass, 0x0204cfa0-0x0204cfb4 (.data).
 *
 * InstantiateClass (02023930 / 02023960) builds a task object from it: the
 * u16 class / group ids, the constructor whose return is the first state
 * function, the method slot, the size of the zero-filled auxiliary block and
 * the arena reference.  Constructor 0204cb3c, method 0204cd80, 0x100-byte state.
 */

extern void Ov010_TitleSceneInit(void);
extern void Ov010_TeardownWorkArea(void);
extern int data_0204c024;

GameClassDescriptor gOv010ConnectionErrorSceneClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    0,  /* nClassId */
    15,  /* nGroupId */
    Ov010_TitleSceneInit,  /* pfnCtor */
    Ov010_TeardownWorkArea,  /* pfnMethod */
    256,  /* nAuxSize */
    &data_0204c024,  /* pArena */
};
