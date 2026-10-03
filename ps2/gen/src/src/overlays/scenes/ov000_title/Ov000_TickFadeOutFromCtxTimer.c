/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/Ov000_TickFadeOutFromCtxTimer.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Advance the fade from the CONTEXT timer at +0x4ae4; once 0x4cb51 ticks have passed, restamp the
 * timer and copy next_state (+0x4ad8) into active_state (+0x4ad0). Fades OUT: the brightness
 * argument is -(elapsed / 0x4cb5), clamped at -16. Fade ramp: 16 steps over 0x4cb51 ticks (0x4cb51
 * / 0x4cb5 = 15.97), driven through SetMasterBrightnessSub with a NEGATIVE brightness. 16 is the DS
 * master brightness range -- the same 0x10 Game_RunSceneLoop writes. */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct {
    u8 pad_0000[0x4ad0];
    int active_state;
    int unknown_4ad4;
    int next_state;
} OverlayContext;

extern OverlayContext *volatile data_ov000_0205ac24;
extern u64 OS_GetTick(void);
extern int kh_rt_ll_udiv_w_32(u64 value, u32 divisor, int mode);

void Ov000_TickFadeOutFromCtxTimer(void) {
    u64 elapsed =
        OS_GetTick() - kh_read_u64_le_unaligned((u8 *)data_ov000_0205ac24 + 0x4ae4);

    SetMasterBrightnessSub(-kh_rt_ll_udiv_w_32(elapsed, 0x4cb5, 0));
    if (elapsed <= 0x4cb51) {
        return;
    }

    kh_write_u64_le_unaligned((u8 *)data_ov000_0205ac24 + 0x4ae4, OS_GetTick());
    SetMasterBrightnessSub(-16);
    {
        OverlayContext *context = data_ov000_0205ac24;
        context->active_state = context->next_state;
    }
}
