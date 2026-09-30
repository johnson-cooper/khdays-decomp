/* PS2: mechanically prepared copy of libs/nitro/gx/calls/GX_SetGraphicsMode.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/gx.h"

typedef void *OSMessage;

#define HW_MAIN_MEM 0x02000000

#define offsetof(type, member) ((u32)&(((type *)0)->member))

#define HW_IOREG ((unsigned int)kh_ds_io + 0x0)
#define HW_REG_BASE HW_IOREG        // alias
#define REG_DISPCNT_OFFSET 0x000
#define REG_DISPCNT_ADDR (HW_REG_BASE + REG_DISPCNT_OFFSET)
#define reg_GX_DISPCNT (*( REGType32v *) REG_DISPCNT_ADDR)
#define REG_GX_DISPCNT_VRAM_MASK 0x000c0000
#define REG_GX_DISPCNT_MODE_SHIFT 16
#define REG_GX_DISPCNT_MODE_MASK 0x00030000
#define REG_GX_DISPCNT_BG02D3D_SHIFT 3
#define REG_GX_DISPCNT_BG02D3D_MASK 0x00000008
#define REG_GX_DISPCNT_BGMODE_SHIFT 0
#define REG_GX_DISPCNT_BGMODE_MASK 0x00000007

extern u16 data_020446d0;
extern u16 data_020422b4;

/* GX_SetGraphicsMode -- NitroSDK gx.c: GX_SetGraphicsMode. */
void GX_SetGraphicsMode (GXDispMode dispMode, GXBGMode bgMode, GXBG0As bg0_2d3d)
{
	u32 cnt = reg_GX_DISPCNT;

	data_020446d0 = (u16)dispMode;
	if (!data_020422b4) {
		dispMode = GX_DISPMODE_OFF;
	}

	cnt &= ~(REG_GX_DISPCNT_BGMODE_MASK |
	         REG_GX_DISPCNT_BG02D3D_MASK | REG_GX_DISPCNT_MODE_MASK | REG_GX_DISPCNT_VRAM_MASK);

	reg_GX_DISPCNT = (u32)(cnt |
	                       (dispMode << REG_GX_DISPCNT_MODE_SHIFT) |
	                       (bgMode << REG_GX_DISPCNT_BGMODE_SHIFT) | (bg0_2d3d <<
	                                                                  REG_GX_DISPCNT_BG02D3D_SHIFT));

	if (data_020446d0 == GX_DISPMODE_OFF) {
		data_020422b4 = FALSE;
	}
}
