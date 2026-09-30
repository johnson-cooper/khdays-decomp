/* PS2: mechanically prepared copy of src/overlays/system/ov024_mobiclip/Ov024_MobiClip_StepScreenFade.c (ps2/tools/prep_sources.py). Do not edit. */

/* The player object. The fade state sits far into it, which is why every access
 * below compiles to a base-plus-offset split rather than a single load. */

#include "nitro/types.h"

struct MobiClipPlayer {
    char pad0000[0x8be2];
    u8 nScreens;
    u8 pad8be3;
    int nToWhite;
    int nStep;
};

extern int Ov024_MobiClip_BufferedFrameCount(void);
extern void GXx_SetMasterBrightness_(volatile unsigned short *reg, int brightness);

#define REG_MASTER_BRIGHT     ((volatile unsigned short *)((unsigned int)kh_ds_io + 0x6c))
#define REG_DB_MASTER_BRIGHT  ((volatile unsigned short *)((unsigned int)kh_ds_io + 0x106c))

void Ov024_MobiClip_StepScreenFade(struct MobiClipPlayer *player) {
    int step;
    int level;

    step = player->nStep;
    if (step < 0) {
        if (Ov024_MobiClip_BufferedFrameCount() <= 0) {
            return;
        }
        GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, 0);
        GXx_SetMasterBrightness_(REG_DB_MASTER_BRIGHT, 0);
        return;
    }
    if (step >= 0x10) {
        return;
    }
    player->nStep = step + 1;

    level = player->nToWhite == 0 ? -player->nStep : player->nStep;

    switch (player->nScreens) {
    case 0:
        GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, level);
        break;
    case 1:
        GXx_SetMasterBrightness_(REG_DB_MASTER_BRIGHT, level);
        break;
    case 2:
        GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, level);
        GXx_SetMasterBrightness_(REG_DB_MASTER_BRIGHT, level);
        break;
    }
}
