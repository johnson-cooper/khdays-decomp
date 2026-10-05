/* PS2 debug override of Ov004_ReleaseSceneObjects (the ov004 child's destroy method): identical
 * logic, with a breadcrumb before every release step.  The DAY 255 -> field handoff stops with
 * EE interrupts disabled somewhere after the calendar parent returns -2; these marks (drawn live
 * on the TV during the handoff, ps2_probe.c) narrow it to one call. */

#include "nitro/types.h"
#include "game/engine.h"

extern char *data_ov004_02051384;

extern void Ov004_FreeResourceRecordBuffer(void *object);
extern void kh_debug_mark(const char *stage, int a, int b);

void Ov004_ReleaseSceneObjects(void)
{
    int i;
    int offset;

    kh_debug_mark("ov004 release: enter", (int)data_ov004_02051384,
                  *(int *)(data_ov004_02051384 + 0x559c));
    if (*(int *)(data_ov004_02051384 + 0x559c) != 0) {
        kh_debug_mark("ov004 release: text renderer", 0, 0);
        TileTextRenderer_Destroy(data_ov004_02051384 + 0x55ac);
        kh_debug_mark("ov004 release: font", 0, 0);
        FontResource_Destroy(data_ov004_02051384 + 0x55a0);
        *(int *)(data_ov004_02051384 + 0x559c) = 0;
    }

    offset = 0;
    for (i = 0; i < 10; i++) {
        kh_debug_mark("ov004 release: model", i, *(int *)(data_ov004_02051384 + offset + 0x74));
        ReleaseField74AndCleanup(data_ov004_02051384 + offset);
        offset += 0x108;
    }

    kh_debug_mark("ov004 release: sprite slots", 0, 0);
    Obj_Release(data_ov004_02051384 + 0xb0c);
    kh_debug_mark("ov004 release: resource record", *(int *)(data_ov004_02051384 + 0x558c), 0);
    Ov004_FreeResourceRecordBuffer(data_ov004_02051384 + 0x558c);

    *(volatile u32 *)((unsigned int)kh_ds_io + 0x0) &= ~0xe000;
    data_ov004_02051384 = 0;
    kh_debug_mark("ov004 release: done", 0, 0);
}
