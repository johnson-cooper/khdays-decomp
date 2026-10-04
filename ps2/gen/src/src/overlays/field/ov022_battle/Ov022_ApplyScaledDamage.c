/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_ApplyScaledDamage.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Host only: applies damage scaled by the attacker's rate (percent) to the target and marks it hit.
 */

#include "game/engine.h"

extern void Ov022_ApplyDamageAndFlagHit(int obj, int v, int mode);
extern int gPartyMembers;

struct Row02093860 { char pad0[0xe]; unsigned short rate; char pad10[0x104 - 0x10]; };

void Ov022_ApplyScaledDamage(int arg0, int arg1, int arg2) {
    int q, prod;
    if (Session_GetLocalPlayerIndex() != 0) return;
    q = (((struct Row02093860 *)&gPartyMembers)[*(unsigned char *)(arg0 + 9)].rate << 0xc) / 100;
    if (arg2 > 0) arg1 = arg2 * 10 + arg1;
    prod = q * arg1;
    if (*(unsigned short *)(arg0 + 0x12) == 0) return;
    Ov022_ApplyDamageAndFlagHit(arg0, (prod + 0xfff) >> 0xc, 0);
    *(kh_unaligned_u64 *)(arg0 + 0x46c) |= 0x10000000000LL;
}
