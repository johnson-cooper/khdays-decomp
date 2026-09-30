/* PS2: mechanically prepared copy of libs/nns/g2d/calls/ChangeBGModeByTableSub.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os.h"
#include "nitro/gx.h"

#define offsetof(type, member) ((u32)&(((type *)0)->member))

#define BG_MODE_WARNING 8

void GXS_SetGraphicsMode(GXBGMode bgMode);
inline GXBGMode GetBGModeSub (void)
{
    return (GXBGMode)(((*( REGType32v *) (((unsigned int)kh_ds_io + 0x0) + 0x1000)) & 0x00000007 ) >> 0 );
}

/* ChangeBGModeByTableSub -- NitroSystem g2d_Screen.c: ChangeBGModeByTableSub. */
void ChangeBGModeByTableSub (const u8 modeTable[])
{
    GXBGMode mode = (GXBGMode)modeTable[GetBGModeSub()];

    if (mode >= BG_MODE_WARNING) {
        mode -= BG_MODE_WARNING;
    }

    GXS_SetGraphicsMode(mode);
}
