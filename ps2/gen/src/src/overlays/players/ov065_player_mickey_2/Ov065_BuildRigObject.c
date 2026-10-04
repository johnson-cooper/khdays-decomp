/* PS2: mechanically prepared copy of src/overlays/players/ov065_player_mickey_2/Ov065_BuildRigObject.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Constructor for the ov047 panel object: takes the root heap block as the object,
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

extern void Ov065_ApplyModeChange(void);
extern void Ov065_TickPairedState(void);
extern void Ov065_InvokeSubHandlerPairWithGlobalBuffer(void);
extern void Ov065_initFlagStateRegionMarshal(void);
extern void Ov065_MapSlotKindToAnim(void);
extern void Ov065_HandleMsgAndFacePartner(void);
extern void Ov065_AcquireGridSlots(void);
extern void Ov065_FlagLocalAndEnterState21(void);

extern void *data_ov065_020b7340;
extern const char gOv065MickeyDefPackPath[];
extern int gOv065MickeyTgName;
extern int gOv065MiWTgRName;
extern int gOv065MickeyRName;
extern int gOv065Bip01Name;

void Ov065_BuildRigObject(struct PanelInitConfig *config)
{
    struct Ov044OpenParams params;
    char *object = (char *)NNSi_FndGetCurrentRootHeap();
    int i;
    int bone;

    data_ov065_020b7340 = object;
    object[9] = (char)config->objectType;
    object[0x4bc] = (char)config->slotId;
    object[8] = config->bitIndex;
    *(int *)(object + 0xc) = 0x10;
    *(kh_unaligned_s64 *)object = 0;

    params.enabled = 1;
    params.scale = 9 << 8;
    params.limit = 7 << 8;
    Entity_ForwardToSlot(*(signed char *)(object + 0x4bc), (u16)(1 << *(u8 *)(object + 8)), 0,
                         &params, 0);

    TailForwardTrackEntry(*(signed char *)(object + 0x4bc), (void *)gOv065MickeyDefPackPath, 1,
                  config->objectType + 7);

    *(void **)(object + 0x664 + 0x00) = (void *)&Ov065_ApplyModeChange;
    *(void **)(object + 0x664 + 0x04) = (void *)&Ov065_TickPairedState;
    *(void **)(object + 0x664 + 0x08) = (void *)&Ov065_InvokeSubHandlerPairWithGlobalBuffer;
    *(void **)(object + 0x664 + 0x0c) = 0;
    *(void **)(object + 0x664 + 0x10) = 0;
    *(void **)(object + 0x664 + 0x14) = (void *)&Ov065_initFlagStateRegionMarshal;
    *(void **)(object + 0x664 + 0x18) = (void *)&Ov065_MapSlotKindToAnim;
    *(void **)(object + 0x664 + 0x20) = (void *)&Ov065_HandleMsgAndFacePartner;
    *(void **)(object + 0x664 + 0x28) = (void *)&Ov065_AcquireGridSlots;
    *(void **)(object + 0x664 + 0x24) = (void *)&Ov065_FlagLocalAndEnterState21;

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
        ? NNS_G3dGetResDictIdxByName((void *)bone, &gOv065MickeyTgName) : -1;
    bone = Ov044_GetBoneBase(object);
    *(int *)(object + 0x518) = bone != 0
        ? NNS_G3dGetResDictIdxByName((void *)bone, &gOv065MiWTgRName) : -1;
    bone = Ov044_GetBoneBase(object);
    *(int *)(object + 0x51c) = bone != 0
        ? NNS_G3dGetResDictIdxByName((void *)bone, &gOv065MickeyRName) : -1;
    bone = Ov044_GetBoneBase(object);
    *(int *)(object + 0x524) = bone != 0
        ? NNS_G3dGetResDictIdxByName((void *)bone, &gOv065Bip01Name) : -1;

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
