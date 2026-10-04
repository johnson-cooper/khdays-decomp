/* PS2: mechanically prepared copy of src/overlays/players/ov102_player_donald_4/Ov102_PanelTick.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Per-frame tick of the ov102 panel: refreshes the sub-object at +0x2644 three times --
 * advance it by the frame delta, step it, then ask whether it has gone idle -- and when it
 * has, raises the 64-bit flag pairs at +0x464 and +0x46c, each guarded by its own check.
 * Finally republishes the pose.
 *
 * THE 64-BIT OR. The ROM's `orr rN, rN, #0` is not a no-op and not a macro artifact: it is
 * the HIGH HALF of a 64-bit OR on a pair of adjacent flag words. `*(kh_unaligned_s64 *)(p) |= mask`
 * emits exactly two loads, `orr` low with the mask, `orr` high with zero, two stores. It also
 * explains the two-step base (`add r0, r4, #0x64` then `[r0, #0x404]`): that is just how mwcc
 * addresses the high half, not a separate source construct. */

#include "game/engine.h"

extern int Ov022_GetGlobal34(void);
extern void Ov022_ForwardToNodeHandler(int a, int b);
extern void Ov022_InvokeCallback24IfBit0(int a);
extern int Ov022_AreStreamsIdle(int a);
extern void func_ov022_020ad588(char *self);

void Ov102_PanelTick(char *self)
{
    Ov022_ForwardToNodeHandler(*(int *)(self + 0x2644), Ov022_GetGlobal34());
    Ov022_InvokeCallback24IfBit0(*(int *)(self + 0x2644));
    if (Ov022_AreStreamsIdle(*(int *)(self + 0x2644)) == 0) {
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_s64 *)(self + 0x464) |= 0x10000;
        }
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_s64 *)(self + 0x46c) |= 0x10000;
        }
    }
    func_ov022_020ad588(self);
}
