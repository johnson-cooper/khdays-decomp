/* PS2: mechanically prepared copy of src/engine/Pad_Sample.c (ps2/tools/prep_sources.py). Do not edit. */
/* Samples the pad once per frame: keeps the previous state, reads the buttons (REG_KEYINPUT plus
 * the ARM7-shared X/Y/debug bits, active-low, masked to 0x2fff) unless the lid is closed, derives
 * the newly pressed set and stamps the change time (vblank count) of every button that changed.
 * Always returns 1. */

#include "nitro/types.h"

typedef struct {
    u16 cont;       /* 0x00 */
    u16 prev;       /* 0x02 */
    u16 trig;       /* 0x04 */
} PadState;

extern PadState gPadHeld;
extern unsigned int gPadPressTimes[];
extern unsigned int VBlank_GetCount(void);   /* GetVBlankCount */

int Pad_Sample(void)
{
    u16 bit = 1;
    u16 prev;
    u16 cont;
    unsigned int now;
    int i;

    gPadHeld.prev = gPadHeld.cont;
    if ((*(volatile u16 *)((unsigned int)kh_ds_hiram + 0x1ffa8) & 0x8000) >> 15) {
        cont = 0;
    } else {
        cont = ((*(volatile u16 *)((unsigned int)kh_ds_io + 0x130) | *(volatile u16 *)((unsigned int)kh_ds_hiram + 0x1ffa8)) ^ 0x2fff) & 0x2fff;
    }
    gPadHeld.cont = cont;
    prev = gPadHeld.prev;
    cont = (u16)gPadHeld.cont;     /* the cast gives the reloaded state its own value, allocated after prev */
    gPadHeld.trig = ~prev & cont;
    now = VBlank_GetCount();
    for (i = 0; i < 12; i++) {
        if ((u16)(prev ^ cont) & bit) {
            gPadPressTimes[i] = now;
        }
        bit = bit << 1;
    }
    return 1;
}
