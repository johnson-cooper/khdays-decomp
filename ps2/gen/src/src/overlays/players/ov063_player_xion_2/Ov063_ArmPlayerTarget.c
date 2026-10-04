/* PS2: mechanically prepared copy of src/overlays/players/ov063_player_xion_2/Ov063_ArmPlayerTarget.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Sets the local player's lock bits, stores the target and arms the tracking state. */

#include "game/engine.h"

extern int Ov022_ActorSetState();

int Ov063_ArmPlayerTarget(int *r0, int r1)
{
    int *r6 = r0;
    int r5 = r1;
    int *r4 = (int *)r6[0xdb4 / 4];

    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)r4 + 0x464) |= 0x10000;
    }
    *(kh_unaligned_s64 *)((char *)r4 + 0x46c) |= 0x10000;
    r6[1] = 0;
    r6[0] = r5;
    if (r5 != 0) {
        return Ov022_ActorSetState(r4, 0x23);
    }
    return Ov022_ActorSetState(r4, 0x22);
}
