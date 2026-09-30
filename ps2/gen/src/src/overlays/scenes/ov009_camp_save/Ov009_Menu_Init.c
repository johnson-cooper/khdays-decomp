/* PS2: mechanically prepared copy of src/overlays/scenes/ov009_camp_save/Ov009_Menu_Init.c (ps2/tools/prep_sources.py). Do not edit. */
/* Allocates the menu context, opens the message containers, starts touch sampling, creates the
 * graphics objects, tile buffers and subsystems. */

#include "nitro/types.h"

typedef struct Ov009ObjectTemplate {
    u32 words[5];
} Ov009ObjectTemplate;

typedef struct Ov009HeaderLimits {
    u16 first;
    u16 second;
} Ov009HeaderLimits;

typedef struct Ov009MenuContext {
    u8 subsystem0[0x4a80];
    u8 subsystem1[0x9500 - 0x4a80];
    u8 object9500[0x4c];
    u8 object954c[0x4c];
    void *graphicsObject;
    u8 pad_959c[0x95a4 - 0x959c];
    void *tileBuffers[7];
    u8 pad_95c0[0x95c0 - (0x95a4 + 7 * 4)];
    void *field_95c0;
    u8 pad_95c4[0x95d0 - 0x95c4];
    int tileGridWidth;
    int brightness;
    u8 brightnessTween[0x1c];
    u8 pad_95f4[0x9600 - 0x95f4];
    int pageAEnabled;
    int pageBEnabled;
    u8 pad_9608[0x9618 - 0x9608];
    int updateTaskPending;
    int blitPending;
    u8 pad_9620[0x9628 - 0x9620];
    int field_9628;
    int field_962c;
    u8 pad_9630[0x963e - 0x9630];
    u16 inputHeader;
    u8 pad_9640[0x9660 - 0x9640];
    u8 activeWidgetList[0x14];
    int subDisplayMode;
    u8 pad_9678[0x96b0 - 0x9678];
    void *primaryMessageContainer;
    void *secondaryMessageContainer;
    void *tertiaryMessageContainer;
} Ov009MenuContext;

extern const Ov009ObjectTemplate data_ov009_02055f58;
extern const Ov009HeaderLimits data_ov009_02055f54;
extern Ov009MenuContext *data_ov009_020563e4[2];
extern const char gOv009UiCmCmPath[];
extern const char gOv009UiCmCmbPath[];
extern const char data_ov009_020562c0[];
extern const char data_ov009_020562e0[];

extern void *NNSi_FndAllocFromDefaultExpHeap(u32 size);
extern void  MI_CpuFill8(void *destination, int value, u32 size);
extern void  LoadOverlaySync(int mode, int overlayId);
extern void *Msg_OpenContainerAndReadHeader(const void *descriptor, int heapId);
extern int   GetLanguage(void);
extern void  Touch_StartAutoSampling(void);
extern void *InstantiateClass(const void *descriptor, int value);
extern void  NNS_FndInitList(void *list, int linkOffset);
extern void  Ov009_PushCannedDescriptors(void);
extern void  Ov009_ClearActiveHandlers(void);
extern void *NNS_FndAllocFromDefaultExpHeapEx(u32 size, int alignment);
extern void  MIi_CpuClear16(int value, void *destination, u32 size);
extern void  Tween_Clear(void *tween);
extern void  Ov009_Container_Init(
    void *object, const Ov009ObjectTemplate *objectTemplate);
extern void  Ov009_InitSubsystemObject(void *object, int value);
extern void  Ov009_EnableBothHalves(int value);
extern void  Header_InitWithLimits(
    u16 *inputHeader, const Ov009HeaderLimits *limits);
extern void  Ov009_StartScreenTransition(int first, int second);
extern void  Ov009_MenuInitHookNoOp(int enabled);

static volatile u32 *const REG_DISPCNT_SUB =
    (volatile u32 *)((unsigned int)kh_ds_io + 0x1000);

void Ov009_Menu_Init(void)
{
    Ov009ObjectTemplate objectTemplate = data_ov009_02055f58;
    Ov009HeaderLimits headerLimits = data_ov009_02055f54;
    int isModeOne;
    int index;

    data_ov009_020563e4[0] = 0;
    data_ov009_020563e4[1] =
        NNSi_FndAllocFromDefaultExpHeap(0x976c);
    MI_CpuFill8(data_ov009_020563e4[1], 0, 0x976c);

    data_ov009_020563e4[1]->pageAEnabled = 1;
    data_ov009_020563e4[1]->pageBEnabled = 1;

    LoadOverlaySync(0, 0x12e);
    data_ov009_020563e4[1]->primaryMessageContainer =
        Msg_OpenContainerAndReadHeader(gOv009UiCmCmPath, 14);
    isModeOne = GetLanguage() == 1;
    if (isModeOne == 0) {
        data_ov009_020563e4[1]->secondaryMessageContainer =
            Msg_OpenContainerAndReadHeader(gOv009UiCmCmbPath, 14);
    }
    data_ov009_020563e4[1]->tertiaryMessageContainer =
        Msg_OpenContainerAndReadHeader(data_ov009_020562c0, 14);

    Touch_StartAutoSampling();
    data_ov009_020563e4[1]->graphicsObject =
        InstantiateClass(data_ov009_020562e0, 0);
    data_ov009_020563e4[1]->tileGridWidth = 0x40;
    data_ov009_020563e4[1]->field_9628 = 1;
    data_ov009_020563e4[1]->field_962c = 1;
    NNS_FndInitList(
        data_ov009_020563e4[1]->activeWidgetList, 4);

    data_ov009_020563e4[1]->brightness = -0x10000;
    data_ov009_020563e4[1]->blitPending = 1;
    Ov009_PushCannedDescriptors();

    data_ov009_020563e4[1]->subDisplayMode =
        (*REG_DISPCNT_SUB & 0xe000) >> 13;
    Ov009_ClearActiveHandlers();

    for (index = 0; index < 7; index++) {
        data_ov009_020563e4[1]->tileBuffers[index] =
            NNS_FndAllocFromDefaultExpHeapEx(
                data_ov009_020563e4[1]->tileGridWidth << 6, 2);
        MIi_CpuClear16(
            0,
            data_ov009_020563e4[1]->tileBuffers[index],
            data_ov009_020563e4[1]->tileGridWidth << 6);
    }

    Tween_Clear(data_ov009_020563e4[1]->brightnessTween);
    Ov009_Container_Init(
        data_ov009_020563e4[1]->object9500, &objectTemplate);
    Ov009_Container_Init(
        data_ov009_020563e4[1]->object954c, &objectTemplate);
    Ov009_InitSubsystemObject(data_ov009_020563e4[1]->subsystem0, 0);
    Ov009_InitSubsystemObject(data_ov009_020563e4[1]->subsystem1, 0);
    Ov009_EnableBothHalves(0);
    Header_InitWithLimits(
        &data_ov009_020563e4[1]->inputHeader, &headerLimits);
    Ov009_StartScreenTransition(0, -1);
    data_ov009_020563e4[1]->field_95c0 = 0;
    Ov009_MenuInitHookNoOp(1);
    data_ov009_020563e4[1]->updateTaskPending = 1;
}
