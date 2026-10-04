/* PS2: mechanically prepared copy of src/overlays/players/ov066_player_donald_2/Ov066_PanelCtor.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Constructor for the ov066 panel object: takes the root heap block as the object,
 * publishes it in the overlay's global slot, stamps the identity fields from the
 * caller's config, opens the object with its display parameters under a fixed name,
 * installs the handler table at +0x664, binds the rig, invalidates the five cached
 * slots at +0x514, resolves four bone indices off the rig, and finally raises the
 * three optional feature flags the config asked for. */

#include "nitro/types.h"
#include "game/engine.h"

struct PanelInitConfig {
    int objectType;
    int slotId;
    u8 bitIndex;
    u8 pad09[3];
    int enableLowFlag;
    int enableMidFlag;
    int enableHighFlag;
};

struct Ov044OpenParams {
    int enabled;
    int limit;
    int scale;
    int unused0c;
    int unused10;
};

struct Ov044RigHeader {
    int pad0;
    int rig;
};

static inline int Ov044_GetBoneBase(char *object)
{
    int rig = ((struct Ov044RigHeader *)(
        *(int *)(object + 0x20) + 0x24))->rig;
    return rig != 0 ? rig + 0x40 : 0;
}

extern void *NNSi_FndGetCurrentRootHeap(void);
extern int NNS_G3dGetResDictIdxByName(void *, void *);
extern void Ov022_InitActor(void *);

extern void Ov066_RequestState(void);
extern void Ov066_PanelTick(void);
extern void Ov066_InvokeSubHandlerPairWithGlobalBuffer(void);
extern void Ov066_initSharedObjSetReadyFlag(void);
extern void Ov066_MapSlotKindToAnim(void);
extern void Ov066_FaceTargetOnRequest21(void);
extern void Ov066_SetupBuildBlock(void);
extern void Ov066_ArmPlayerBlock(void);

extern void *data_ov066_020b6b80;
extern const char gOv066DonaldDefPackPath[];
extern int gOv066DonaldTgName;
extern int gOv066DoHRName;
extern int gOv066DonaldRName;
extern int gOv066Bip01Name;

void Ov066_PanelCtor(struct PanelInitConfig *config)
{
    struct Ov044OpenParams params;
    char *object = (char *)NNSi_FndGetCurrentRootHeap();
    int i;
    int bone;

    data_ov066_020b6b80 = object;
    object[9] = (char)config->objectType;
    object[0x4bc] = (char)config->slotId;
    object[8] = config->bitIndex;
    *(int *)(object + 0xc) = 0x11;
    *(kh_unaligned_s64 *)object = 0;

    params.enabled = 1;
    params.scale = 9 << 8;
    params.limit = 7 << 8;
    Entity_ForwardToSlot(*(signed char *)(object + 0x4bc), (u16)(1 << *(u8 *)(object + 8)), 0,
                         &params, 0);

    TailForwardTrackEntry(*(signed char *)(object + 0x4bc), (void *)gOv066DonaldDefPackPath, 1,
                  config->objectType + 7);

    *(void **)(object + 0x664 + 0x00) = (void *)&Ov066_RequestState;
    *(void **)(object + 0x664 + 0x04) = (void *)&Ov066_PanelTick;
    *(void **)(object + 0x664 + 0x08) = (void *)&Ov066_InvokeSubHandlerPairWithGlobalBuffer;
    *(void **)(object + 0x664 + 0x0c) = 0;
    *(void **)(object + 0x664 + 0x10) = 0;
    *(void **)(object + 0x664 + 0x14) = (void *)&Ov066_initSharedObjSetReadyFlag;
    *(void **)(object + 0x664 + 0x18) = (void *)&Ov066_MapSlotKindToAnim;
    *(void **)(object + 0x664 + 0x20) = (void *)&Ov066_FaceTargetOnRequest21;
    *(void **)(object + 0x664 + 0x28) = (void *)&Ov066_SetupBuildBlock;
    *(void **)(object + 0x664 + 0x24) = (void *)&Ov066_ArmPlayerBlock;

    Actor_InitEntityLink(object + 0x20,
                  ArrayEntryPtrD0(*(signed char *)(object + 0x4bc)));

    i = 0;
    goto test;
body:
    *(int *)(object + i * sizeof(int) + 0x514) = -1;
    i++;
test:
    if (i < 5) {
        goto body;
    }

    bone = Ov044_GetBoneBase(object);
    *(int *)(object + 0x520) = bone != 0
        ? NNS_G3dGetResDictIdxByName((void *)bone, &gOv066DonaldTgName) : -1;
    bone = Ov044_GetBoneBase(object);
    *(int *)(object + 0x518) = bone != 0
        ? NNS_G3dGetResDictIdxByName((void *)bone, &gOv066DoHRName) : -1;
    bone = Ov044_GetBoneBase(object);
    *(int *)(object + 0x51c) = bone != 0
        ? NNS_G3dGetResDictIdxByName((void *)bone, &gOv066DonaldRName) : -1;
    bone = Ov044_GetBoneBase(object);
    *(int *)(object + 0x524) = bone != 0
        ? NNS_G3dGetResDictIdxByName((void *)bone, &gOv066Bip01Name) : -1;

    if (config->enableLowFlag != 0) {
        *(kh_unaligned_s64 *)object |= 0x20;
    }
    if (config->enableMidFlag != 0) {
        *(kh_unaligned_s64 *)object |= 0x10000;
    }
    if (config->enableHighFlag != 0) {
        *(kh_unaligned_s64 *)object |= 0x1000000000LL;
    }
    Ov022_InitActor(object);
}
