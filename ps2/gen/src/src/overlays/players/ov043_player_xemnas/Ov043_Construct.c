/* PS2: mechanically prepared copy of src/overlays/players/ov043_player_xemnas/Ov043_Construct.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Ov043_Construct -- overlay boot of the mission enemy (ov043/ov062 twins): the ov042 shape
 * (see Ov042_BuildRigObject for the load-bearing forms): grabs the root heap block as the object,
 * latches it in the overlay global, copies the identity fields from the caller's config, opens
 * the slot with the {1, 0x1f00, 0x900} parameter block, binds the animation table -- the
 * alternate one when the config's flag at +0x18 is set, fills the handler vtable at +0x664,
 * attaches the scene node, resolves the five bone handles by name, folds the three optional
 * capability bits into the 64-bit flag word and hands the object over. */

#include "game/engine.h"

extern void *NNSi_FndGetCurrentRootHeap(void);
extern int  NNS_G3dGetResDictIdxByName(void *node, void *desc);
extern void Ov022_InitActor(void *obj);

extern void Ov043_HandleModeChange(void);
extern void Ov043_UpdateTick(void);
extern void Ov043_InvokeSubHandlerPairWithGlobalBuffer(void);
extern void Ov043_MissionStart(void);
extern void Ov043_MapSlotKindToAnim(void);
extern void Ov043_ResolveStateHandler(void);
extern void Ov043_SpawnProjectile(void);
extern void Ov043_BuildStep(void);
extern void *data_ov043_020b58e0;
extern int gOv043XemnasDefPackPath;
extern int gOv043XemnasDefHPackPath;
extern void Ov043_EnterState21(void);
extern int gOv043XemnasTgName;
extern int gOv043XeHRName;
extern int gOv043XeHLName;
extern int gOv043XemnasRName;
extern int gOv043Bip01Name;

/* The rig hangs off the scene node at +0x28; each bone block starts 0x40 further in. */
typedef struct { int pad[1]; int f4; } RigHdr;

static inline int bone(char *obj) {
    int p = ((RigHdr *)(*(int *)(obj + 0x20) + 0x24))->f4;
    return p != 0 ? p + 0x40 : 0;
}

void Ov043_Construct(int *cfg) {
    struct { int a, b, c, d, e; } params;
    char *obj = (char *)NNSi_FndGetCurrentRootHeap();
    int b;

    data_ov043_020b58e0 = obj;
    obj[9] = (char)cfg[0];
    obj[0x4bc] = (char)cfg[1];
    obj[8] = *(unsigned char *)((char *)cfg + 8);
    *(int *)(obj + 0xc) = 0xd;
    *(kh_unaligned_s64 *)obj = 0;

    params.a = 1;
    params.c = 9 << 8;
    params.b = 0x1f00;
    Entity_ForwardToSlot(*(signed char *)(obj + 0x4bc),
                         (unsigned short)(1 << *(unsigned char *)(obj + 8)), 0, &params, 0);

    if (cfg[6] == 0) {
        TailForwardTrackEntry(*(signed char *)(obj + 0x4bc), &gOv043XemnasDefPackPath, 1, cfg[0] + 7);
    } else {
        TailForwardTrackEntry(*(signed char *)(obj + 0x4bc), &gOv043XemnasDefHPackPath, 1, cfg[0] + 7);
    }
    *(void **)(obj + 0x664 + 0x00) = (void *)&Ov043_HandleModeChange;
    *(void **)(obj + 0x664 + 0x04) = (void *)&Ov043_UpdateTick;
    *(void **)(obj + 0x664 + 0x08) = (void *)&Ov043_InvokeSubHandlerPairWithGlobalBuffer;
    *(void **)(obj + 0x664 + 0x0c) = 0;
    *(void **)(obj + 0x664 + 0x10) = 0;
    *(void **)(obj + 0x664 + 0x14) = (void *)&Ov043_MissionStart;
    *(void **)(obj + 0x664 + 0x18) = (void *)&Ov043_MapSlotKindToAnim;
    *(void **)(obj + 0x664 + 0x20) = (void *)&Ov043_ResolveStateHandler;
    *(void **)(obj + 0x664 + 0x1c) = (void *)&Ov043_SpawnProjectile;
    *(void **)(obj + 0x664 + 0x28) = (void *)&Ov043_BuildStep;
    *(void **)(obj + 0x664 + 0x24) = (void *)&Ov043_EnterState21;

    Actor_InitEntityLink(obj + 0x20, ArrayEntryPtrD0(*(signed char *)(obj + 0x4bc)));

    b = bone(obj);
    *(int *)(obj + 0x520) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv043XemnasTgName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x518) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv043XeHRName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x514) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv043XeHLName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x51c) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv043XemnasRName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x524) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv043Bip01Name) : -1;
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
