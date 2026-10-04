/* PS2: mechanically prepared copy of src/overlays/players/ov102_player_donald_4/Ov102_ArmPlayerBlock.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Sets the local player's lock bits and arms the build block with the value. */

#include "game/engine.h"

extern int Ov022_ActorSetState(int *self, int state);
extern int data_ov102_020bb920;

int Ov102_ArmPlayerBlock(int *self, int v) {
    int *blk = (int *)(*(int *)&data_ov102_020bb920 + 0xc50 + 0x2000);
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x464) |= 0x10000;
    }
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x46c) |= 0x10000;
    }
    blk[0] = v;
    blk[1] = 1;
    *(int *)((char *)blk + 0x120) = 0;
    blk[2] = 0;
    *(int *)((char *)blk + 0x124) = 0;
    return Ov022_ActorSetState(self, 0x21);
}
