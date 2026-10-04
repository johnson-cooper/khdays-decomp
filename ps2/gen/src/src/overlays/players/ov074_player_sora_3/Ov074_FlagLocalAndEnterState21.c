/* PS2: mechanically prepared copy of src/overlays/players/ov074_player_sora_3/Ov074_FlagLocalAndEnterState21.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* When this is the local player, sets bit 16 of the two 64-bit flag words at +0x464 and +0x46c;
 * then switches the actor to its fixed follow-up state (Ov022_ActorSetState). */

#include "game/engine.h"

extern int Ov022_ActorSetState();

int Ov074_FlagLocalAndEnterState21(int *r0)
{
    int *r4 = r0;

    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)r4 + 0x464) |= 0x10000;
    }
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)r4 + 0x46c) |= 0x10000;
    }
    return Ov022_ActorSetState(r4, 0x21);
}
