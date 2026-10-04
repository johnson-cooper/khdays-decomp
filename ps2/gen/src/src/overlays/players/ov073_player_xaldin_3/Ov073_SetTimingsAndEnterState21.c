/* PS2: mechanically prepared copy of src/overlays/players/ov073_player_xaldin_3/Ov073_SetTimingsAndEnterState21.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Begins the special attack: for the local player sets bit 16 of the two 64-bit flag words, picks
 * the effect speed and range for the game mode and switches to state 0x21. */

#include "game/engine.h"

extern int Ov022_ActorSetState(int *self, int state);
extern int data_ov073_020ba540;

int Ov073_SetTimingsAndEnterState21(int *self) {
    int *blk = (int *)(*(int *)&data_ov073_020ba540 + 0xe4 + 0x2c00);
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x464) |= 0x10000;
    }
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x46c) |= 0x10000;
    }
    blk[5] = (GetFrameRateMode() == 1) ? 0x1333 : 0xccd;
    blk[6] = (GetFrameRateMode() == 1) ? 0x600 : 0x400;
    return Ov022_ActorSetState(self, 0x21);
}
