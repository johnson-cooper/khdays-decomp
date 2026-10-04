/* PS2: mechanically prepared copy of src/overlays/players/ov094_player_lexaeus_4/Ov094_UpdateAllPartsAndFlagLocal.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Ticks all ov094 scene-part state machines and publishes local-player flags after charge begins.
 */

#include "game/engine.h"

extern void Ov094_StepCharge(int a, int b, int c);
extern void Ov094_UpdateTracksWhileActive(int a, int b, int c);
extern void Ov094_DriveSwingSequence(int a, int b, int c);
extern void Ov094_PlaceChargeEffect(int a, int b, int c);
extern void Ov094_TickChargeStateGuarded(int a, int b, int c);

void Ov094_UpdateAllPartsAndFlagLocal(int self, char *blk, int arg) {
    int any = 0;
    if (*(int *)(blk + 0x228) != 0) any = 1;
    Ov094_StepCharge(self, (int)(blk + 0x228), arg);
    Ov094_UpdateTracksWhileActive(self, (int)blk, arg);
    Ov094_DriveSwingSequence(self, (int)(blk + 0x118), arg);
    Ov094_PlaceChargeEffect(self, (int)(blk + 0x338), arg);
    Ov094_TickChargeStateGuarded(self, (int)(blk + 0x44 + 0x400), arg);
    if (any == 0) return;
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x464) |= 0x10000;
    }
    if (Session_GetLocalPlayerIndex() != 0) return;
    *(kh_unaligned_s64 *)((char *)self + 0x46c) |= 0x10000;
}
