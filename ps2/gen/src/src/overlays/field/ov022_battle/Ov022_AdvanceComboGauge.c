/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_AdvanceComboGauge.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Combo/gauge advance, gated on Session_GetLocalPlayerIndex idle and obj bit 16 clear. Requires the
 * byte at obj+0x2ab4 (state) or param2 non-zero. A small switch on param2 picks a step delta (2 for
 * kind 0, 5 for kinds 1/2, bumping the counter at obj+0x2ab3 on kinds 1[if param3>0]/2), then +1
 * more if lookup Slot_EvalPackedParam(obj[9],0x4f) is positive. Clamps the counter (>=0xf -> 2)
 * and, unless the state byte is already 2/3/4, sets state=1, adds the delta to obj+0x2ab5, and when
 * the counter >=0xb caps that at 10 and sets state=2. */

extern int Session_GetLocalPlayerIndex(void);
extern int Slot_EvalPackedParam(int a, int b);

void Ov022_AdvanceComboGauge(int obj, int param2, int param3) {
    unsigned char *p = (unsigned char *)(obj + 0x2aa4);
    int r5;
    if (Session_GetLocalPlayerIndex() != 0) return;
    if ((*(kh_unaligned_u64 *)obj & 0x10000LL) != 0) return;
    if (p[0x10] == 0 && param2 == 0) return;
    switch (param2) {
    case 0:
        r5 = 2;
        break;
    case 1:
        r5 = 5;
        if (param3 > 0) p[0xf]++;
        break;
    case 2:
        r5 = 5;
        p[0xf]++;
        break;
    }
    if (Slot_EvalPackedParam(*(unsigned char *)(obj + 9), 0x4f) > 0) r5++;
    if (p[0xf] >= 0xf) p[0xf] = 2;
    if (p[0x10] != 2 && p[0x10] != 3 && p[0x10] != 4) {
        p[0x10] = 1;
        p[0x11] += r5;
        if (p[0xf] >= 0xb) {
            p[0x11] = 10;
            p[0x10] = 2;
        }
    }
}
