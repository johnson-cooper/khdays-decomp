/* PS2: mechanically prepared copy of src/overlays/players/ov050_player_axel_2/Ov050_ActorCtor.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Constructor for the ov050 actor. Same routine as the panel-class constructor and
 * built on the same helpers, but with its own tuning: state 1 rather than 0x11, an
 * open limit of 0x1700, a TWO-WAY name choice on the config's alternateName field --
 * which the whole ov047 family leaves unused -- five bone lookups rather than four,
 * and an eight-entry handler table. */

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
    int alternateName;
    int nameGroup;
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

extern void Ov050_ActorApplyModeChange(void);
extern void Ov050_initSubObjectsSetU64Flags(void);
extern void Ov050_dispatchSubHandlerPair(void);
extern void Ov050_ClaimBothSlots(void);
extern void Ov050_MapSlotKindToAnim(void);
extern void Ov050_enterState21ComputeAimAngle(void);
extern void Ov050_ActorBuildStep(void);
extern void Ov050_beginObjectSlotIfReady(void);

extern void *data_ov050_020b75c0;
extern const char gOv050AxelDefPackPath[];
extern const char gOv050AxelDefHPackPath[];
extern int gOv050AxelTgName;
extern int gOv050AxHRName;
extern int gOv050AxHLName;
extern int gOv050AxelRName;
extern int gOv050Bip01Name;

void Ov050_ActorCtor(struct PanelInitConfig *config)
{
    struct Ov044OpenParams params;
    char *object = (char *)NNSi_FndGetCurrentRootHeap();
    int i;
    int bone;

    data_ov050_020b75c0 = object;
    object[9] = (char)config->objectType;
    object[0x4bc] = (char)config->slotId;
    object[8] = config->bitIndex;
    *(int *)(object + 0xc) = 1;
    *(kh_unaligned_s64 *)object = 0;

    params.enabled = 1;
    params.scale = 9 << 8;
    params.limit = 0x17 << 8;
    Entity_ForwardToSlot(*(signed char *)(object + 0x4bc), (u16)(1 << *(u8 *)(object + 8)), 0,
                         &params, 0);

    if (config->alternateName == 0) {
        TailForwardTrackEntry(*(signed char *)(object + 0x4bc), (void *)gOv050AxelDefPackPath, 1,
                      config->objectType + 7);
    } else {
        TailForwardTrackEntry(*(signed char *)(object + 0x4bc), (void *)gOv050AxelDefHPackPath, 1,
                      config->objectType + 7);
    }

    *(void **)(object + 0x664 + 0x00) = (void *)&Ov050_ActorApplyModeChange;
    *(void **)(object + 0x664 + 0x04) = (void *)&Ov050_initSubObjectsSetU64Flags;
    *(void **)(object + 0x664 + 0x08) = (void *)&Ov050_dispatchSubHandlerPair;
    *(void **)(object + 0x664 + 0x0c) = 0;
    *(void **)(object + 0x664 + 0x10) = 0;
    *(void **)(object + 0x664 + 0x14) = (void *)&Ov050_ClaimBothSlots;
    *(void **)(object + 0x664 + 0x18) = (void *)&Ov050_MapSlotKindToAnim;
    *(void **)(object + 0x664 + 0x20) = (void *)&Ov050_enterState21ComputeAimAngle;
    *(void **)(object + 0x664 + 0x28) = 0;
    *(void **)(object + 0x664 + 0x28) = (void *)&Ov050_ActorBuildStep;
    *(void **)(object + 0x664 + 0x24) = (void *)&Ov050_beginObjectSlotIfReady;

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
        ? NNS_G3dGetResDictIdxByName((void *)bone, &gOv050AxelTgName) : -1;
    bone = Ov044_GetBoneBase(object);
    *(int *)(object + 0x518) = bone != 0
        ? NNS_G3dGetResDictIdxByName((void *)bone, &gOv050AxHRName) : -1;
    bone = Ov044_GetBoneBase(object);
    *(int *)(object + 0x514) = bone != 0
        ? NNS_G3dGetResDictIdxByName((void *)bone, &gOv050AxHLName) : -1;
    bone = Ov044_GetBoneBase(object);
    *(int *)(object + 0x51c) = bone != 0
        ? NNS_G3dGetResDictIdxByName((void *)bone, &gOv050AxelRName) : -1;
    bone = Ov044_GetBoneBase(object);
    *(int *)(object + 0x524) = bone != 0
        ? NNS_G3dGetResDictIdxByName((void *)bone, &gOv050Bip01Name) : -1;

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
