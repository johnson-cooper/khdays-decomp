/* PS2: mechanically prepared copy of libs/nns/g2d/calls/ChangeBGModeByTableMain.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os.h"
#include "nitro/gx.h"

#define offsetof(type, member) ((u32)&(((type *)0)->member))

#define BG_MODE_WARNING 8

void GX_SetGraphicsMode(GXDispMode dispMode, GXBGMode bgMode, GXBG0As bg0_2d3d);
inline BOOL IsBG03D (void)
{
    return ((*( REGType32v *) (((unsigned int)kh_ds_io + 0x0) + 0x000)) & 0x00000008 ) != 0;
}
inline GXBGMode GetBGModeMain (void)
{
    return (GXBGMode)(((*( REGType32v *) (((unsigned int)kh_ds_io + 0x0) + 0x000)) & 0x00000007 ) >> 0 );
}

/* ChangeBGModeByTableMain -- NitroSystem g2d_Screen.c: ChangeBGModeByTableMain. */
void ChangeBGModeByTableMain (const u8 modeTable[])
{
    GXBGMode mode = (GXBGMode)modeTable[GetBGModeMain()];
    GXBG0As bg0as = IsBG03D() ? GX_BG0_AS_3D: GX_BG0_AS_2D;

    if (mode >= BG_MODE_WARNING) {
        mode -= BG_MODE_WARNING;
    }

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, mode, bg0as);
}
