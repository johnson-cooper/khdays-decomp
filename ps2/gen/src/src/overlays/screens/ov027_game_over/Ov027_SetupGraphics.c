/* PS2: mechanically prepared copy of src/overlays/screens/ov027_game_over/Ov027_SetupGraphics.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov027_SetupGraphics -- Ov027_SetupGraphics: bring both engines up for the game-over scene.
 * After GX_Init both master brightnesses start black (-16) in a wireless session (bit 2 of
 * data_0204c240) or white (+16) otherwise -- the scene fades from there.  All VRAM goes to the
 * LCDC (0x1ff) and 0xa4000 bytes from 0x06800000 are cleared, the sound mutex released
 * (020065d4), then the banks are handed out: texture 7, OBJ palette at 0x60, BG 8, no BG
 * extended palette; graphics mode 1 / 0 / 1 (020056b4).  BG1 / BG2 get their control words
 * (0x704 / 0x170c over the two preserved bits), DISPCNT shows BG0..BG2, the priorities become
 * BG0 0, BG2 1, BG1 2 and DISP3DCNT loses bits 1, 2, 12..13 for bit 3.  The sprite layer is
 * reset (02010f08, 02010e80 3 / 1, 0201133c, 02011174 with 0x8000), the ov002 display
 * (02076028) put on the top screen and the game mode set to 0 (02023c30).  The sub engine
 * blends BG0 against the backdrop at 1 / 0x26 / 0 / 16 (G2x_SetBlendAlpha_ on BLDCNT), runs in
 * graphics mode 0 with VRAM bank 0x180 for its BGs, BG1 control 0x1508 and DISPCNT showing
 * BG1 only. */

#include "nitro/types.h"
#include "game/engine.h"

static volatile u32 *const REG_DISPCNT = (volatile u32 *)((unsigned int)kh_ds_io + 0x0);
static volatile u16 *const REG_BG0CNT = (volatile u16 *)((unsigned int)kh_ds_io + 0x8);
static volatile u16 *const REG_BG1CNT = (volatile u16 *)((unsigned int)kh_ds_io + 0xa);
static volatile u16 *const REG_BG2CNT = (volatile u16 *)((unsigned int)kh_ds_io + 0xc);
static volatile u16 *const REG_DISP3DCNT = (volatile u16 *)((unsigned int)kh_ds_io + 0x60);
static volatile u16 *const REG_BLDCNT = (volatile u16 *)((unsigned int)kh_ds_io + 0x50);
static volatile u32 *const REG_DB_DISPCNT = (volatile u32 *)((unsigned int)kh_ds_io + 0x1000);
static volatile u16 *const REG_DB_BG1CNT = (volatile u16 *)((unsigned int)kh_ds_io + 0x100a);

extern void GX_SetBankForLCDC(int nBanks);
extern void MIi_CpuClearFast(u32 nValue, void *pDst, u32 nSize);
extern void GX_DisableBankForLCDC(void);
extern void GX_SetBankForTex(int nBank);
extern void GX_SetBankForTexPltt(int nOffset);
extern void GX_SetBankForBG(int nBank);
extern void GX_SetBankForBGExtPltt(int nBank);
extern void GX_SetGraphicsMode(int nMode, int nBgMode, int nBg0Mode);    /* GX_SetGraphicsMode */
extern void NNS_GfdResetFrmTexVramState(void);
extern void NNS_GfdInitFrmTexVramManager(int nA, int nB);
extern void NNS_GfdResetFrmPlttVramState(void);
extern void NNS_GfdInitFrmPlttVramManager(int nA, int nB);
extern void Ov002_SetDisplaySwap(int bTop);                          /* ov002: put the display on the top screen */
extern void G2x_SetBlendAlpha_(volatile u16 *pReg, int nPlane1, int nPlane2, int nEva, int nEvb);
extern void GXS_SetGraphicsMode(int nMode);
extern void GX_SetBankForSubBG(int nBank);
extern u8   data_0204c240;                                          /* session bits */

void Ov027_SetupGraphics(void)
{
    Gfx_Reset2DEngines();
    SetMasterBrightnessMain((data_0204c240 & 4) ? -16 : 16);
    SetMasterBrightnessSub((data_0204c240 & 4) ? -16 : 16);
    GX_SetBankForLCDC(0x1ff);
    MIi_CpuClearFast(0, (void *)((unsigned int)kh_ds_vram + 0x0), 0xa4000);
    GX_DisableBankForLCDC();
    GX_SetBankForTex(7);
    GX_SetBankForTexPltt(0x60);
    GX_SetBankForBG(8);
    GX_SetBankForBGExtPltt(0);
    GX_SetGraphicsMode(1, 0, 1);
    *REG_BG1CNT = (*REG_BG1CNT & 0x43) | 0x704;
    *REG_BG2CNT = (*REG_BG2CNT & 0x43) | 0x170c;
    *REG_DISPCNT = (*REG_DISPCNT & 0xffffe0ff) | 0x700;
    *REG_BG0CNT = *REG_BG0CNT & ~3;
    *REG_BG2CNT = (*REG_BG2CNT & ~3) | 1;
    *REG_BG1CNT = (*REG_BG1CNT & ~3) | 2;
    *REG_DISP3DCNT &= 0xffffcffd;
    *REG_DISP3DCNT &= 0xcffb;
    *REG_DISP3DCNT = (*REG_DISP3DCNT & ~0x3000) | 8;
    NNS_GfdResetFrmTexVramState();
    NNS_GfdInitFrmTexVramManager(3, 1);
    NNS_GfdResetFrmPlttVramState();
    NNS_GfdInitFrmPlttVramManager(0x8000, 1);
    Ov002_SetDisplaySwap(1);
    SetGameMode(0);
    G2x_SetBlendAlpha_(REG_BLDCNT, 1, 0x26, 0, 0x10);
    GXS_SetGraphicsMode(0);
    GX_SetBankForSubBG(0x180);
    *REG_DB_BG1CNT = (*REG_DB_BG1CNT & 0x43) | 0x1508;
    *REG_DB_DISPCNT = (*REG_DB_DISPCNT & 0xffffe0ff) | 0x200;
}
