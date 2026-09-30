/* PS2: mechanically prepared copy of src/overlays/system/ov024_mobiclip/Ov024_MobiClip_SetUpSubEngine.c (ps2/tools/prep_sources.py). Do not edit. */
/* MobiClip: set the sub engine up for movie playback.
 *
 * The sub-screen twin of the main-engine setup: its own VRAM banks and
 * graphics mode, the same four background screen blocks, the sprite grid on
 * this screen, the same affine reset sixteen scanlines up, and the same
 * hide-everything-then-reorder finish.
 */

#include "nitro/types.h"

struct Mtx22 {
    int m[4];
};

extern void GX_SetBankForSubBG(int nBank);
extern void GX_SetBankForSubOBJ(int nBank);
extern void GXS_SetGraphicsMode(int nMode);
extern void Ov024_MobiClip_LoadSpriteGrid(int bMainScreen);
extern void MTX_Identity22_(struct Mtx22 *pMtx);
extern void G2x_SetBGyAffine_(volatile void *pReg, const struct Mtx22 *pMtx,
                              int nCentreX, int nCentreY, int nX, int nY);

void Ov024_MobiClip_SetUpSubEngine(void)
{
    struct Mtx22 mtx;

    GX_SetBankForSubBG(4);
    GX_SetBankForSubOBJ(8);
    GXS_SetGraphicsMode(3);

    {
        volatile u16 *reg_db_bg0cnt = (volatile u16 *)((unsigned int)kh_ds_io + 0x1008);
        volatile u16 *reg_db_bg1cnt = (volatile u16 *)((unsigned int)kh_ds_io + 0x100a);
        volatile u16 *reg_db_bg2cnt = (volatile u16 *)((unsigned int)kh_ds_io + 0x100c);
        volatile u16 *reg_db_bg3cnt = (volatile u16 *)((unsigned int)kh_ds_io + 0x100e);

        *reg_db_bg0cnt = (u16)((*reg_db_bg0cnt & 0x43) | 0xc00);
        *reg_db_bg1cnt = (u16)((*reg_db_bg1cnt & 0x43) | 0xd00);
        *reg_db_bg2cnt = (u16)((*reg_db_bg2cnt & 0x43) | 0xe00);
        *reg_db_bg3cnt = (u16)((*reg_db_bg3cnt & 0x43) | 0x284 | 0x4000);
    }

    Ov024_MobiClip_LoadSpriteGrid(0);
    MTX_Identity22_(&mtx);
    G2x_SetBGyAffine_((volatile void *)((unsigned int)kh_ds_io + 0x1030), &mtx, 0, 0, 0, -0x10);

    {
        volatile u32 *reg_db_dispcnt = (volatile u32 *)((unsigned int)kh_ds_io + 0x1000);
        volatile u32 *reg_db_bg0ofs = (volatile u32 *)((unsigned int)kh_ds_io + 0x1010);
        volatile u32 *reg_db_bg1ofs = (volatile u32 *)((unsigned int)kh_ds_io + 0x1014);
        volatile u32 *reg_db_bg2ofs = (volatile u32 *)((unsigned int)kh_ds_io + 0x1018);
        volatile u16 *reg_db_bg0cnt = (volatile u16 *)((unsigned int)kh_ds_io + 0x1008);
        volatile u16 *reg_db_bg1cnt = (volatile u16 *)((unsigned int)kh_ds_io + 0x100a);
        volatile u16 *reg_db_bg2cnt = (volatile u16 *)((unsigned int)kh_ds_io + 0x100c);
        volatile u16 *reg_db_bg3cnt = (volatile u16 *)((unsigned int)kh_ds_io + 0x100e);

        *reg_db_dispcnt = *reg_db_dispcnt & ~0x1f00;
        *reg_db_bg0ofs = 0;
        *reg_db_bg1ofs = 1;
        *reg_db_bg2ofs = 0x10000;
        *reg_db_bg0cnt = (u16)(*reg_db_bg0cnt & ~3);
        *reg_db_bg1cnt = (u16)((*reg_db_bg1cnt & ~3) | 1);
        *reg_db_bg2cnt = (u16)((*reg_db_bg2cnt & ~3) | 2);
        *reg_db_bg3cnt = (u16)((*reg_db_bg3cnt & ~3) | 3);
    }
}
