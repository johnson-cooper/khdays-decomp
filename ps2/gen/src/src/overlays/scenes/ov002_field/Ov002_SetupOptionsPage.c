/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_SetupOptionsPage.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
#include "game/engine.h"

extern char *Ov002_Field_GetBlock194(void);
extern char *NNSi_FndGetCurrentRootHeap(void);
extern void MI_CpuFill8(void *dst, int value, unsigned size);
extern void Ov002_BuildOptionsPage(void);
extern int InstantiateClass(void *desc, int a);
extern void *NNS_FndAllocFromDefaultExpHeapEx(unsigned size, int align);
extern int Ov002_ForwardToSubDc(int id);
extern void Ov002_ForwardWithContext(int a, int b, int c, int d, int e, int f, void *cb);
extern int Ov002_Ctx_FindActiveEntryByTag(int id);
extern void Ov002_Ctx_SetTagTrackerNodeArmed_5(int handle, int a);
extern void Ov002_World_SetElementListHalf20(void);
extern void Ov002_RequestPageAdvance(void);
extern void Ov002_RequestPageRetry(void);
extern void Ov002_StepPageClose(void);
extern char *data_ov002_0207f634;
extern int data_ov002_0207edf4;

/* Sets the options page up: clears the state, builds the page, allocates the two 0x20-byte
 * scratch blocks and -- on the debug build -- adds the two extra rows. */
void *Ov002_SetupOptionsPage(void) {
    char *page = Ov002_Field_GetBlock194();
    char *self = NNSi_FndGetCurrentRootHeap();
    data_ov002_0207f634 = self;
    MI_CpuFill8(self, 0, 0x3c);
    Ov002_BuildOptionsPage();
    *(int *)self = InstantiateClass(&data_ov002_0207edf4, 0);
    *(void **)(self + 8) = NNS_FndAllocFromDefaultExpHeapEx(0x20, 4);
    *(void **)(self + 0xc) = NNS_FndAllocFromDefaultExpHeapEx(0x20, 4);
    *(kh_unaligned_s64 *)(page + 0x18) = 0x6646;
    if (Session_IsActive() != 0) {
        Ov002_ForwardWithContext(Ov002_ForwardToSubDc(0x3eb), 0, 0x60, 0x10,
                            0x30, 0xffff, (void *)&Ov002_RequestPageAdvance);
        Ov002_ForwardWithContext(Ov002_ForwardToSubDc(0x3f9), 0xf0, 0x18, 0x10,
                            0x18, 0xffff, (void *)&Ov002_RequestPageRetry);
        Ov002_Ctx_SetTagTrackerNodeArmed_5(Ov002_Ctx_FindActiveEntryByTag(4), 1);
    }
    Ov002_World_SetElementListHalf20();
    return (void *)&Ov002_StepPageClose;
}
