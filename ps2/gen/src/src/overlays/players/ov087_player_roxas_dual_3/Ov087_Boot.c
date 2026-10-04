/* PS2: mechanically prepared copy of src/overlays/players/ov087_player_roxas_dual_3/Ov087_Boot.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Ov087_Boot -- overlay boot of the ov049 enemy (x4: ov049/068/087/104): the ov042 shape (see
 * Ov042_BuildRigObject for the load-bearing forms): grabs the root heap block as the object, latches
 * it in the overlay global, copies the identity fields from the caller's config, opens the slot
 * with the {1, 0xf00, 0x900} parameter block, binds the animation table -- the alternate one when the config's flag at +0x18 is set, fills the handler vtable
 * at +0x664, attaches the scene node, invalidates the five bone handles and resolves 5 of them by
 * name, folds the three optional capability bits into the 64-bit flag word and hands the object
 * over. */

#include "game/engine.h"

extern void *NNSi_FndGetCurrentRootHeap(void);
extern int  NNS_G3dGetResDictIdxByName(void *node, void *desc);
extern void Ov022_InitActor(void *obj);

extern void Ov087_ApplyModeChange(void);
extern void Ov087_UpdateAttachmentAndChannels(void);
extern void Ov087_InvokeSubHandlerPairWithGlobalBuffer(void);
extern void Ov087_ReloadTwoSceneResources(void);
extern void Ov087_MapSlotKindToAnim(void);
extern void Ov087_HandleMsgAndFaceHeading(void);
extern void Ov087_BuildRenderHandles(void);
extern void Ov087_StoreArgToGlobalSlot22AndForward(void);
extern void *data_ov087_020b9be0;
extern int gOv087RoxasDualDefPackPath;
extern int gOv087RoxasDualDefHPackPath;
extern int gOv087RoxasTgName;
extern int gOv087RoHRName;
extern int gOv087RoHLName;
extern int gOv087RoxasRName;
extern int gOv087Bip01Name;

/* The rig hangs off the scene node at +0x28; each bone block starts 0x40 further in. */
typedef struct { int pad[1]; int f4; } RigHdr;

static inline int bone(char *obj) {
    int p = ((RigHdr *)(*(int *)(obj + 0x20) + 0x24))->f4;
    return p != 0 ? p + 0x40 : 0;
}

void Ov087_Boot(int *cfg) {
    struct { int a, b, c, d, e; } params;
    char *obj = (char *)NNSi_FndGetCurrentRootHeap();
    int i;
    int b;

    data_ov087_020b9be0 = obj;
    obj[9] = (char)cfg[0];
    obj[0x4bc] = (char)cfg[1];
    obj[8] = *(unsigned char *)((char *)cfg + 8);
    *(int *)(obj + 0xc) = 19;
    *(kh_unaligned_s64 *)obj = 0;

    params.a = 1;
    params.c = 9 << 8;
    params.b = 0xf00;
    Entity_ForwardToSlot(*(signed char *)(obj + 0x4bc),
                         (unsigned short)(1 << *(unsigned char *)(obj + 8)), 0, &params, 0);

    if (cfg[6] == 0) {
        TailForwardTrackEntry(*(signed char *)(obj + 0x4bc), &gOv087RoxasDualDefPackPath, 1, cfg[0] + 7);
    } else {
        TailForwardTrackEntry(*(signed char *)(obj + 0x4bc), &gOv087RoxasDualDefHPackPath, 1, cfg[0] + 7);
    }
    *(void **)(obj + 0x664 + 0x00) = (void *)&Ov087_ApplyModeChange;
    *(void **)(obj + 0x664 + 0x04) = (void *)&Ov087_UpdateAttachmentAndChannels;
    *(void **)(obj + 0x664 + 0x08) = (void *)&Ov087_InvokeSubHandlerPairWithGlobalBuffer;
    *(void **)(obj + 0x664 + 0x0c) = 0;
    *(void **)(obj + 0x664 + 0x10) = 0;
    *(void **)(obj + 0x664 + 0x14) = (void *)&Ov087_ReloadTwoSceneResources;
    *(void **)(obj + 0x664 + 0x18) = (void *)&Ov087_MapSlotKindToAnim;
    *(void **)(obj + 0x664 + 0x20) = (void *)&Ov087_HandleMsgAndFaceHeading;
    *(void **)(obj + 0x664 + 0x28) = (void *)&Ov087_BuildRenderHandles;
    *(void **)(obj + 0x664 + 0x24) = (void *)&Ov087_StoreArgToGlobalSlot22AndForward;

    Actor_InitEntityLink(obj + 0x20, ArrayEntryPtrD0(*(signed char *)(obj + 0x4bc)));

    i = 0;
    goto test;
body:
    *(int *)(obj + i * sizeof(int) + 0x514) = -1;
    i++;
test:
    if (i < 5) {
        goto body;
    }

    b = bone(obj);
    *(int *)(obj + 0x520) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv087RoxasTgName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x518) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv087RoHRName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x514) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv087RoHLName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x51c) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv087RoxasRName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x524) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv087Bip01Name) : -1;
    if (cfg[3] != 0) {
        *(kh_unaligned_s64 *)obj |= 0x20;
    }
    if (cfg[4] != 0) {
        *(kh_unaligned_s64 *)obj |= 0x10000;
    }
    if (cfg[5] != 0) {
        *(kh_unaligned_s64 *)obj |= 0x1000000000LL;
    }
    Ov022_InitActor(obj);
}
