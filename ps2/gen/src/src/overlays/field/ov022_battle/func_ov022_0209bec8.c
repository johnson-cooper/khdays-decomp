/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/func_ov022_0209bec8.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Picks a random guard entry (one fewer when flagged) and arms the actor's guard window with it. */

#include "game/engine.h"

extern void Ov022_ArmGuardWindow(int a, unsigned int *b, int c);
extern void Ov022_SpendGuardOnHit(int a, unsigned int *b, int c);

struct tbl9_0209bec8 {
    int v[9];
};
extern struct tbl9_0209bec8 data_ov022_020b26d0;

void func_ov022_0209bec8(unsigned int *param_1) {
    unsigned int auStack_34[10];
    struct tbl9_0209bec8 local_58;
    unsigned int uVar2;
    int iVar1;
    uVar2 = 9;
    local_58 = data_ov022_020b26d0;
    if ((*(kh_unaligned_u64 *)param_1 & 0x1000000000LL) != 0) {
        uVar2 = 8;
    }
    iVar1 = Session_RandNextScaled(uVar2);
    auStack_34[5] = local_58.v[iVar1];
    Ov022_ArmGuardWindow((int)param_1, auStack_34, 1);
    Ov022_SpendGuardOnHit((int)param_1, auStack_34, 1);
}
