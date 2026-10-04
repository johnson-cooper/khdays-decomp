/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/func_ov022_020a6f9c.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Runs the actor's pending action or tail state unless its guard object is busy. */

extern int Ov022_IsBit0Set_5(unsigned char *p);
extern int func_ov022_02094074(unsigned char *p);
extern int Ov022_TryPendingAction(unsigned int *p, int a);
extern int Ov022_UpdateTailState(unsigned int *p);

int func_ov022_020a6f9c(unsigned int *param_1) {
    int iVar2 = 0;
    if (Ov022_IsBit0Set_5((unsigned char *)(param_1 + 0x723)) != 0 &&
        func_ov022_02094074((unsigned char *)(param_1 + 0x723)) == 0) {
        return iVar2;
    }
    if ((*(kh_unaligned_u64 *)param_1 & 4) == 0 &&
        (iVar2 = Ov022_TryPendingAction(param_1, 0)) == 0) {
        iVar2 = Ov022_UpdateTailState(param_1);
    }
    return iVar2;
}
