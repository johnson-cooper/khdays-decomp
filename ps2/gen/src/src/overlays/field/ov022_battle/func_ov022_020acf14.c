/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/func_ov022_020acf14.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Sets the actor's animation frame and flags the change (the host also marks it for sync). */

#include "game/engine.h"

extern void Anim_SetFrameWrapped(unsigned short *a, unsigned int b, unsigned int c);

void func_ov022_020acf14(unsigned int *param_1, unsigned int param_2) {
    Anim_SetFrameWrapped((unsigned short *)(param_1[8] + 4), 0, param_2);
    param_1[0x1ec] = param_2;
    *(kh_unaligned_u64 *)param_1 |= 0x20000000;
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_u64 *)(param_1 + 0x119) |= 0x80000000;
    }
    if (Session_GetLocalPlayerIndex() != 0) {
        return;
    }
    *(kh_unaligned_u64 *)(param_1 + 0x119) |= 0x2000000000LL;
}
