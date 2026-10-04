/* PS2: mechanically prepared copy of src/overlays/players/ov061_player_vexen_2/Ov061_FlagLocalAndMaybeEnterState21.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Begins the special attack: for the local player sets bit 16 of the two 64-bit flag words, records
 * whether the slot is ready and switches to state 0x21 unless that variant's effect stream is still
 * busy; returns the state result. */

#include "game/engine.h"

extern int Ov022_IsSlotReady(int a);
extern int Ov022_AreStreamsIdle(int a);
extern int Ov022_ActorSetState(int self, int state);
extern int data_ov061_020b7000;

int Ov061_FlagLocalAndMaybeEnterState21(int self) {
    int ret = 0;
    int *blk = (int *)(*(int *)&data_ov061_020b7000 + 0x2c + 0x2c00);
    int ok = 1;
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x464) |= 0x10000;
    }
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x46c) |= 0x10000;
    }
    blk[2] = Ov022_IsSlotReady(self + 0x2f8 + 0x2000);
    if (blk[2] != 0) {
        if (Ov022_AreStreamsIdle(*(int *)(self + 0x2000 + 0x644) + 0x30) == 0) {
            ok = 0;
        }
    }
    if (ok != 0) {
        ret = Ov022_ActorSetState(self, 0x21);
    }
    return ret;
}
