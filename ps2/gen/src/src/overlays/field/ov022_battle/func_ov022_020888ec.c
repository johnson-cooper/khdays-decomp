/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/func_ov022_020888ec.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Host only: sets or clears a player's flags 0x80 and 0x1000000. */

#include "game/engine.h"

void func_ov022_020888ec(int param_1, int param_2) {
    unsigned int *puVar2;
    if (Session_GetLocalPlayerIndex() != 0) {
        return;
    }
    puVar2 = (unsigned int *)GetEntryField20ByIndex(param_1);
    if (puVar2 == 0) {
        return;
    }
    if (param_2 != 0) {
        *(kh_unaligned_u64 *)puVar2 |= 0x80;
        *(kh_unaligned_u64 *)puVar2 |= 0x1000000;
        return;
    }
    *(kh_unaligned_u64 *)puVar2 &= ~0x80LL;
    *(kh_unaligned_u64 *)puVar2 &= ~0x1000000LL;
}
