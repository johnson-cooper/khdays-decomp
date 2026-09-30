/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_InitSubScreenGraphics.c (ps2/tools/prep_sources.py). Do not edit. */
/* Resets the texture VRAM managers and configures DISPCNT and the BG layers for the sub screen. */

#include "nitro/types.h"
#include "game/engine.h"

extern void  NNS_GfdResetFrmTexVramState(void);
extern void  NNS_GfdInitFrmTexVramManager(int a, int b);
extern void  NNS_GfdResetFrmPlttVramState(void);
extern void  NNS_GfdInitFrmPlttVramManager(int a, int b);
extern void  GX_SetBankForOBJ(int bank);
extern void  GX_SetBankForBG(int bank);
extern void  GX_SetBankForBGExtPltt(int bank);
extern void  GX_SetGraphicsMode(int a, int b, int c);
extern void  GX_SetBankForSubBG(int bank);
extern void  GX_SetBankForSubOBJ(int bank);
extern void  GX_SetBankForSubBGExtPltt(int bank);
extern void  GX_SetBankForSubOBJExtPltt(int bank);
extern void  Ov008_SetRenderModeWord(int arg);
extern void *G2_GetBG1CharPtr(void);
extern void  MIi_CpuClearFast(u32 value, void *dst, u32 size);

typedef struct DisplayRegisters {
    volatile u32 dispcnt;
    u8 pad_04[4];
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

void Ov008_InitSubScreenGraphics(void)
{
    void *charData;

    Gfx_Reset2DEngines();
    NNS_GfdResetFrmTexVramState();
    NNS_GfdInitFrmTexVramManager(1, 1);
    NNS_GfdResetFrmPlttVramState();
    NNS_GfdInitFrmPlttVramManager(0x10000, 1);

    MAIN_DISPLAY->dispcnt =
        (MAIN_DISPLAY->dispcnt & 0xffcfffef) | 0x10 | 0x200000;
    GX_SetBankForOBJ(1);
    GX_SetBankForBG(2);
    GX_SetBankForBGExtPltt(0);
    GX_SetGraphicsMode(1, 0, 1);
    MAIN_DISPLAY->dispcnt =
        (MAIN_DISPLAY->dispcnt & ~0x1f00) | 0x1e00;

    SUB_DISPLAY->dispcnt =
        (SUB_DISPLAY->dispcnt & 0xffcfffef) | 0x10 | 0x200000;
    GX_SetBankForSubBG(4);
    GX_SetBankForSubOBJ(8);
    GX_SetBankForSubBGExtPltt(0);
    GX_SetBankForSubOBJExtPltt(0);
    SUB_DISPLAY->dispcnt =
        (SUB_DISPLAY->dispcnt & ~0x1f00) | 0x1f00;

    *REG_POWCNT1 |= 0x8000;

    MAIN_DISPLAY->bg1cnt = (MAIN_DISPLAY->bg1cnt & 0x43) | 0x410;
    MAIN_DISPLAY->bg2cnt = (MAIN_DISPLAY->bg2cnt & 0x43) | 0x4008;
    MAIN_DISPLAY->bg3cnt = (MAIN_DISPLAY->bg3cnt & 0x43) | 0x208;
    SUB_DISPLAY->bg0cnt = (SUB_DISPLAY->bg0cnt & 0x43) | 0x4080;
    SUB_DISPLAY->bg1cnt = (SUB_DISPLAY->bg1cnt & 0x43) | 0x4290;
    SUB_DISPLAY->bg2cnt = (SUB_DISPLAY->bg2cnt & 0x43) | 0x4490;
    SUB_DISPLAY->bg3cnt = (SUB_DISPLAY->bg3cnt & 0x43) | 0x4690;

    Ov008_SetRenderModeWord(1);
    charData = G2_GetBG1CharPtr();
    MIi_CpuClearFast(0, charData, 0x20);
}
