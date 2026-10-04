/* PS2: mechanically prepared copy of src/overlays/players/ov050_player_axel_2/Ov050_initSubObjectsSetU64Flags.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Forwards the global command to the three effect streams and runs their callbacks; when any stream
 * is still busy, sets bit 16 of the local player's two 64-bit flag words; then finishes the
 * battle-module update. */

#include "game/engine.h"

extern int Ov022_GetGlobal34(void);
extern void Ov022_ForwardToNodeHandler(int a, int b);
extern void Ov022_InvokeCallback24IfBit0(int a);
extern int Ov022_AreStreamsIdle(int a);
extern void func_ov022_020ad588(int a);

void Ov050_initSubObjectsSetU64Flags(int this) {
    int v;
    v = Ov022_GetGlobal34();
    Ov022_ForwardToNodeHandler(*(int *)(this + 0x2644), v);
    v = Ov022_GetGlobal34();
    Ov022_ForwardToNodeHandler(*(int *)(this + 0x2644) + 0x30, v);
    v = Ov022_GetGlobal34();
    Ov022_ForwardToNodeHandler(*(int *)(this + 0x2644) + 0x60, v);
    Ov022_InvokeCallback24IfBit0(*(int *)(this + 0x2644));
    Ov022_InvokeCallback24IfBit0(*(int *)(this + 0x2644) + 0x30);
    Ov022_InvokeCallback24IfBit0(*(int *)(this + 0x2644) + 0x60);
    if (Ov022_AreStreamsIdle(*(int *)(this + 0x2644)) == 0 ||
        Ov022_AreStreamsIdle(*(int *)(this + 0x2644) + 0x30) == 0 ||
        Ov022_AreStreamsIdle(*(int *)(this + 0x2644) + 0x60) == 0) {
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_u64 *)(this + 0x464) |= 0x10000;
        }
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_u64 *)(this + 0x46c) |= 0x10000;
        }
    }
    func_ov022_020ad588(this);
}
