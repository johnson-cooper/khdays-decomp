/* PS2: mechanically prepared copy of src/overlays/players/ov034_player_xaldin/Ov034_Boot.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Ov034_Boot -- overlay boot of the ov034 enemy (x4: ov034/052/072/090): the ov042 shape
 * (see Ov042_BuildRigObject for the load-bearing forms): grabs the root heap block as the object,
 * latches it in the overlay global, copies the identity fields from the caller's config, opens
 * the slot with the {1, 0x1f00, 0x900} parameter block, binds the animation table (the alternate
 * one when the config's flag at +0x18 is set), fills the handler vtable at +0x664, attaches the
 * scene node, invalidates the five bone handles and resolves three of them by name, folds the
 * three optional capability bits into the 64-bit flag word and hands the object over. */

#include "game/engine.h"

extern void *NNSi_FndGetCurrentRootHeap(void);
extern int  NNS_G3dGetResDictIdxByName(void *node, void *desc);
extern void Ov022_InitActor(void *obj);

extern void Ov034_ApplyModeChange(void);
extern void Ov034_SyncHandleAndDrawAim(void);
extern void Ov034_InvokeSubHandlerPairWithGlobalBuffer(void);
extern void Ov034_BindChannelsAndSlotMaps(void);
extern void Ov034_MapSlotKindToAnim(void);
extern void Ov034_HandleMessage(void);
extern void Ov034_BuildRenderHandles(void);
extern void Ov034_SetTimingsAndEnterState21(void);

extern void *data_ov034_020b5660;
extern int gOv034XaldinDefPackPath;
extern int gOv034XaldinDefHPackPath;
extern int gOv034XaldinTgName;
extern int gOv034XaldinRName;
extern int gOv034Bip01Name;

/* The rig hangs off the scene node at +0x28; each bone block starts 0x40 further in. */
typedef struct { int pad[1]; int f4; } RigHdr;

static inline int bone(char *obj) {
    int p = ((RigHdr *)(*(int *)(obj + 0x20) + 0x24))->f4;
    return p != 0 ? p + 0x40 : 0;
}

void Ov034_Boot(int *cfg) {
    struct { int a, b, c, d, e; } params;
    char *obj = (char *)NNSi_FndGetCurrentRootHeap();
    int i;
    int b;

    data_ov034_020b5660 = obj;
    obj[9] = (char)cfg[0];
    obj[0x4bc] = (char)cfg[1];
    obj[8] = *(unsigned char *)((char *)cfg + 8);
    *(int *)(obj + 0xc) = 4;
    *(kh_unaligned_s64 *)obj = 0;

    params.a = 1;
    params.c = 9 << 8;
    params.b = 0x1f << 8;
    Entity_ForwardToSlot(*(signed char *)(obj + 0x4bc),
                         (unsigned short)(1 << *(unsigned char *)(obj + 8)), 0, &params, 0);

    if (cfg[6] == 0) {
        TailForwardTrackEntry(*(signed char *)(obj + 0x4bc), &gOv034XaldinDefPackPath, 1, cfg[0] + 7);
    } else {
        TailForwardTrackEntry(*(signed char *)(obj + 0x4bc), &gOv034XaldinDefHPackPath, 1, cfg[0] + 7);
    }

    *(void **)(obj + 0x664 + 0x00) = (void *)&Ov034_ApplyModeChange;
    *(void **)(obj + 0x664 + 0x04) = (void *)&Ov034_SyncHandleAndDrawAim;
    *(void **)(obj + 0x664 + 0x08) = (void *)&Ov034_InvokeSubHandlerPairWithGlobalBuffer;
    *(void **)(obj + 0x664 + 0x0c) = 0;
    *(void **)(obj + 0x664 + 0x10) = 0;
    *(void **)(obj + 0x664 + 0x14) = (void *)&Ov034_BindChannelsAndSlotMaps;
    *(void **)(obj + 0x664 + 0x18) = (void *)&Ov034_MapSlotKindToAnim;
    *(void **)(obj + 0x664 + 0x20) = (void *)&Ov034_HandleMessage;
    *(void **)(obj + 0x664 + 0x28) = (void *)&Ov034_BuildRenderHandles;
    *(void **)(obj + 0x664 + 0x24) = (void *)&Ov034_SetTimingsAndEnterState21;

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
    *(int *)(obj + 0x520) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv034XaldinTgName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x51c) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv034XaldinRName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x524) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv034Bip01Name) : -1;

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
