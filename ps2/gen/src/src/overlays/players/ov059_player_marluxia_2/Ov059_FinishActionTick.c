/* PS2: mechanically prepared copy of src/overlays/players/ov059_player_marluxia_2/Ov059_FinishActionTick.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Advances the finish-action motion and records the tick hook result in bit 1
 * of the byte at actor offset 0x694. On completion it wakes the actor node,
 * clears both motion accumulators, and returns the actor to mode 0 or mode 2.
 * The finish hook takes the actor and the zero mode value as two arguments.
 */typedef struct { int x, y, z; } Vec3;
typedef struct { unsigned char b0 : 1; unsigned char b1 : 1; } Flags694;
typedef int (*Hook)(int obj);
typedef void (*FinishHook)(int obj, int mode);

extern int Session_GetLocalPlayerIndex(void);
extern void Ov022_StepAnchorDelta(int obj, Vec3 *out);
extern int VEC_Add(void *a, void *b, void *d);
extern void Ov059_AttackBurstTick(int obj);
extern void SceneNode_Enable(int p);
extern int Ov022_ActorSetState(int obj, int mode);

int Ov059_FinishActionTick(int obj) {
    Vec3 q;
    Vec3 v;
    int r = 0;
    int clear;

    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_u64 *)(obj + 0x464) |= 0x10000LL;
    }
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_u64 *)(obj + 0x46c) |= 0x10000LL;
    }
    Ov022_StepAnchorDelta(obj, &q);
    if (q.y != 0) {
        *(int *)(obj + 0x58) = q.y;
    } else if ((*(unsigned int *)(obj + 0x24) & 4) == 0) {
        *(kh_unaligned_u64 *)obj |= 0x400000000000LL;
        *(int *)(obj + 0x58) = 0;
    }
    v = q;
    v.y = 0;
    VEC_Add((Vec3 *)(obj + 0x498), &v, (Vec3 *)(obj + 0x498));
    Ov059_AttackBurstTick(obj);
    ((Flags694 *)(obj + 0x694))->b1 = (*(Hook *)(obj + 0x668))(obj);
    if (((Flags694 *)(obj + 0x694))->b1) {
        int p;
        *(kh_unaligned_u64 *)obj |= 0x2000000000000LL;
        p = *(int *)(obj + 0x20);
        if ((*(unsigned int *)p & 0x20) == 0) {
            SceneNode_Enable(p + 4);
        }
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_u64 *)(obj + 0x464) |= 2LL;
        }
    }
    if (((Flags694 *)(obj + 0x694))->b1) {
        clear = 0;
        *(int *)(obj + 0x698) = *(int *)(obj + 0x69c) =
            *(int *)(obj + 0x6a0) = *(int *)(obj + 0x498) =
            *(int *)(obj + 0x49c) = *(int *)(obj + 0x4a0) = clear;
        *(kh_unaligned_u64 *)obj |= 4LL;
        if ((*(unsigned int *)(obj + 0x24) & 4) != 0) {
            (*(FinishHook *)(obj + 0x664))(obj, clear);
            r = Ov022_ActorSetState(obj, 0);
        } else {
            r = Ov022_ActorSetState(obj, 2);
        }
    }
    return r;
}
