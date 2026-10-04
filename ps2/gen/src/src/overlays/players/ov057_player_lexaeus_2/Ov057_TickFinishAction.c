/* PS2: mechanically prepared copy of src/overlays/players/ov057_player_lexaeus_2/Ov057_TickFinishAction.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Advances the finish-action motion and records the tick hook result in bit 1
 * of the byte at actor offset 0x694. On completion it wakes the actor node,
 * clears both motion accumulators, and returns the actor to mode 0 or mode 2.
 * The finish hook takes the actor and the zero mode value as two arguments.
 */typedef struct { int x, y, z; } Vec3;
typedef struct { unsigned char b0 : 1; unsigned char b1 : 1; } Flags694;
typedef int (*Hook)(int pActor);
typedef void (*FinishHook)(int pActor, int mode);

extern int Session_GetLocalPlayerIndex(void);
extern void Ov022_StepAnchorDelta(int pActor, Vec3 *out);
extern int VEC_Add(void *a, void *b, void *d);
extern void Ov057_FireChargeBurst(int pActor);
extern void SceneNode_Enable(int pNode);
extern int Ov022_ActorSetState(int pActor, int mode);

int Ov057_TickFinishAction(int pActor) {
    Vec3 motionSample;
    Vec3 motionDelta;
    int result = 0;
    int zeroValue;

    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_u64 *)(pActor + 0x464) |= 0x10000LL;
    }
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_u64 *)(pActor + 0x46c) |= 0x10000LL;
    }
    Ov022_StepAnchorDelta(pActor, &motionSample);
    if (motionSample.y != 0) {
        *(int *)(pActor + 0x58) = motionSample.y;
    } else if ((*(unsigned int *)(pActor + 0x24) & 4) == 0) {
        *(kh_unaligned_u64 *)pActor |= 0x400000000000LL;
        *(int *)(pActor + 0x58) = 0;
    }
    motionDelta = motionSample;
    motionDelta.y = 0;
    VEC_Add((Vec3 *)(pActor + 0x498), &motionDelta, (Vec3 *)(pActor + 0x498));
    Ov057_FireChargeBurst(pActor);
    ((Flags694 *)(pActor + 0x694))->b1 = (*(Hook *)(pActor + 0x668))(pActor);
    if (((Flags694 *)(pActor + 0x694))->b1) {
        int pNode;
        *(kh_unaligned_u64 *)pActor |= 0x2000000000000LL;
        pNode = *(int *)(pActor + 0x20);
        if ((*(unsigned int *)pNode & 0x20) == 0) {
            SceneNode_Enable(pNode + 4);
        }
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_u64 *)(pActor + 0x464) |= 2LL;
        }
    }
    if (((Flags694 *)(pActor + 0x694))->b1) {
        zeroValue = 0;
        *(int *)(pActor + 0x698) = *(int *)(pActor + 0x69c) =
            *(int *)(pActor + 0x6a0) = *(int *)(pActor + 0x498) =
            *(int *)(pActor + 0x49c) = *(int *)(pActor + 0x4a0) = zeroValue;
        *(kh_unaligned_u64 *)pActor |= 4LL;
        if ((*(unsigned int *)(pActor + 0x24) & 4) != 0) {
            (*(FinishHook *)(pActor + 0x664))(pActor, zeroValue);
            result = Ov022_ActorSetState(pActor, 0);
        } else {
            result = Ov022_ActorSetState(pActor, 2);
        }
    }
    return result;
}
