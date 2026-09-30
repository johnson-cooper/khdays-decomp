/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/Ov000_BootFadeAndSubScreenSetup.c (ps2/tools/prep_sources.py). Do not edit. */
/* Boot step: draws the logo and fades the sub screen in, then sets up the sub screen backgrounds,
 * releases the text engine and creates the next scene. */

#include "nitro/types.h"

extern int NNSi_FndGetCurrentRootHeap(void);
extern void Camera_CommitMatricesEx(int p, int a, int b, int c, int d);
extern void Scene_DrawNode(unsigned short *p);
extern void SetMasterBrightnessSub(int n);
extern void Slot_UnlinkAll(int *p);
extern void DispObjList_UpdateQueued(int *p);
extern void Obj_CommitAllSlots(int p);
extern void Obj_Release(int *p);
extern void *InstantiateClass(void *class_desc, int arg);
extern int data_ov000_0205aa34;
extern void Ov000_TickBootTeardown(void);

typedef struct Ov000SceneNextField {
    int value;
} Ov000SceneNextField;

typedef struct Ov000BootContext {
    int frame;
    u8 pad_0004[8];
    u8 renderNode[0x108];
    u8 scrollBounds[0x9c];
    u8 textEngine[1];
    u8 pad_01b1[0x4a7c];
    s8 savedSlotIndex;
    s8 savedSceneIds[3];
    s8 resumeBoot;
    u8 pad_4c32[0x43a];
    void *sceneInstance;
    Ov000SceneNextField sceneNext;
} Ov000BootContext;

int Ov000_BootFadeAndSubScreenSetup(void) {
    register int zero;
    Ov000BootContext *context =
        (Ov000BootContext *)NNSi_FndGetCurrentRootHeap();
    volatile unsigned short *bg = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x1008);

    Camera_CommitMatricesEx((int)context->scrollBounds, 0x3b33, -0x3b33, -0x4d9a,
                  0x4d9a);
    Scene_DrawNode((u16 *)context->renderNode);

    if (context->frame <= 0x10) {
        SetMasterBrightnessSub(-context->frame);
    } else {
        short base = 0x4004;
        bg[0] = bg[0] & 0x43 | base;
        bg[1] = bg[1] & 0x43 | (base + 0x200);
        bg[2] = bg[2] & 0x43 | (base + 0x400);
        bg[3] = bg[3] & 0x43 | (base - 0x3a00);
        *(volatile unsigned int *)((unsigned int)kh_ds_io + 0x1000) =
            *(volatile unsigned int *)((unsigned int)kh_ds_io + 0x1000) & ~0x1f00 | 0x1f00;
        bg[1] = bg[1] & ~3 | 3;
        bg[0] = bg[0] & ~3 | 2;
        bg[2] = bg[2] & ~3 | 1;
        bg[3] = bg[3] & ~3;

        Slot_UnlinkAll((int *)context->textEngine);
        DispObjList_UpdateQueued((int *)context->textEngine);
        Obj_CommitAllSlots((int)context->textEngine);
        Obj_Release((int *)context->textEngine);
        SetMasterBrightnessSub(-0x10);

        if (context->resumeBoot == 1) {
            context->sceneInstance =
                InstantiateClass((void *)&data_ov000_0205aa34,
                              context->savedSceneIds[context->savedSlotIndex] +
                                  1);
        } else {
            context->sceneInstance =
                InstantiateClass((void *)&data_ov000_0205aa34, 0);
        }
        zero = 0;
        context->sceneNext.value = zero;
        context->frame = zero;
        return (int)Ov000_TickBootTeardown;
    }
    context->frame = context->frame + 1;
    return 0;
}
