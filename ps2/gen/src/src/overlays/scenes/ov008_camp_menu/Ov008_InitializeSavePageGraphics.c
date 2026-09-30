/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_InitializeSavePageGraphics.c (ps2/tools/prep_sources.py). Do not edit. */
/* Sets up the save page's graphics: resets the 2D engines, assigns the VRAM banks, sets the display
 * modes, the background controls and priorities, the windows and the blending. */

#include "nitro/types.h"
#include "game/engine.h"

extern void GX_SetBankForTex(int bank);
extern void GX_SetBankForTexPltt(int offset);
extern void GX_SetBankForBG(int bank);
extern void GX_SetBankForOBJ(int bank);
extern void GX_SetBankForSubBG(int bank);
extern void GX_SetBankForSubOBJ(int bank);
extern void GX_SetGraphicsMode(int displayMode, int bgMode, int bg0Mode);
extern void GXS_SetGraphicsMode(int bgMode);
extern void GX_SetBankForOBJExtPltt(int bank);
extern void GX_SetBankForBGExtPltt(int bank);
extern void GX_SetBankForSubOBJExtPltt(int bank);
extern void GX_SetBankForSubBGExtPltt(int bank);
extern void G2x_SetBlendAlpha_(
    volatile u16 *reg, int firstTarget, int secondTarget, int eva, int evb);

typedef struct DisplayRegisters {
    volatile u32 dispcnt;
    u8 pad04[4];
    volatile u16 bg0cnt;
    volatile u16 bg1cnt;
    volatile u16 bg2cnt;
    volatile u16 bg3cnt;
} DisplayRegisters;

static volatile DisplayRegisters *const MAIN_DISPLAY =
    (volatile DisplayRegisters *)((unsigned int)kh_ds_io + 0x0);
static volatile DisplayRegisters *const SUB_DISPLAY =
    (volatile DisplayRegisters *)((unsigned int)kh_ds_io + 0x1000);
static volatile u16 *const REG_POWCNT1 = (volatile u16 *)((unsigned int)kh_ds_io + 0x304);
static volatile u16 *const REG_WIN0H = (volatile u16 *)((unsigned int)kh_ds_io + 0x40);
static volatile u16 *const REG_WIN0V = (volatile u16 *)((unsigned int)kh_ds_io + 0x44);
static volatile u16 *const REG_WININ = (volatile u16 *)((unsigned int)kh_ds_io + 0x48);
static volatile u16 *const REG_WINOUT = (volatile u16 *)((unsigned int)kh_ds_io + 0x4a);

void Ov008_InitializeSavePageGraphics(void)
{
    int base;

    Gfx_Reset2DEngines();
    *REG_POWCNT1 &= ~0x8000;

    GX_SetBankForTex(1);
    GX_SetBankForTexPltt(0x60);
    GX_SetBankForBG(2);
    GX_SetBankForOBJ(0x10);
    GX_SetBankForSubBG(4);
    GX_SetBankForSubOBJ(8);
    GX_SetGraphicsMode(1, 0, 1);

    base = 0x4084;
    MAIN_DISPLAY->bg1cnt = (MAIN_DISPLAY->bg1cnt & 0x43) | base;
    MAIN_DISPLAY->bg2cnt =
        (MAIN_DISPLAY->bg2cnt & 0x43) | (base + 0x200);
    MAIN_DISPLAY->bg3cnt =
        (MAIN_DISPLAY->bg3cnt & 0x43) | (base + 0x400);
    MAIN_DISPLAY->dispcnt =
        (MAIN_DISPLAY->dispcnt & 0xffcfffef) | 0x200010;
    MAIN_DISPLAY->bg0cnt &= ~3;
    MAIN_DISPLAY->bg1cnt = (MAIN_DISPLAY->bg1cnt & ~3) | 1;
    MAIN_DISPLAY->bg2cnt = (MAIN_DISPLAY->bg2cnt & ~3) | 2;
    MAIN_DISPLAY->bg3cnt = (MAIN_DISPLAY->bg3cnt & ~3) | 3;
    MAIN_DISPLAY->dispcnt =
        (MAIN_DISPLAY->dispcnt & ~0x1f00) | 0x1f00;

    GXS_SetGraphicsMode(0);

    base = 0x4080;
    SUB_DISPLAY->bg0cnt = (SUB_DISPLAY->bg0cnt & 0x43) | base;
    SUB_DISPLAY->bg1cnt =
        (SUB_DISPLAY->bg1cnt & 0x43) | (base + 0x210);
    SUB_DISPLAY->bg2cnt =
        (SUB_DISPLAY->bg2cnt & 0x43) | (base + 0x410);
    SUB_DISPLAY->bg3cnt =
        (SUB_DISPLAY->bg3cnt & 0x43) | (base + 0x610);
    SUB_DISPLAY->dispcnt =
        (SUB_DISPLAY->dispcnt & 0xffcfffef) | 0x200010;
    SUB_DISPLAY->dispcnt =
        (SUB_DISPLAY->dispcnt & ~0x1f00) | 0x1f00;

    *REG_WININ = (*REG_WININ & ~0x3f) | 0x3f;
    *REG_WINOUT = (*REG_WINOUT & ~0x3f) | 0x3b;
    *REG_WIN0H = 0x68e9;
    *REG_WIN0V = 0x1899;
    MAIN_DISPLAY->dispcnt =
        (MAIN_DISPLAY->dispcnt & ~0xe000) | 0x2000;

    GX_SetBankForOBJExtPltt(0);
    GX_SetBankForBGExtPltt(0);
    GX_SetBankForSubOBJExtPltt(0);
    GX_SetBankForSubBGExtPltt(0);
    G2x_SetBlendAlpha_((volatile u16 *)((unsigned int)kh_ds_io + 0x50), 1, 0x1e, 0x10, 0x10);
}

