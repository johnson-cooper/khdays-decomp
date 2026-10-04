/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_ResetTimersAndMaybeSignal.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Resets the actor's timers, flags it on the host, counts the statistic for player 0 outside mode 4
 * (unless forced) and sets its state to 1. Returns what Ov022_ActorSetState returns. */

#include "game/engine.h"

extern void Ov002_World_AddStat(int a, int b);
extern int Ov022_ActorSetState(int obj, int a);
extern int data_0204c240;

int Ov022_ResetTimersAndMaybeSignal(int arg0, int arg1) {
    *(int *)(arg0 + 0x698) = 0;
    *(int *)(arg0 + 0x69c) = 0x630;
    *(int *)(arg0 + 0x6a0) = 0;
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_u64 *)(arg0 + 0x464) |= 0x20LL;
    }
    if (arg1 == 0 && (*(unsigned char *)&data_0204c240 & 4) == 0) {
        if ((*(unsigned int *)arg0 & 0x10000) == 0 && *(unsigned char *)(arg0 + 9) == 0)
            Ov002_World_AddStat(1, 3);
    }
    return Ov022_ActorSetState(arg0, 1);
}
