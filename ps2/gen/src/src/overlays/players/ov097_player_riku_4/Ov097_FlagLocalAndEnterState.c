/* PS2: mechanically prepared copy of src/overlays/players/ov097_player_riku_4/Ov097_FlagLocalAndEnterState.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Begins the special attack: for the local player sets bit 16 of the two 64-bit flag words and
 * switches to state 0x22 for the alternate variant or 0x21 otherwise. */

#include "game/engine.h"

extern int Ov022_ActorSetState(int *self, int state);

int Ov097_FlagLocalAndEnterState(int *self, int alt) {
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x464) |= 0x10000;
    }
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x46c) |= 0x10000;
    }
    if (alt != 0) {
        return Ov022_ActorSetState(self, 0x22);
    }
    return Ov022_ActorSetState(self, 0x21);
}
