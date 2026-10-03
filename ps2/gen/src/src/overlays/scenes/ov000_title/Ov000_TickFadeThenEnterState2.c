/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/Ov000_TickFadeThenEnterState2.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Fades the sub screen in over time and, when done, enters state 2. */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct {
    u8 pad_0000[0x4ad0];
    int active_state;
    u8 pad_4ad4[0x2c0];
    int transition_flag;
} OverlayContext;

extern OverlayContext *volatile data_ov000_0205ac24;
extern u64 OS_GetTick(void);
extern int kh_rt_ll_udiv_w_32(u64 value, u32 divisor, int mode);

void Ov000_TickFadeThenEnterState2(void) {
    u64 elapsed =
        OS_GetTick() - kh_read_u64_le_unaligned((u8 *)data_ov000_0205ac24 + 0x4ae4);

    SetMasterBrightnessSub(kh_rt_ll_udiv_w_32(elapsed, 0x4cb5, 0) - 16);
    if (elapsed <= 0x4cb51) {
        return;
    }

    kh_write_u64_le_unaligned((u8 *)data_ov000_0205ac24 + 0x4ae4, OS_GetTick());
    SetMasterBrightnessSub(0);
    data_ov000_0205ac24->active_state = 2;
    data_ov000_0205ac24->transition_flag = 0;
}
