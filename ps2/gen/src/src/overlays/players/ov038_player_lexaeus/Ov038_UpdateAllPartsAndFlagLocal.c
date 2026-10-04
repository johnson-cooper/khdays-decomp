/* PS2: mechanically prepared copy of src/overlays/players/ov038_player_lexaeus/Ov038_UpdateAllPartsAndFlagLocal.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Ticks all ov038 scene-part state machines and publishes local-player flags after charge begins.
 */

#include "game/engine.h"

extern void Ov038_StepCharge(int a, int b, int c);
extern void Ov038_UpdateTracksWhileActive(int a, int b, int c);
extern void Ov038_DriveSwingSequence(int a, int b, int c);
extern void Ov038_PlaceChargeEffect(int a, int b, int c);
extern void Ov038_TickChargeStateGuarded(int a, int b, int c);

void Ov038_UpdateAllPartsAndFlagLocal(int self, char *blk, int arg) {
    int any = 0;
    if (*(int *)(blk + 0x228) != 0) any = 1;
    Ov038_StepCharge(self, (int)(blk + 0x228), arg);
    Ov038_UpdateTracksWhileActive(self, (int)blk, arg);
    Ov038_DriveSwingSequence(self, (int)(blk + 0x118), arg);
    Ov038_PlaceChargeEffect(self, (int)(blk + 0x338), arg);
    Ov038_TickChargeStateGuarded(self, (int)(blk + 0x44 + 0x400), arg);
    if (any == 0) return;
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x464) |= 0x10000;
    }
    if (Session_GetLocalPlayerIndex() != 0) return;
    *(kh_unaligned_s64 *)((char *)self + 0x46c) |= 0x10000;
}
