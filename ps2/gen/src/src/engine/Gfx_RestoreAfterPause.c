/* PS2: mechanically prepared copy of src/engine/Gfx_RestoreAfterPause.c (ps2/tools/prep_sources.py). Do not edit. */
/* Re-enables the BG layers of both engines, resets the brightness, restores the graphics mode and
 * VBlank callback and refreshes the scene. */

#include "nitro/types.h"
#include "game/engine.h"

extern char *data_0204be08;
extern char gPauseRefreshName[16];

extern void G2x_SetBlendBrightness_(u16 *dst, u32 attr, int value);
/* Defined taking param_1 as GXDispMode: declared narrower here, which is what makes mwcc truncate the
 * argument at the call as the ROM does (declared as defined, the code comes out different). */
extern void GX_SetGraphicsMode(u16 param_1, u32 param_2, int param_3);
extern void Ov002_UpdatePanelBlend(void);
extern int Ov106_GetBrightness(void);

/* GBATEK: DISPCNT, main engine. */
#define REG_DISPCNT (*(volatile u32 *)((unsigned int)kh_ds_io + 0x0))

void Gfx_RestoreAfterPause(void)
{
    char *heap = (&data_0204be08)[1];

    if ((LoadGlobalU16At0() & 8) != 0) {
        REG_DISPCNT = (REG_DISPCNT & 0xffffe0ff) | 0x100;
    } else {
        REG_DISPCNT = (REG_DISPCNT & 0xffffe0ff) | 0x1f00;
    }

    G2x_SetBlendBrightness_((u16 *)((unsigned int)kh_ds_io + 0x50), 1, 0);

    {
        volatile u32 *subDispCnt = (volatile u32 *)((unsigned int)kh_ds_io + 0x1000);
        u32 attr = (*subDispCnt & 0x1f00) >> 8;
        G2x_SetBlendBrightness_((u16 *)((char *)subDispCnt + 0x50), attr, 0);
    }

    if (*(int *)(heap + 0xe4) != 0) {
        GX_SetGraphicsMode(0xe, 4, 1);
    }

    VBlank_UnregisterCallback(1, gPauseRefreshName);
    Ov002_UpdatePanelBlend();

    if (LoadGlobalU16At0() == 0x2a) {
        G2x_SetBlendBrightness_((u16 *)((unsigned int)kh_ds_io + 0x50), 1, Ov106_GetBrightness());
    }
}
