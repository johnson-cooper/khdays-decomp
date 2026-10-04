/* PS2: mechanically prepared copy of src/overlays/players/ov056_player_larxene_2/Ov056_PublishAltAndEnterState.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Begins the special attack: for the local player sets bit 16 of the two 64-bit flag words, records
 * the variant and switches to state 0x22 for the alternate variant or 0x21 otherwise. */

#include "game/engine.h"

extern int Ov022_ActorSetState(int *self, int state);
extern int data_ov056_020b7620;

int Ov056_PublishAltAndEnterState(int *self, int alt) {
    char *blk = (char *)(*(int *)&data_ov056_020b7620 + 0x2c + 0x2c00);
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x464) |= 0x10000;
    }
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x46c) |= 0x10000;
    }
    *(int *)(blk + 0x114) = alt;
    if (alt != 0) {
        return Ov022_ActorSetState(self, 0x22);
    }
    return Ov022_ActorSetState(self, 0x21);
}
