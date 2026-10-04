/* PS2: mechanically prepared copy of src/overlays/players/ov043_player_xemnas/Ov043_UpdateTick.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Per-frame update of the mission enemy: samples the model's track-0 frame, forwards the
 * shared tick to the animation-argument pass (3aec), and while the +0x2644 record's +0x30
 * sub-object is idle raises bit 16 of both 64-bit flag words (+0x464, +0x46c) on the local
 * player's session. Then the 3828 pass and 020ad588 run. */

#include "game/engine.h"

extern int Ov022_GetGlobal34(void);
extern void Ov043_PushAnimArg(char *self, int tick);
extern int Ov022_AreStreamsIdle(char *sub);
extern void Ov043_DriveScriptNode(char *self);
extern void func_ov022_020ad588(char *self);

void Ov043_UpdateTick(char *self)
{
    Anim_GetFrame(*(char **)(self + 0x20) + 4, 0);
    Ov043_PushAnimArg(self, Ov022_GetGlobal34());
    if (Ov022_AreStreamsIdle(*(char **)(self + 0x2000 + 0x644) + 0x30) == 0) {
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_s64 *)(self + 0x464) |= 0x10000;
        }
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_s64 *)(self + 0x46c) |= 0x10000;
        }
    }
    Ov043_DriveScriptNode(self);
    func_ov022_020ad588(self);
}
