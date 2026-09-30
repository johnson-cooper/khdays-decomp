/* PS2: mechanically prepared copy of src/overlays/scenes/ov004_calendar/Ov004_ReleaseSceneObjects.c (ps2/tools/prep_sources.py). Do not edit. */
/* Releases the calendar's text renderer, font, digit models, sprites and text, and turns the
 * windows off. */

#include "nitro/types.h"
#include "game/engine.h"

extern char *data_ov004_02051384;

extern void Ov004_FreeResourceRecordBuffer(void *object);

void Ov004_ReleaseSceneObjects(void) {
    int i;
    int offset;

    if (*(int *)(data_ov004_02051384 + 0x559c) != 0) {
        TileTextRenderer_Destroy(data_ov004_02051384 + 0x55ac);
        FontResource_Destroy(data_ov004_02051384 + 0x55a0);
        *(int *)(data_ov004_02051384 + 0x559c) = 0;
    }

    i = 0;
    offset = i;
    for (; i < 10; i++) {
        ReleaseField74AndCleanup(data_ov004_02051384 + offset);
        offset += 0x108;
    }

    Obj_Release(data_ov004_02051384 + 0xb0c);
    Ov004_FreeResourceRecordBuffer(data_ov004_02051384 + 0x558c);

    *(volatile u32 *)((unsigned int)kh_ds_io + 0x0) &= ~0xe000;
    data_ov004_02051384 = 0;
}

