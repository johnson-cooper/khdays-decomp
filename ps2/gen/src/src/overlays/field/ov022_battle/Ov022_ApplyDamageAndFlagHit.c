/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_ApplyDamageAndFlagHit.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
#include "game/engine.h"

extern int func_ov022_020ad7b0(int obj);
extern void Ov022_ActorSetHp(int obj, int v);

void Ov022_ApplyDamageAndFlagHit(int obj, unsigned int v, int mode) {
    int ok = 1;
    if (mode == 0 && Slot_EvalPackedParam(*(unsigned char *)(obj + 9), 0x41) != 0 &&
        func_ov022_020ad7b0(obj) != 0) {
        ok = 0;
    }
    if (ok == 0) return;
    if (mode != 0) Ov022_ActorSetHp(obj, v);
    else Ov022_ActorSetHp(obj, *(unsigned short *)(obj + 0x12) + v);
    if (Session_GetLocalPlayerIndex() != 0) return;
    *(kh_unaligned_u64 *)(obj + 0x46c) |= 0x10000000000LL;
}
