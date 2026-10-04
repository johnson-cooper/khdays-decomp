/* PS2: mechanically prepared copy of src/overlays/players/ov062_player_xemnas_2/Ov062_EnterState21.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Enters state 0x21 of the mission enemy: raises bit 16 of both 64-bit flag words (+0x464,
 * +0x46c) on the local player's session, resets the mission owner's +0x2d38 block speed (+0xc)
 * to 0x50a -- scaled by 1.5 at 20 fps -- and its +0x128 duration to 0x1e6 (0x144
 * otherwise), then runs the shared state entry. */

#include "game/engine.h"

extern int Ov022_ActorSetState(int *self, int state);
extern int data_ov062_020b80e0;

static inline int FX_Mul(int a, int b)
{
    return (int)(((long long)a * b + 0x800) >> 12);
}

int Ov062_EnterState21(int *self) {
    int *blk = (int *)(*(int *)&data_ov062_020b80e0 + 0x138 + 0x2c00);
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x464) |= 0x10000;
    }
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x46c) |= 0x10000;
    }
    blk[3] = 0x50a;
    if (GetFrameRateMode() == 1) {
        blk[3] = FX_Mul(blk[3], 0x1800);
    }
    blk[0x4a] = (GetFrameRateMode() == 1) ? 0x1e6 : 0x144;
    return Ov022_ActorSetState(self, 0x21);
}
