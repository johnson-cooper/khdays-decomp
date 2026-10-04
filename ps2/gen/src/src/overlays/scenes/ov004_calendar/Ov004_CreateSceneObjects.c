/* PS2: mechanically prepared copy of src/overlays/scenes/ov004_calendar/Ov004_CreateSceneObjects.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Creates the calendar's objects: records the start and target day (as rolling values), loads its
 * text and graphics, and starts the first state. */

#include "nitro/types.h"

extern char *data_ov004_02051384;
extern char gOv004UiCalTtlPath[];

extern char *NNSi_FndGetCurrentRootHeap(void);
extern void MI_CpuFill8(void *dst, int value, u32 size);
extern void MI_CpuCopy8(void *src, void *dst, u32 size);
extern void Ov004_InitResourceRecord(void *object, void *resource);
extern void Ov004_LoadSceneGraphics(void);
extern s64 OS_GetTick(void);
extern void *Ov004_StepSceneFrame(void);

void *Ov004_CreateSceneObjects(void *args) {
    char *context;
    int currentFx;

    context = NNSi_FndGetCurrentRootHeap();
    data_ov004_02051384 = context;
    MI_CpuFill8(context, 0, 0x5618);
    MI_CpuCopy8(args, context + 0x5558, 8);

    *(int *)(context + 0x5598) = *(int *)(context + 0x555c);
    if (*(int *)(context + 0x5558) == 0x190) {
        *(int *)(context + 0x555c) = 0xff;
        *(int *)(context + 0x5558) = 0xff;
    }

    currentFx = *(int *)(context + 0x5558) << 12;
    *(int *)(context + 0x5568) = currentFx;
    *(int *)(context + 0x5560) = currentFx;
    *(int *)(context + 0x556c) = *(int *)(context + 0x555c) << 12;
    *(int *)(context + 0x5564) =
        *(int *)(context + 0x556c) - *(int *)(context + 0x5560);

    Ov004_InitResourceRecord(context + 0x558c, gOv004UiCalTtlPath);
    Ov004_LoadSceneGraphics();

    *(int *)(context + 0xb08) = 0;
    *(int *)(context + 0xaf8) = 0;
    *(kh_unaligned_s64 *)(context + 0xb00) = OS_GetTick();

    return (void *)Ov004_StepSceneFrame;
}

