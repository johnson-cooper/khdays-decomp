/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/func_ov022_02093900.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Raises the actor's ability levels for the abilities it has (15 rows) by the amount; the host
 * flags the actor when any changed. */

#include "game/engine.h"

void func_ov022_02093900(int param_1, int param_2) {
    int bVar1 = 0;
    int iVar5 = 0;
    do {
        if (Slot_EvalPackedParam(*(unsigned char *)(param_1 + 9), iVar5 + 1) != 0) {
            int iVar4;
            bVar1 = 1;
            iVar4 = Load2DArrayU8(*(unsigned char *)(param_1 + 9), iVar5);
            ClampAndStoreLevelEntry(*(unsigned char *)(param_1 + 9), iVar5, param_2 + iVar4);
        }
        iVar5 = iVar5 + 1;
    } while (iVar5 < 0xf);
    if (Session_GetLocalPlayerIndex() != 0) {
        return;
    }
    if (bVar1 == 0) {
        return;
    }
    *(kh_unaligned_u64 *)(param_1 + 0x46c) |= 0x20000000000LL;
}
