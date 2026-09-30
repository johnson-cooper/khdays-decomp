/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/Ov000_TickFadeOutToScene.c (ps2/tools/prep_sources.py). Do not edit. */
/* Fade-out tick: run the darken, then on the last frame reprogram engine-B BG registers, kick the
 * manager, load the next resource, and hand off to Ov000_WaitSubMenuResult.
 *
 * While nStateFrame <= 16 it just drives SetMasterBrightnessSub(-frame). On the frame it passes 16
 * it rewrites the engine-B display registers (0x0400100a/0e = BG1/BG3CNT, 0x04001000 = DISPCNT:
 * BG mode field -> 0x1a), sets brightness to full-dark, runs the four manager ticks, creates the
 * next scene resource, and returns the next scene callback. Same 0xd18c Ov000SceneContext; +0 read
 * as the 32-bit frame counter.
 */

#include "nitro/types.h"

typedef void (*OverlayCallback)(void);

typedef struct {
    int nStateFrame;        /* +0x00 */
    u8 pad_0004[8];
    u8 renderNode[0x1a4];   /* +0x0c */
    u8 manager[0x4ebc];     /* +0x1b0 */
    int nResourceState;     /* +0x506c */
    void *pResource;        /* +0x5070 */
} Ov000SceneContext;

extern u8 data_ov000_0205ab0c[];
extern Ov000SceneContext *NNSi_FndGetCurrentRootHeap(void);
extern void Scene_DrawNode(void *object);
extern void SetMasterBrightnessSub(int value);
extern void Slot_UnlinkAll(void *manager);
extern void DispObjList_UpdateQueued(void *manager);
extern void Obj_CommitAllSlots(void *manager);
extern void Obj_Release(void *manager);
extern void *InstantiateClass(const void *descriptor, int argument);
extern void Ov000_WaitSubMenuResult(void);

OverlayCallback Ov000_TickFadeOutToScene(void) {
    Ov000SceneContext *context = NNSi_FndGetCurrentRootHeap();

    Scene_DrawNode(context->renderNode);
    if (context->nStateFrame <= 16) {
        SetMasterBrightnessSub(-context->nStateFrame);
    } else {
        *(volatile u16 *)((unsigned int)kh_ds_io + 0x100a) =
            (*(volatile u16 *)((unsigned int)kh_ds_io + 0x100a) & 0x43) | 0x4008;
        *(volatile u16 *)((unsigned int)kh_ds_io + 0x100e) =
            (*(volatile u16 *)((unsigned int)kh_ds_io + 0x100e) & 0x43) | 0x210;
        *(volatile u32 *)((unsigned int)kh_ds_io + 0x1000) =
            (*(volatile u32 *)((unsigned int)kh_ds_io + 0x1000) & ~0x1f00) | 0x1a00;
        *(volatile u16 *)((unsigned int)kh_ds_io + 0x100a) =
            (*(volatile u16 *)((unsigned int)kh_ds_io + 0x100a) & ~3) | 3;
        *(volatile u16 *)((unsigned int)kh_ds_io + 0x100e) &= ~3;

        Slot_UnlinkAll(context->manager);
        DispObjList_UpdateQueued(context->manager);
        Obj_CommitAllSlots(context->manager);
        Obj_Release(context->manager);
        SetMasterBrightnessSub(-16);

        context->pResource = InstantiateClass(data_ov000_0205ab0c, 0);
        context->nResourceState = 0;
        context->nStateFrame = 0;
        return Ov000_WaitSubMenuResult;
    }

    context->nStateFrame++;
    return 0;
}
