/* PS2: mechanically prepared copy of src/overlays/players/ov089_player_saix_4/Ov089_Boot.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Ov089_Boot -- overlay boot of the ov033 enemy (x4: ov033/051/071/089): the ov042 shape (see
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

extern void Ov089_dispatchStateTransitionCode(void);
extern void Ov089_UpdateShotsByMode(void);
extern void Ov089_beginStateBindHandlers(void);
extern void Ov089_initSharedObjSetReadyFlag(void);
extern void Ov089_MapSlotKindToAnim(void);
extern void Ov089_HandleMsgAndReaimArc(void);
extern void Ov089_BuildRenderHandle(void);
extern void Ov089_activateAndComputeAimAngle(void);
extern void *data_ov089_020bc120;
extern int gOv089SaixDefPackPath;
extern int gOv089SaixDefHPackPath;
extern int gOv089SaixTgName;
extern int gOv089SaHRName;
extern int gOv089SaHLName;
extern int gOv089SaixRName;
extern int gOv089Bip01Name;

/* The rig hangs off the scene node at +0x28; each bone block starts 0x40 further in. */
typedef struct { int pad[1]; int f4; } RigHdr;

static inline int bone(char *obj) {
    int p = ((RigHdr *)(*(int *)(obj + 0x20) + 0x24))->f4;
    return p != 0 ? p + 0x40 : 0;
}

void Ov089_Boot(int *cfg) {
    struct { int a, b, c, d, e; } params;
    char *obj = (char *)NNSi_FndGetCurrentRootHeap();
    int i;
    int b;

    data_ov089_020bc120 = obj;
    obj[9] = (char)cfg[0];
    obj[0x4bc] = (char)cfg[1];
    obj[8] = *(unsigned char *)((char *)cfg + 8);
    *(int *)(obj + 0xc) = 3;
    *(kh_unaligned_s64 *)obj = 0;

    params.a = 1;
    params.c = 9 << 8;
    params.b = 0x1700;
    Entity_ForwardToSlot(*(signed char *)(obj + 0x4bc),
                         (unsigned short)(1 << *(unsigned char *)(obj + 8)), 0, &params, 0);

    if (cfg[6] == 0) {
        TailForwardTrackEntry(*(signed char *)(obj + 0x4bc), &gOv089SaixDefPackPath, 1, cfg[0] + 7);
    } else {
        TailForwardTrackEntry(*(signed char *)(obj + 0x4bc), &gOv089SaixDefHPackPath, 1, cfg[0] + 7);
    }
    *(void **)(obj + 0x664 + 0x00) = (void *)&Ov089_dispatchStateTransitionCode;
    *(void **)(obj + 0x664 + 0x04) = (void *)&Ov089_UpdateShotsByMode;
    *(void **)(obj + 0x664 + 0x08) = (void *)&Ov089_beginStateBindHandlers;
    *(void **)(obj + 0x664 + 0x0c) = 0;
    *(void **)(obj + 0x664 + 0x10) = 0;
    *(void **)(obj + 0x664 + 0x14) = (void *)&Ov089_initSharedObjSetReadyFlag;
    *(void **)(obj + 0x664 + 0x18) = (void *)&Ov089_MapSlotKindToAnim;
    *(void **)(obj + 0x664 + 0x20) = (void *)&Ov089_HandleMsgAndReaimArc;
    *(void **)(obj + 0x664 + 0x28) = (void *)&Ov089_BuildRenderHandle;
    *(void **)(obj + 0x664 + 0x24) = (void *)&Ov089_activateAndComputeAimAngle;

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
    *(int *)(obj + 0x520) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv089SaixTgName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x518) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv089SaHRName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x514) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv089SaHLName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x51c) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv089SaixRName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x524) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv089Bip01Name) : -1;
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
