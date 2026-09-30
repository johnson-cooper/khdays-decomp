/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/Ov025_SetupDisplay.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov025_SetupDisplay -- Ov025_SetupDisplay: the records scene's display setup (hook 0 of the
 * sub-flow table).  Both master brightnesses go black (-16), both DISPCNTs lose their BG bits
 * (8..12), the sub engine goes to graphics mode 0 and the ov002 display to the top screen
 * (02076028); the BG priorities become main BG0 3, BG1 0, BG2 1, BG3 2 and sub BG0 0, BG1 1,
 * BG2 2, BG3 3. */

#include "nitro/types.h"
#include "game/engine.h"

static volatile u32 *const REG_DISPCNT = (volatile u32 *)((unsigned int)kh_ds_io + 0x0);
static volatile u16 *const REG_BG0CNT = (volatile u16 *)((unsigned int)kh_ds_io + 0x8);
static volatile u16 *const REG_BG1CNT = (volatile u16 *)((unsigned int)kh_ds_io + 0xa);
static volatile u16 *const REG_BG2CNT = (volatile u16 *)((unsigned int)kh_ds_io + 0xc);
static volatile u16 *const REG_BG3CNT = (volatile u16 *)((unsigned int)kh_ds_io + 0xe);
static volatile u32 *const REG_DB_DISPCNT = (volatile u32 *)((unsigned int)kh_ds_io + 0x1000);
static volatile u16 *const REG_DB_BG0CNT = (volatile u16 *)((unsigned int)kh_ds_io + 0x1008);
static volatile u16 *const REG_DB_BG1CNT = (volatile u16 *)((unsigned int)kh_ds_io + 0x100a);
static volatile u16 *const REG_DB_BG2CNT = (volatile u16 *)((unsigned int)kh_ds_io + 0x100c);
static volatile u16 *const REG_DB_BG3CNT = (volatile u16 *)((unsigned int)kh_ds_io + 0x100e);

extern void  GXS_SetGraphicsMode(int nMode);
extern void  Ov002_SetDisplaySwap(int bTop);                         /* ov002: put the display on the top screen */

void Ov025_SetupDisplay(void)
{
    SetMasterBrightnessMain(-16);
    SetMasterBrightnessSub(-16);
    *REG_DISPCNT = *REG_DISPCNT & 0xffffe0ff;
    *REG_DB_DISPCNT = *REG_DB_DISPCNT & 0xffffe0ff;
    GXS_SetGraphicsMode(0);
    Ov002_SetDisplaySwap(1);
    *REG_BG0CNT = (*REG_BG0CNT & ~3) | 3;
    *REG_BG1CNT = *REG_BG1CNT & ~3;
    *REG_BG2CNT = (*REG_BG2CNT & ~3) | 1;
    *REG_BG3CNT = (*REG_BG3CNT & ~3) | 2;
    *REG_DB_BG0CNT = *REG_DB_BG0CNT & ~3;
    *REG_DB_BG1CNT = (*REG_DB_BG1CNT & ~3) | 1;
    *REG_DB_BG2CNT = (*REG_DB_BG2CNT & ~3) | 2;
    *REG_DB_BG3CNT = (*REG_DB_BG3CNT & ~3) | 3;
}
