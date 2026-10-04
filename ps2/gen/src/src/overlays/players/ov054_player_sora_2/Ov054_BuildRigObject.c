/* PS2: mechanically prepared copy of src/overlays/players/ov054_player_sora_2/Ov054_BuildRigObject.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Builds the panel object in the root heap: slot/bit setup, callback table at +0x664, bone indices.
 */

#include "nitro/types.h"
#include "game/engine.h"

struct InitConfig {
    int type;
    int slot;
    u8 bit;
    u8 pad[3];
    int low;
    int mid;
    int high;
};

struct OpenParams { int enabled, limit, scale, unused0c, unused10; };
struct RigHeader { int unused, rig; };

extern void *NNSi_FndGetCurrentRootHeap(void);
extern int NNS_G3dGetResDictIdxByName(void *, void *);
extern void Ov022_InitActor(void *);

extern void Ov054_ApplyModeChange(void);
extern void Ov054_Weapon_TickStreams(void);
extern void Ov054_StepSlotAndForward(void);
extern void Ov054_initFlagStateRegionMarshal(void);
extern void Ov054_MapSlotKindToAnim(void);
extern void Ov054_EnterActorState(void);
struct Runtime;
typedef u8 (*SetupHandler)(struct Runtime *);
typedef int (*StateHandler)(int *);
extern u8 Ov054_RegisterHandlersAndArm(struct Runtime *);
extern int Ov054_FlagLocalAndEnterState21(int *);
extern void *data_ov054_020b74a0;
extern char gOv054SoraDefPackPath[];
extern char gOv054SoraTgName[];
extern char gOv054SoWTg00Name[];
extern char gOv054SoLeftDummyName[];
extern char gOv054SoraRName[];
extern char gOv054Bip01Name[];

static inline int bone(char *obj) {
    int rig = ((struct RigHeader *)(*(int *)(obj + 0x20) + 0x24))->rig;
    return rig != 0 ? rig + 0x40 : 0;
}

void Ov054_BuildRigObject(struct InitConfig *cfg) {
    struct OpenParams p;
    char *obj = (char *)NNSi_FndGetCurrentRootHeap();
    int i, b, mode;

    data_ov054_020b74a0 = obj;
    obj[9] = (char)cfg->type;
    obj[0x4bc] = (char)cfg->slot;
    obj[8] = cfg->bit;
    *(int *)(obj + 0xc) = 5;
    *(kh_unaligned_s64 *)obj = 0;

    p.enabled = 1;
    p.scale = 0x900;
    p.limit = 0xf00;
    Entity_ForwardToSlot(*(signed char *)(obj + 0x4bc),
                  (unsigned short)(1 << *(u8 *)(obj + 8)), 0, &p, 0);
    TailForwardTrackEntry(*(signed char *)(obj + 0x4bc), gOv054SoraDefPackPath,
                  1, cfg->type + 7);

    *(void **)(obj + 0x664) = (void *)&Ov054_ApplyModeChange;
    *(void **)(obj + 0x668) = (void *)&Ov054_Weapon_TickStreams;
    *(void **)(obj + 0x66c) = (void *)&Ov054_StepSlotAndForward;
    *(void **)(obj + 0x670) = 0;
    *(void **)(obj + 0x674) = 0;
    *(void **)(obj + 0x678) = (void *)&Ov054_initFlagStateRegionMarshal;
    *(void **)(obj + 0x67c) = (void *)&Ov054_MapSlotKindToAnim;
    *(void **)(obj + 0x684) = (void *)&Ov054_EnterActorState;

    mode = LoadGlobalU16At0();
    if (mode != 0x2a) {
        *(SetupHandler *)(obj + 0x68c) = Ov054_RegisterHandlersAndArm;
    } else {
        *(SetupHandler *)(obj + 0x68c) = 0;
    }
    *(StateHandler *)(obj + 0x688) = Ov054_FlagLocalAndEnterState21;

    Actor_InitEntityLink(obj + 0x20, ArrayEntryPtrD0(*(signed char *)(obj + 0x4bc)));
    i = 0;
    goto test;
body:
    *(int *)(obj + i * sizeof(int) + 0x514) = -1;
    i++;
test:
    if (i < 5) goto body;

    b = bone(obj);
    *(int *)(obj + 0x520) = b ? NNS_G3dGetResDictIdxByName((void *)b, gOv054SoraTgName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x518) = b ? NNS_G3dGetResDictIdxByName((void *)b, gOv054SoWTg00Name) : -1;
    b = bone(obj);
    *(int *)(obj + 0x514) = b ? NNS_G3dGetResDictIdxByName((void *)b, gOv054SoLeftDummyName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x51c) = b ? NNS_G3dGetResDictIdxByName((void *)b, gOv054SoraRName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x524) = b ? NNS_G3dGetResDictIdxByName((void *)b, gOv054Bip01Name) : -1;

    if (cfg->low) *(kh_unaligned_s64 *)obj |= 0x20;
    if (cfg->mid) *(kh_unaligned_s64 *)obj |= 0x10000;
    if (cfg->high) *(kh_unaligned_s64 *)obj |= 0x1000000000LL;
    Ov022_InitActor(obj);
}
