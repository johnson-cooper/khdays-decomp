/* PS2: mechanically prepared copy of src/overlays/players/ov093_player_larxene_4/Ov093_Boot.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Ov093_Boot -- overlay boot of the ov037 enemy (x4: ov037/055/075/092): the ov042 shape (see
 * Ov042_BuildRigObject for the load-bearing forms): grabs the root heap block as the object, latches
 * it in the overlay global, copies the identity fields from the caller's config, opens the slot
 * with the {1, 0x1700, 0x900} parameter block, binds the animation table -- the alternate one when the config's flag at +0x18 is set, fills the handler vtable
 * at +0x664, attaches the scene node, invalidates the five bone handles and resolves 5 of them by
 * name, folds the three optional capability bits into the 64-bit flag word and hands the object
 * over. */

#include "game/engine.h"

extern void *NNSi_FndGetCurrentRootHeap(void);
extern int  NNS_G3dGetResDictIdxByName(void *node, void *desc);
extern void Ov022_InitActor(void *obj);

extern void Ov093_DispatchRequestAndSetState(void);
extern void Ov093_UpdateTargetAndTimers(void);
extern void Ov093_StepAttackSlotAndForward(void);
extern void Ov093_initDualFlagStateThenCall(void);
extern void Ov093_MapSlotKindToAnim(void);
extern void Ov093_HandleMsgAndLaunch(void);
extern void Ov093_FirePartShot(void);
extern void Ov093_BuildRenderHandles(void);
extern void Ov093_PublishAltAndEnterState(void);
extern void *data_ov093_020bc3c0;
extern int gOv093LarxeneDefPackPath;
extern int gOv093LarxeneDefHPackPath;
extern int gOv093LarxeneTgName;
extern int gOv093LaHRName;
extern int gOv093LaHLName;
extern int gOv093LarxeneRName;
extern int gOv093Bip01Name;

/* The rig hangs off the scene node at +0x28; each bone block starts 0x40 further in. */
typedef struct { int pad[1]; int f4; } RigHdr;

static inline int bone(char *obj) {
    int p = ((RigHdr *)(*(int *)(obj + 0x20) + 0x24))->f4;
    return p != 0 ? p + 0x40 : 0;
}

void Ov093_Boot(int *cfg) {
    struct { int a, b, c, d, e; } params;
    char *obj = (char *)NNSi_FndGetCurrentRootHeap();
    int i;
    int b;

    data_ov093_020bc3c0 = obj;
    obj[9] = (char)cfg[0];
    obj[0x4bc] = (char)cfg[1];
    obj[8] = *(unsigned char *)((char *)cfg + 8);
    *(int *)(obj + 0xc) = 7;
    *(kh_unaligned_s64 *)obj = 0;

    params.a = 1;
    params.c = 9 << 8;
    params.b = 0x1700;
    Entity_ForwardToSlot(*(signed char *)(obj + 0x4bc),
                         (unsigned short)(1 << *(unsigned char *)(obj + 8)), 0, &params, 0);

    if (cfg[6] == 0) {
        TailForwardTrackEntry(*(signed char *)(obj + 0x4bc), &gOv093LarxeneDefPackPath, 1, cfg[0] + 7);
    } else {
        TailForwardTrackEntry(*(signed char *)(obj + 0x4bc), &gOv093LarxeneDefHPackPath, 1, cfg[0] + 7);
    }
    *(void **)(obj + 0x664 + 0x00) = (void *)&Ov093_DispatchRequestAndSetState;
    *(void **)(obj + 0x664 + 0x04) = (void *)&Ov093_UpdateTargetAndTimers;
    *(void **)(obj + 0x664 + 0x08) = (void *)&Ov093_StepAttackSlotAndForward;
    *(void **)(obj + 0x664 + 0x0c) = 0;
    *(void **)(obj + 0x664 + 0x10) = 0;
    *(void **)(obj + 0x664 + 0x14) = (void *)&Ov093_initDualFlagStateThenCall;
    *(void **)(obj + 0x664 + 0x18) = (void *)&Ov093_MapSlotKindToAnim;
    *(void **)(obj + 0x664 + 0x20) = (void *)&Ov093_HandleMsgAndLaunch;
    *(void **)(obj + 0x664 + 0x1c) = (void *)&Ov093_FirePartShot;
    *(void **)(obj + 0x664 + 0x28) = (void *)&Ov093_BuildRenderHandles;
    *(void **)(obj + 0x664 + 0x24) = (void *)&Ov093_PublishAltAndEnterState;

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
    *(int *)(obj + 0x520) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv093LarxeneTgName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x518) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv093LaHRName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x514) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv093LaHLName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x51c) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv093LarxeneRName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x524) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv093Bip01Name) : -1;
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
