/* PS2: mechanically prepared copy of src/overlays/players/ov092_player_demyx_4/Ov092_UpdateAimAndFlagLocal.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Per-frame update: draws the charge effect, updates and draws the effect nodes, and sets bit 16 of
 * the local player's two 64-bit flag words while either effect stream is busy. */

#include "game/engine.h"

extern void Ov092_PickChargeLevelAndDraw(int self);
extern void Ov092_ForwardWithHeaderOffset(int a, int b, int c);
extern void Ov092_ForwardPlus14IfFlag694(int a, int b);
extern int Ov022_AreStreamsIdle(int a);
extern int data_ov092_020bc4e0;

void Ov092_UpdateAimAndFlagLocal(int self) {
    char *blk = (char *)(*(int *)&data_ov092_020bc4e0 + 0x194);
    Ov092_PickChargeLevelAndDraw(self);
    Ov092_ForwardWithHeaderOffset(self, (int)(blk + 0x2c00), *(short *)(self + 0x2a00 + 0xba));
    Ov092_ForwardPlus14IfFlag694(self, (int)(blk + 0x2c00));
    if (Ov022_AreStreamsIdle(*(int *)(self + 0x2000 + 0x644) + 0x30) != 0) {
        if (Ov022_AreStreamsIdle(*(int *)(self + 0x2000 + 0x644) + 0x60) != 0) return;
    }
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x464) |= 0x10000;
    }
    if (Session_GetLocalPlayerIndex() != 0) return;
    *(kh_unaligned_s64 *)((char *)self + 0x46c) |= 0x10000;
}
