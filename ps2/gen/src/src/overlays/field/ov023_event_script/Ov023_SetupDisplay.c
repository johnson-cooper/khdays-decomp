/* PS2: mechanically prepared copy of src/overlays/field/ov023_event_script/Ov023_SetupDisplay.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov023_SetupDisplay -- Ov023_SetupDisplay: put both engines into the event scene's display
 * mode.  Any fade level (0201e428) other than the two extremes is forced to -0x10 and applied
 * to both screens (0201e374 / 0201e3cc); VRAM banks are handed out (texture 7, OBJ palette
 * 0x60, BG 0x10, no BG extended palette), the graphics mode set (020056b4 mode 1 / 0 / 1) and
 * the ov002 renderer told (02076028); DISP3DCNT loses bits 1, 2 and 12..13 and gets bit 3;
 * DISPCNT's BG mode field becomes 3, BG0 gets priority 1 and BG1 priority 0; the 3D clear
 * colour is reset (G3X_SetClearColor); the sub engine gets banks 0x180 / 8 and its DISPCNT
 * bits 4 and 21 with mode 1 (bits 8..12); finally the sprite layer is reset (02010f08,
 * 02010e80 3 / 1, 0201133c, 02011174 0x8000 / 1). */

#include "nitro/types.h"
#include "game/engine.h"

static volatile u16 *const REG_DISP3DCNT = (volatile u16 *)((unsigned int)kh_ds_io + 0x60);
static volatile u32 *const REG_DISPCNT = (volatile u32 *)((unsigned int)kh_ds_io + 0x0);
static volatile u16 *const REG_BG0CNT = (volatile u16 *)((unsigned int)kh_ds_io + 0x8);
static volatile u16 *const REG_BG1CNT = (volatile u16 *)((unsigned int)kh_ds_io + 0xa);
static volatile u32 *const REG_DISPCNT_SUB = (volatile u32 *)((unsigned int)kh_ds_io + 0x1000);

extern void GX_SetBankForTex(int nBank);
extern void GX_SetBankForTexPltt(int nOffset);
extern void GX_SetBankForBG(int nBank);
extern void GX_SetGraphicsMode(int nMode, int nBgMode, int nBg0Mode);    /* GX_SetGraphicsMode */
extern void Ov002_SetDisplaySwap(int nMode);
extern void GX_SetBankForBGExtPltt(int nBank);
extern void G3X_SetClearColor(u32 nColor, u32 nAlpha, u32 nDepth, u32 nPolygonId, int bFog);
extern void GX_SetBankForSubBG(int nBank);
extern void GX_SetBankForSubOBJ(int nBank);
extern void NNS_GfdResetFrmTexVramState(void);
extern void NNS_GfdInitFrmTexVramManager(int nA, int nB);
extern void NNS_GfdResetFrmPlttVramState(void);
extern void NNS_GfdInitFrmPlttVramManager(int nA, int nB);

void Ov023_SetupDisplay(void)
{
    int nLevel;

    nLevel = GetMasterBrightnessMain();
    if (nLevel != -0x10 && nLevel != 0x10) {
        nLevel = -0x10;
    }
    SetMasterBrightnessMain(nLevel);
    SetMasterBrightnessSub(nLevel);
    GX_SetBankForTex(7);
    GX_SetBankForTexPltt(0x60);
    GX_SetBankForBG(0x10);
    GX_SetGraphicsMode(1, 0, 1);
    Ov002_SetDisplaySwap(1);
    GX_SetBankForBGExtPltt(0);
    *REG_DISP3DCNT &= 0xffffcffd;
    *REG_DISP3DCNT &= 0xcffb;
    *REG_DISP3DCNT = (*REG_DISP3DCNT & ~0x3000) | 8;
    *REG_DISPCNT = (*REG_DISPCNT & ~0x1f00) | 0x300;
    *REG_BG0CNT = (*REG_BG0CNT & ~3) | 1;
    *REG_BG1CNT &= ~3;
    G3X_SetClearColor(0, 0x1f, 0x7fff, 0x3f, 0);
    GX_SetBankForSubBG(0x180);
    GX_SetBankForSubOBJ(8);
    *REG_DISPCNT_SUB = (*REG_DISPCNT_SUB & 0xffcfffef) | 0x10 | 0x200000;
    *REG_DISPCNT_SUB = (*REG_DISPCNT_SUB & ~0x1f00) | 0x1000;
    NNS_GfdResetFrmTexVramState();
    NNS_GfdInitFrmTexVramManager(3, 1);
    NNS_GfdResetFrmPlttVramState();
    NNS_GfdInitFrmPlttVramManager(0x8000, 1);
}
