/* PS2: mechanically prepared copy of src/overlays/system/ov024_mobiclip/Ov024_MobiClip_PresentOnVBlank.c (ps2/tools/prep_sources.py). Do not edit. */
/* MobiClip: present the next movie frame, once per vertical blank.
 *
 * The player draws alternate frames two ways -- one as background layers, the
 * next as the sprite grid -- so presenting a frame is just a matter of
 * switching which of the two the display shows. Each screen that is playing
 * gets its front buffer flipped, is marked as having presented, and has its
 * frame counter advanced; the frame alarm is waiting on that mark before it
 * hands the decoder the buffer that just came free.
 *
 * The work is confined to the vertical blank proper, scanlines 0xa0 to 0x103.
 */

#include "nitro/types.h"

#define DISPCNT_LAYERS   0x1f00
#define LAYERS_AS_BG     0x0f00
#define LAYERS_AS_SPRITE 0x1700

struct MobiClipFrameTimer {
    u8 pad0000[0x39];
    u8 nFrontBuffer;
    u8 bPresented;
    u8 pad003b[0x48 - 0x3b];
    int nPresented;
};

extern int data_ov024_02093a2c;
extern int data_ov024_0209ba48;

void Ov024_MobiClip_PresentOnVBlank(void)
{
    volatile u16 *reg_vcount = (volatile u16 *)((unsigned int)kh_ds_io + 0x6);
    struct MobiClipFrameTimer *pTimer;
    void (*pfnAfterPresent)(void);
    u32 nScanline;

    nScanline = *reg_vcount;
    if (nScanline < 0xa0 || nScanline >= 0x104) {
        return;
    }

    pTimer = ((struct MobiClipFrameTimer **)&data_ov024_02093a2c)[2];
    if (pTimer != 0 && pTimer->bPresented == 0) {
        volatile u32 *reg_dispcnt = (volatile u32 *)((unsigned int)kh_ds_io + 0x0);

        if (pTimer->nFrontBuffer == 0) {
            *reg_dispcnt = (*reg_dispcnt & ~DISPCNT_LAYERS) | LAYERS_AS_BG;
        } else {
            *reg_dispcnt = (*reg_dispcnt & ~DISPCNT_LAYERS) | LAYERS_AS_SPRITE;
        }
        pTimer->nFrontBuffer = (u8)(pTimer->nFrontBuffer ^ 1);
        pTimer->bPresented = 1;
        pTimer->nPresented++;
    }

    pTimer = ((struct MobiClipFrameTimer **)&data_ov024_02093a2c)[3];
    if (pTimer != 0 && pTimer->bPresented == 0) {
        volatile u32 *reg_db_dispcnt = (volatile u32 *)((unsigned int)kh_ds_io + 0x1000);

        if (pTimer->nFrontBuffer == 0) {
            *reg_db_dispcnt = (*reg_db_dispcnt & ~DISPCNT_LAYERS) | LAYERS_AS_BG;
        } else {
            *reg_db_dispcnt = (*reg_db_dispcnt & ~DISPCNT_LAYERS) | LAYERS_AS_SPRITE;
        }
        pTimer->nFrontBuffer = (u8)(pTimer->nFrontBuffer ^ 1);
        pTimer->bPresented = 1;
        pTimer->nPresented++;
    }

    pfnAfterPresent = ((void (**)(void))&data_ov024_0209ba48)[0x38];
    if (pfnAfterPresent != 0) {
        pfnAfterPresent();
    }
}
