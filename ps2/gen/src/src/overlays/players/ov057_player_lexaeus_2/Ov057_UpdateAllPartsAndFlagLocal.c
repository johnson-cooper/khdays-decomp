/* PS2: mechanically prepared copy of src/overlays/players/ov057_player_lexaeus_2/Ov057_UpdateAllPartsAndFlagLocal.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Ticks all ov057 scene-part state machines and publishes local-player flags after charge begins.
 */

#include "game/engine.h"

extern void Ov057_TickChargeSequence(int a, int b, int c);
extern void Ov057_UpdateTracksWhileActive(int a, int b, int c);
extern void Ov057_DriveSwingSequence(int a, int b, int c);
extern void Ov057_TickForwardEffectSequence(int a, int b, int c);
extern void Ov057_TickChargeStateGuarded(int a, int b, int c);

void Ov057_UpdateAllPartsAndFlagLocal(int self, char *blk, int arg) {
    int any = 0;
    if (*(int *)(blk + 0x228) != 0) any = 1;
    Ov057_TickChargeSequence(self, (int)(blk + 0x228), arg);
    Ov057_UpdateTracksWhileActive(self, (int)blk, arg);
    Ov057_DriveSwingSequence(self, (int)(blk + 0x118), arg);
    Ov057_TickForwardEffectSequence(self, (int)(blk + 0x338), arg);
    Ov057_TickChargeStateGuarded(self, (int)(blk + 0x44 + 0x400), arg);
    if (any == 0) return;
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x464) |= 0x10000;
    }
    if (Session_GetLocalPlayerIndex() != 0) return;
    *(kh_unaligned_s64 *)((char *)self + 0x46c) |= 0x10000;
}
