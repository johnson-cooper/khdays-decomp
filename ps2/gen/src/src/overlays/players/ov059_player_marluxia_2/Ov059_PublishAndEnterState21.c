/* PS2: mechanically prepared copy of src/overlays/players/ov059_player_marluxia_2/Ov059_PublishAndEnterState21.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Begins the special attack: for the local player sets bit 16 of the two 64-bit flag words, records
 * the variant and switches to state 0x21. */

#include "game/engine.h"

extern int Ov022_ActorSetState(int *self, int state);
extern int data_ov059_020b7320;

int Ov059_PublishAndEnterState21(int *self, int v) {
    int *slot = (int *)(*(int *)&data_ov059_020b7320 + 0xc50 + 0x2000);
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x464) |= 0x10000;
    }
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x46c) |= 0x10000;
    }
    *slot = v;
    return Ov022_ActorSetState(self, 0x21);
}
