/* PS2: mechanically prepared copy of src/overlays/players/ov032_player_xigbar/Ov032_Boot.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Overlay boot of the ov032 enemy (and its byte-identical twins): takes the root heap block 3c00 hands
 * out as the object, latches it in the overlay global, copies the identity fields from the
 * caller's config, clears bit 1 of the +0x2c30 flags, opens the slot with the {1, 0x1700, 0x900}
 * parameter block, binds the animation table (the alternate one when the config's +0x18 flag is
 * set), fills the handler vtable at +0x664, attaches the scene node, resolves five bone handles
 * by name, folds the three optional capability bits into the 64-bit flag word and hands the
 * object over. */

#include "game/engine.h"

extern void *NNSi_FndGetCurrentRootHeap(void);
extern int  NNS_G3dGetResDictIdxByName(void *node, void *desc);
extern void Ov022_InitActor(void *obj);

extern void Ov032_ApplyMode(void);
extern void Ov032_UpdateTick(void);
extern void Ov032_EffectTick(void);
extern void Ov032_ForwardSetFlagBit3(void);
extern void Ov032_SpawnAttachEffects(void);
extern void Ov032_SceneSetup(void);
extern void Ov032_MapSlotKindToAnim(void);
extern void Ov032_HandleStateMessage(void);
extern void Ov032_SpawnShot(void);
extern void Ov032_BuildRenderHandles(void);
extern void Ov032_ArmPlayerLock(void);
extern void *data_ov032_020b58c0;
extern int gOv032XigbarDefPackPath;
extern int gOv032XigbarDefHPackPath;
extern int gOv032XigbarTgName;
extern int gOv032XigHRName;
extern int gOv032XigHLName;
extern int gOv032XigbarRName;
extern int gOv032Bip01Name;

/* The rig hangs off the scene node at +0x28; each bone block starts 0x40 further in. */
typedef struct { int pad[1]; int f4; } RigHdr;

static inline int bone(char *obj) {
    int p = ((RigHdr *)(*(int *)(obj + 0x20) + 0x24))->f4;
    return p != 0 ? p + 0x40 : 0;
}

void Ov032_Boot(int *cfg) {
    struct { int a, b, c, d, e; } params;
    char *obj = (char *)NNSi_FndGetCurrentRootHeap();
    int b;

    data_ov032_020b58c0 = obj;
    obj[9] = (char)cfg[0];
    obj[0x4bc] = (char)cfg[1];
    obj[8] = *(unsigned char *)((char *)cfg + 8);
    *(int *)(obj + 0xc) = 2;
    *(kh_unaligned_s64 *)obj = 0;
    *(unsigned char *)(obj + 0x2c30) &= ~2;

    params.a = 1;
    params.c = 9 << 8;
    params.b = 0x1700;
    Entity_ForwardToSlot(*(signed char *)(obj + 0x4bc),
                         (unsigned short)(1 << *(unsigned char *)(obj + 8)), 0, &params, 0);

    if (cfg[6] == 0) {
        TailForwardTrackEntry(*(signed char *)(obj + 0x4bc), &gOv032XigbarDefPackPath, 1, cfg[0] + 7);
    } else {
        TailForwardTrackEntry(*(signed char *)(obj + 0x4bc), &gOv032XigbarDefHPackPath, 1, cfg[0] + 7);
    }
    *(void **)(obj + 0x664 + 0x00) = (void *)&Ov032_ApplyMode;
    *(void **)(obj + 0x664 + 0x04) = (void *)&Ov032_UpdateTick;
    *(void **)(obj + 0x664 + 0x08) = (void *)&Ov032_EffectTick;
    *(void **)(obj + 0x664 + 0x0c) = (void *)&Ov032_ForwardSetFlagBit3;
    *(void **)(obj + 0x664 + 0x10) = (void *)&Ov032_SpawnAttachEffects;
    *(void **)(obj + 0x664 + 0x14) = (void *)&Ov032_SceneSetup;
    *(void **)(obj + 0x664 + 0x18) = (void *)&Ov032_MapSlotKindToAnim;
    *(void **)(obj + 0x664 + 0x20) = (void *)&Ov032_HandleStateMessage;
    *(void **)(obj + 0x664 + 0x1c) = (void *)&Ov032_SpawnShot;
    *(void **)(obj + 0x664 + 0x28) = (void *)&Ov032_BuildRenderHandles;
    *(void **)(obj + 0x664 + 0x24) = (void *)&Ov032_ArmPlayerLock;

    Actor_InitEntityLink(obj + 0x20, ArrayEntryPtrD0(*(signed char *)(obj + 0x4bc)));

    b = bone(obj);
    *(int *)(obj + 0x520) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv032XigbarTgName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x518) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv032XigHRName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x514) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv032XigHLName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x51c) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv032XigbarRName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x524) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv032Bip01Name) : -1;
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
