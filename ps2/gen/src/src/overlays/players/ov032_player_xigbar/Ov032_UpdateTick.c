/* PS2: mechanically prepared copy of src/overlays/players/ov032_player_xigbar/Ov032_UpdateTick.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Per-frame update of the ov032 enemy (and its byte-identical twins): reads the model's
 * track-0 frame, forwards the shared tick to the message handler (463c), and while the +0x2644
 * item's +0x30 sub-object is idle raises bit 16 of both 64-bit flag words (+0x464, +0x46c) on
 * the local player's session. Bit 2 of the +0x2c30 flags pins the frame at 0x1d000; the two
 * +0xdac animation blocks are wound to it on channels 0/1 while it is inside their length.
 * Finally 020ad588 runs. */

#include "game/engine.h"

extern int Ov022_GetGlobal34(void);
extern void Ov032_HandleMessage(char *self, int tick);
extern int Ov022_AreStreamsIdle(char *sub);
extern void Anim_SetFrameWrapped(void *animation, int channel, int frame);
extern void func_ov022_020ad588(char *self);

struct b3 { unsigned char b0 : 1, b1 : 1, b2 : 1; };

void Ov032_UpdateTick(char *self)
{
    int frame;
    int i;
    char *anim;

    frame = Anim_GetFrame(*(char **)(self + 0x20) + 4, 0);
    Ov032_HandleMessage(self, Ov022_GetGlobal34());
    if (Ov022_AreStreamsIdle(*(char **)(self + 0x2000 + 0x644) + 0x30) == 0) {
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_s64 *)(self + 0x464) |= 0x10000;
        }
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_s64 *)(self + 0x46c) |= 0x10000;
        }
    }
    if (((struct b3 *)(self + 0x2000 + 0xc30))->b2 != 0) {
        frame = 0x1d000;
    }
    anim = self + 0x1ac + 0xc00;
    for (i = 0; i < 2; i++) {
        if (frame < Anim_GetLengthQ12(anim, 0)) {
            Anim_SetFrameWrapped(anim, 0, frame);
            Anim_SetFrameWrapped(anim, 1, frame);
        }
        anim += 0x164;
    }
    func_ov022_020ad588(self);
}
