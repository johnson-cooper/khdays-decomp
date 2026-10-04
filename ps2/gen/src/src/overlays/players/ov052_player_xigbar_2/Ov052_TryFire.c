/* PS2: mechanically prepared copy of src/overlays/players/ov052_player_xigbar_2/Ov052_TryFire.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Fire attempt of the ov032 enemy (and its byte-identical twins): clears bit 7 of the +0x24
 * flags, raises bit 16 of both 64-bit effect words (+0x464, +0x46c) on the local player's
 * session, marks the actor unrestricted (bit 46) with a cleared +0x58 unless bit 2 of +0x24
 * is set, then asks the +0x668 handler whether the shot fires. A fired shot sets bit 49,
 * restarts the model's animation unless its bit 5 is set, raises bit 1 of the +0x464 word on
 * the local session, clears the six velocity words and reports the state through 020a35f4
 * (after the +0x664 handler when bit 2 of +0x24 is set). */

#include "game/engine.h"

struct ActorBits {
    unsigned char bUnk0 : 1;
    unsigned char bFired : 1;
};

extern int Ov022_ActorSetState(char *self, int mode);

int Ov052_TryFire(char *self)
{
    int nRet = 0;

    *(int *)(self + 0x24) &= ~0x80;
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_u64 *)(self + 0x464) |= 0x10000;
    }
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_u64 *)(self + 0x46c) |= 0x10000;
    }
    if ((*(int *)(self + 0x24) & 4) == 0) {
        *(kh_unaligned_u64 *)self |= 0x400000000000ULL;
        *(int *)(self + 0x58) = 0;
    }
    ((struct ActorBits *)(self + 0x694))->bFired = (*(int (**)(char *))(self + 0x668))(self);
    if (((struct ActorBits *)(self + 0x694))->bFired) {
        *(kh_unaligned_u64 *)self |= 0x2000000000000ULL;
        if ((**(int **)(self + 0x20) & 0x20) == 0) {
            SceneNode_Enable(*(int **)(self + 0x20) + 1);
        }
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_u64 *)(self + 0x464) |= 2ULL;
        }
    }
    if (((struct ActorBits *)(self + 0x694))->bFired) {
        *(int *)(self + 0x4a0) = 0;
        *(int *)(self + 0x49c) = 0;
        *(int *)(self + 0x498) = 0;
        *(int *)(self + 0x6a0) = 0;
        *(int *)(self + 0x69c) = 0;
        *(int *)(self + 0x698) = 0;
        *(kh_unaligned_u64 *)self |= 4ULL;
        if ((*(int *)(self + 0x24) & 4) != 0) {
            nRet = Ov022_ActorSetState(self, 0);
            (*(void (**)(char *, int))(self + 0x664))(self, 0);
        } else {
            nRet = Ov022_ActorSetState(self, 2);
        }
    }
    return nRet;
}
