/* PS2: mechanically prepared copy of src/overlays/players/ov104_player_roxas_dual_4/Ov104_UpdateAttachmentAndChannels.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Per-frame update of the ov049 enemy (x4: ov049/068/087/104): reads the node's animation frame,
 * feeds the attachment with the current tick and steps it; when the attachment reports idle, the
 * two 64-bit flag words at +0x464 and +0x46c get bit 16 (each guarded by 02030788 being idle);
 * then the two secondary channels at +0xda8 (0x164 apart) are driven from their configs at
 * +0x2c54 (0x54 apart) with the animation step at +0x2aba and the frame, and the common post-update
 * runs. */

#include "game/engine.h"

extern int Ov022_GetGlobal34(void);
extern void Ov022_ForwardToNodeHandler(void *attach, int tick);
extern void Ov022_InvokeCallback24IfBit0(void *attach);
extern int Ov022_AreStreamsIdle(void *attach);
extern void Ov002_WidgetScrollCommit(char *channel, char *config, int heading, int frame);
extern void func_ov022_020ad588(char *self);

void Ov104_UpdateAttachmentAndChannels(char *self)
{
    int frame = Anim_GetFrame(*(char **)(self + 0x20) + 4, 0);
    int i;
    char *config;
    char *channel;

    Ov022_ForwardToNodeHandler(*(void **)(self + 0x2644), Ov022_GetGlobal34());
    Ov022_InvokeCallback24IfBit0(*(void **)(self + 0x2644));
    if (Ov022_AreStreamsIdle(*(void **)(self + 0x2644)) == 0) {
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_s64 *)(self + 0x464) |= 0x10000;
        }
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_s64 *)(self + 0x46c) |= 0x10000;
        }
    }
    config = self + 0x54 + 0x2c00;
    channel = self + 0x1a8 + 0xc00;
    for (i = 0; i < 2; i++) {
        Ov002_WidgetScrollCommit(channel, config, *(short *)(self + 0x2aba), frame);
        config += 0x54;
        channel += 0x164;
    }
    func_ov022_020ad588(self);
}
