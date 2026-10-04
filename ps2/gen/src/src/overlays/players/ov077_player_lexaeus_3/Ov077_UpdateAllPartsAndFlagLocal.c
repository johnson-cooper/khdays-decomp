/* PS2: mechanically prepared copy of src/overlays/players/ov077_player_lexaeus_3/Ov077_UpdateAllPartsAndFlagLocal.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Updates all five ov077 scene parts and raises the actor local flags after the li_e0 controller
 * becomes active. */

#include "game/engine.h"

extern void Ov077_UpdateChargeSequence(int a, int b, int c);
extern void Ov077_UpdateTracksWhileActive(int a, int b, int c);
extern void Ov077_DriveSwingSequence(int a, int b, int c);
extern void Ov077_UpdateChargeEmitter(int a, int b, int c);
extern void Ov077_TickChargeStateGuarded(int a, int b, int c);

void Ov077_UpdateAllPartsAndFlagLocal(int self, char *blk, int arg) {
    int any = 0;
    if (*(int *)(blk + 0x228) != 0) any = 1;
    Ov077_UpdateChargeSequence(self, (int)(blk + 0x228), arg);
    Ov077_UpdateTracksWhileActive(self, (int)blk, arg);
    Ov077_DriveSwingSequence(self, (int)(blk + 0x118), arg);
    Ov077_UpdateChargeEmitter(self, (int)(blk + 0x338), arg);
    Ov077_TickChargeStateGuarded(self, (int)(blk + 0x44 + 0x400), arg);
    if (any == 0) return;
    if (Session_GetLocalPlayerIndex() == 0) {
        *(kh_unaligned_s64 *)((char *)self + 0x464) |= 0x10000;
    }
    if (Session_GetLocalPlayerIndex() != 0) return;
    *(kh_unaligned_s64 *)((char *)self + 0x46c) |= 0x10000;
}
