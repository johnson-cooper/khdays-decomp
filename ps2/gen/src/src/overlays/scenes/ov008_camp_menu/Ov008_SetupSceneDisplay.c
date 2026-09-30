/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_SetupSceneDisplay.c (ps2/tools/prep_sources.py). Do not edit. */
/* Configures the ov008 scene display: initializes GX, assigns VRAM banks, sets blend/clear state,
 * programs main/sub display controls and BG layers, clears BG1 character data, and configures
 * window 0 bounds and masks. */

#include "nitro/types.h"
#include "game/engine.h"

extern void GX_SetBankForTex(int bank);
extern void GX_SetBankForTexPltt(int bank);
extern void G2x_SetBlendAlpha_(volatile u16 *reg, int firstTarget,
                              int secondTarget, int eva, int evb);
extern void G3X_SetClearColor(int red, int green, int blue, int alpha,
                             int polygonId);
extern void NNS_GfdResetFrmTexVramState(void);
extern void NNS_GfdInitFrmTexVramManager(int mode, int enable);
extern void NNS_GfdResetFrmPlttVramState(void);
extern void NNS_GfdInitFrmPlttVramManager(int value, int enable);
extern void GX_SetBankForOBJExtPltt(int bank);
extern void GX_SetBankForOBJ(int bank);
extern void GX_SetBankForBG(int bank);
extern void GX_SetBankForBGExtPltt(int bank);
extern void GX_SetGraphicsMode(int displayMode, int bgMode, int bg0Mode);
extern void GX_SetBankForSubBG(int bank);
extern void GX_SetBankForSubBGExtPltt(int bank);
extern void GX_SetBankForSubOBJ(int bank);
extern void GX_SetBankForSubOBJExtPltt(int bank);
extern void Ov008_SetRenderModeWord(int mode);
extern void *G2_GetBG1CharPtr(void);
extern void MIi_CpuClearFast(u32 value, void *destination, u32 size);

#define REG_DISPCNT (*(volatile u32 *)((unsigned int)kh_ds_io + 0x0))
#define REG_DISP3DCNT (*(volatile u16 *)((unsigned int)kh_ds_io + 0x60))
#define REG_DISPCNT_SUB (*(volatile u32 *)((unsigned int)kh_ds_io + 0x1000))
#define REG_POWCNT1 (*(volatile u16 *)((unsigned int)kh_ds_io + 0x304))

typedef struct DisplayRegisters {
    volatile u32 dispcnt;
    u8 pad_04[4];
    volatile u16 bg0cnt;
    volatile u16 bg1cnt;
    volatile u16 bg2cnt;
    volatile u16 bg3cnt;
    u8 pad_10[0x30];
    volatile u16 win0h;
    volatile u16 win1h;
    volatile u16 win0v;
    volatile u16 win1v;
    volatile u16 winIn;
    volatile u16 winOut;
} DisplayRegisters;

static volatile DisplayRegisters *const MAIN_DISPLAY =
    (volatile DisplayRegisters *)((unsigned int)kh_ds_io + 0x0);
static volatile DisplayRegisters *const SUB_DISPLAY =
    (volatile DisplayRegisters *)((unsigned int)kh_ds_io + 0x1000);

void Ov008_SetupSceneDisplay(void)
{
    void *bg1CharData;

    Gfx_Reset2DEngines();
    GX_SetBankForTex(2);
    GX_SetBankForTexPltt(0x60);

    REG_DISP3DCNT = REG_DISP3DCNT & 0xffffcffd;
    REG_DISP3DCNT = REG_DISP3DCNT & 0xcffb;
    REG_DISP3DCNT = (REG_DISP3DCNT & ~0x3000) | 8;
    G2x_SetBlendAlpha_((volatile u16 *)((unsigned int)kh_ds_io + 0x50), 1, 10, 0, 0x10);
    G3X_SetClearColor(0, 0, 0x7fff, 0x3f, 0);

    NNS_GfdResetFrmTexVramState();
    NNS_GfdInitFrmTexVramManager(1, 1);
    NNS_GfdResetFrmPlttVramState();
    NNS_GfdInitFrmPlttVramManager(0x8000, 1);

    GX_SetBankForOBJExtPltt(0);
    GX_SetBankForOBJ(1);
    MAIN_DISPLAY->dispcnt =
        (MAIN_DISPLAY->dispcnt & 0xffcfffef) | 0x200010;
    GX_SetBankForBG(0x10);
    GX_SetBankForBGExtPltt(0);
    GX_SetGraphicsMode(1, 0, 1);
    MAIN_DISPLAY->dispcnt =
        (MAIN_DISPLAY->dispcnt & ~0x1f00) | 0x1f00;

    GX_SetBankForSubBG(4);
    GX_SetBankForSubBGExtPltt(0);
    GX_SetBankForSubOBJ(8);
    SUB_DISPLAY->dispcnt =
        (SUB_DISPLAY->dispcnt & 0xffcfffef) | 0x200010;
    GX_SetBankForSubOBJExtPltt(0);
    REG_POWCNT1 |= 0x8000;

    MAIN_DISPLAY->bg1cnt = (MAIN_DISPLAY->bg1cnt & 0x43) | 0x4604;
    MAIN_DISPLAY->bg2cnt = (MAIN_DISPLAY->bg2cnt & 0x43) | 0x4c00;
    MAIN_DISPLAY->bg3cnt = (MAIN_DISPLAY->bg3cnt & 0x43) | 0x4e00;

    Ov008_SetRenderModeWord(1);
    bg1CharData = G2_GetBG1CharPtr();
    MIi_CpuClearFast(0, bg1CharData, 0x20);

    MAIN_DISPLAY->winIn = (MAIN_DISPLAY->winIn & ~0x3f) | 0x1f;
    MAIN_DISPLAY->winOut = (MAIN_DISPLAY->winOut & ~0x3f) | 0x1e;
    MAIN_DISPLAY->win0h = 0x76ca;
    MAIN_DISPLAY->win0v = 0x1e82;
    MAIN_DISPLAY->dispcnt =
        (MAIN_DISPLAY->dispcnt & ~0xe000) | 0x2000;
}
