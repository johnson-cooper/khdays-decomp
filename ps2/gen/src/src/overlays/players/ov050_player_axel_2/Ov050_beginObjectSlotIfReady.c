/* PS2: mechanically prepared copy of src/overlays/players/ov050_player_axel_2/Ov050_beginObjectSlotIfReady.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Begins the character's special attack: for the local player sets bit 16 of the two 64-bit flag
 * words, records the attack variant, and switches to state 0x21 unless the variant needs the two
 * effect streams and they are still busy; returns the state result. */

#include "game/engine.h"

extern int data_ov050_020b75c0;
extern int Ov022_AreStreamsIdle(int x);
extern int Ov022_ActorSetState(int this, int tag);

int Ov050_beginObjectSlotIfReady(int this, int param2) {
    int base = data_ov050_020b75c0 + 0x2c2c;
    int result = 0;
    int ok = 1;

    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_u64 *)(this + 0x464) |= 0x10000ULL;
    }
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_u64 *)(this + 0x46c) |= 0x10000ULL;
    }
    *(int *)(base + 4) = 0;
    *(int *)base = param2;
    if (param2 != 0) {
        if (Ov022_AreStreamsIdle(*(int *)(this + 0x2644) + 0x30) == 0 ||
            Ov022_AreStreamsIdle(*(int *)(this + 0x2644) + 0x60) == 0) {
            ok = 0;
        }
    }
    if (ok != 0) {
        result = Ov022_ActorSetState(this, 0x21);
    }
    return result;
}
