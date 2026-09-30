/* PS2: mechanically prepared copy of libs/nitro/gx/calls/GX_SetBankForOBJ.c (ps2/tools/prep_sources.py). Do not edit. */


/* NitroSDK gx_vramcnt.c: VRAM bank control (gx_vramcnt.h enums and the bank register values). */

#include "nitro/types.h"
#include "nitro/os.h"
#include "nitro/gx.h"

#ifdef SDK_CW_WARNOFF_SAFESTRB
    #include <nitro/code32.h>
#endif

#define reg_GX_DISPCNT      (*(REGType32v *)((unsigned int)kh_ds_io + 0x0))
#define reg_G3X_DISP3DCNT   (*(REGType16v *)((unsigned int)kh_ds_io + 0x60))
#define reg_GXS_DB_DISPCNT  (*(REGType32v *)((unsigned int)kh_ds_io + 0x1000))
#define reg_GX_VRAMCNT_A    (*(REGType8v *)((unsigned int)kh_ds_io + 0x240))
#define reg_GX_VRAMCNT_B    (*(REGType8v *)((unsigned int)kh_ds_io + 0x241))
#define reg_GX_VRAMCNT_C    (*(REGType8v *)((unsigned int)kh_ds_io + 0x242))
#define reg_GX_VRAMCNT_D    (*(REGType8v *)((unsigned int)kh_ds_io + 0x243))
#define reg_GX_VRAMCNT_E    (*(REGType8v *)((unsigned int)kh_ds_io + 0x244))
#define reg_GX_VRAMCNT_F    (*(REGType8v *)((unsigned int)kh_ds_io + 0x245))
#define reg_GX_VRAMCNT_G    (*(REGType8v *)((unsigned int)kh_ds_io + 0x246))
#define reg_GX_VRAMCNT_H    (*(REGType8v *)((unsigned int)kh_ds_io + 0x248))
#define reg_GX_VRAMCNT_I    (*(REGType8v *)((unsigned int)kh_ds_io + 0x249))

extern GX_State data_020446d4;   /* gGXState */
#define gGXState data_020446d4
extern void GX_VRAMCNT_SetLCDC_(int lcdc);

static inline void GX_VRAMCNT_SetBG_ (GXVRamBG bg)
{
	switch (bg) {
	case GX_VRAM_BG_128_D:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_BG_0x06000000;
		break;
	case GX_VRAM_BG_256_CD:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_BG_0x06020000;
	case GX_VRAM_BG_128_C:
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_BG_0x06000000;
		break;
	case GX_VRAM_BG_384_BCD:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_BG_0x06040000;
	case GX_VRAM_BG_256_BC:
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_BG_0x06020000;
	case GX_VRAM_BG_128_B:
		reg_GX_VRAMCNT_B = (u8)GX_VRAMCNT_B_BG_0x06000000;
		break;
	case GX_VRAM_BG_512_ABCD:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_BG_0x06060000;
	case GX_VRAM_BG_384_ABC:
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_BG_0x06040000;
	case GX_VRAM_BG_256_AB:
		reg_GX_VRAMCNT_B = (u8)GX_VRAMCNT_B_BG_0x06020000;
	case GX_VRAM_BG_128_A:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_BG_0x06000000;
	case GX_VRAM_BG_NONE:
		break;
	case GX_VRAM_BG_384_ABD:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_BG_0x06000000;
		reg_GX_VRAMCNT_B = (u8)GX_VRAMCNT_B_BG_0x06020000;
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_BG_0x06040000;
		break;
	case GX_VRAM_BG_384_ACD:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_BG_0x06040000;
	case GX_VRAM_BG_256_AC:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_BG_0x06000000;
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_BG_0x06020000;
		break;
	case GX_VRAM_BG_256_AD:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_BG_0x06000000;
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_BG_0x06020000;
		break;
	case GX_VRAM_BG_256_BD:
		reg_GX_VRAMCNT_B = (u8)GX_VRAMCNT_B_BG_0x06000000;
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_BG_0x06020000;
		break;
	case GX_VRAM_BG_96_EFG:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_BG_0x06014000;
	case GX_VRAM_BG_80_EF:
		reg_GX_VRAMCNT_F = (u8)GX_VRAMCNT_F_BG_0x06010000;
	case GX_VRAM_BG_64_E:
		reg_GX_VRAMCNT_E = (u8)GX_VRAMCNT_E_BG_0x06000000;
		break;
	case GX_VRAM_BG_80_EG:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_BG_0x06010000;
		reg_GX_VRAMCNT_E = (u8)GX_VRAMCNT_E_BG_0x06000000;
		break;
	case GX_VRAM_BG_32_FG:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_BG_0x06004000;
	case GX_VRAM_BG_16_F:
		reg_GX_VRAMCNT_F = (u8)GX_VRAMCNT_F_BG_0x06000000;
		break;
	case GX_VRAM_BG_16_G:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_BG_0x06000000;
		break;
	default:
		break;
	}
}

static inline void GX_VRAMCNT_SetBGEx1_ (GXVRamBG bg)
{
	switch (bg) {
	case GX_VRAM_BG_96_EFG:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_BG_0x06014000;
	case GX_VRAM_BG_80_EF:
		reg_GX_VRAMCNT_F = (u8)GX_VRAMCNT_F_BG_0x06010000;
	case GX_VRAM_BG_64_E:
		reg_GX_VRAMCNT_E = (u8)GX_VRAMCNT_E_BG_0x06000000;
		break;
	case GX_VRAM_BG_80_EG:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_BG_0x06010000;
		reg_GX_VRAMCNT_E = (u8)GX_VRAMCNT_E_BG_0x06000000;
		break;
	case GX_VRAM_BG_32_FG:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_BG_0x06004000;
	case GX_VRAM_BG_16_F:
		reg_GX_VRAMCNT_F = (u8)GX_VRAMCNT_F_BG_0x06000000;
		break;
	case GX_VRAM_BG_16_G:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_BG_0x06000000;
		break;
	default:
		break;
	}
}

static inline void GX_VRAMCNT_SetBGEx2_ (GXVRamBG bg)
{
	switch (bg) {
	case GX_VRAM_BG_128_D:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_BG_0x06020000;
		break;
	case GX_VRAM_BG_256_CD:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_BG_0x06040000;
	case GX_VRAM_BG_128_C:
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_BG_0x06020000;
		break;
	case GX_VRAM_BG_384_BCD:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_BG_0x06060000;
	case GX_VRAM_BG_256_BC:
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_BG_0x06040000;
	case GX_VRAM_BG_128_B:
		reg_GX_VRAMCNT_B = (u8)GX_VRAMCNT_B_BG_0x06020000;
		break;
	case GX_VRAM_BG_384_ABC:
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_BG_0x06060000;
	case GX_VRAM_BG_256_AB:
		reg_GX_VRAMCNT_B = (u8)GX_VRAMCNT_B_BG_0x06040000;
	case GX_VRAM_BG_128_A:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_BG_0x06020000;
	case GX_VRAM_BG_NONE:
		break;
	case GX_VRAM_BG_384_ABD:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_BG_0x06020000;
		reg_GX_VRAMCNT_B = (u8)GX_VRAMCNT_B_BG_0x06040000;
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_BG_0x06060000;
		break;
	case GX_VRAM_BG_384_ACD:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_BG_0x06060000;
	case GX_VRAM_BG_256_AC:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_BG_0x06020000;
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_BG_0x06040000;
		break;
	case GX_VRAM_BG_256_AD:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_BG_0x06020000;
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_BG_0x06040000;
		break;
	case GX_VRAM_BG_256_BD:
		reg_GX_VRAMCNT_B = (u8)GX_VRAMCNT_B_BG_0x06020000;
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_BG_0x06040000;
		break;
	default:
		break;
	}
}

static inline void GX_VRAMCNT_SetOBJ_ (GXVRamOBJ obj)
{
	switch (obj) {
	case GX_VRAM_OBJ_256_AB:
		reg_GX_VRAMCNT_B = (u8)GX_VRAMCNT_B_OBJ_0x06420000;
	case GX_VRAM_OBJ_128_A:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_OBJ_0x06400000;
	case GX_VRAM_OBJ_NONE:
		break;
	case GX_VRAM_OBJ_128_B:
		reg_GX_VRAMCNT_B = (u8)GX_VRAMCNT_B_OBJ_0x06400000;
		break;
	case GX_VRAM_OBJ_96_EFG:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_OBJ_0x06414000;
	case GX_VRAM_OBJ_80_EF:
		reg_GX_VRAMCNT_F = (u8)GX_VRAMCNT_F_OBJ_0x06410000;
	case GX_VRAM_OBJ_64_E:
		reg_GX_VRAMCNT_E = (u8)GX_VRAMCNT_E_OBJ_0x06400000;
		break;
	case GX_VRAM_OBJ_80_EG:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_OBJ_0x06410000;
		reg_GX_VRAMCNT_E = (u8)GX_VRAMCNT_E_OBJ_0x06400000;
		break;
	case GX_VRAM_OBJ_32_FG:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_OBJ_0x06404000;
	case GX_VRAM_OBJ_16_F:
		reg_GX_VRAMCNT_F = (u8)GX_VRAMCNT_F_OBJ_0x06400000;
		break;
	case GX_VRAM_OBJ_16_G:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_OBJ_0x06400000;
		break;
	default:
		break;
	}
}

static inline void GX_VRAMCNT_SetARM7_ (GXVRamARM7 arm7)
{
	switch (arm7) {
	case GX_VRAM_ARM7_256_CD:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_ARM7_0x06020000;
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_ARM7_0x06000000;
		break;
	case GX_VRAM_ARM7_128_C:
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_ARM7_0x06000000;
		break;
	case GX_VRAM_ARM7_128_D:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_ARM7_0x06000000;
	case GX_VRAM_ARM7_NONE:
		break;
	default:
		break;
	}
}

static inline void texOn_ (void)
{
	reg_G3X_DISP3DCNT = (u16)((reg_G3X_DISP3DCNT &
	                           ~(REG_G3X_DISP3DCNT_RO_MASK | REG_G3X_DISP3DCNT_GO_MASK)) |
	                          REG_G3X_DISP3DCNT_TME_MASK);
}

static inline void texOff_ (void)
{
	reg_G3X_DISP3DCNT &= (u16) ~(REG_G3X_DISP3DCNT_TME_MASK |
	                             REG_G3X_DISP3DCNT_RO_MASK | REG_G3X_DISP3DCNT_GO_MASK);
}

static inline void GX_VRAMCNT_SetTEX_ (GXVRamTex tex)
{
	if (tex == GX_VRAM_TEX_NONE) {
		texOff_();
		return;
	}

	texOn_();

	switch (tex) {
	case GX_VRAM_TEX_01_AC:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_TEX_0;
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_TEX_1;
		break;
	case GX_VRAM_TEX_01_AD:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_TEX_0;
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_TEX_1;
		break;
	case GX_VRAM_TEX_01_BD:
		reg_GX_VRAMCNT_B = (u8)GX_VRAMCNT_B_TEX_0;
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_TEX_1;
		break;
	case GX_VRAM_TEX_012_ABD:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_TEX_0;
		reg_GX_VRAMCNT_B = (u8)GX_VRAMCNT_B_TEX_1;
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_TEX_2;
		break;
	case GX_VRAM_TEX_012_ACD:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_TEX_0;
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_TEX_1;
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_TEX_2;
		break;
	case GX_VRAM_TEX_0_D:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_TEX_0;
		break;
	case GX_VRAM_TEX_01_CD:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_TEX_1;
	case GX_VRAM_TEX_0_C:
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_TEX_0;
		break;
	case GX_VRAM_TEX_012_BCD:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_TEX_2;
	case GX_VRAM_TEX_01_BC:
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_TEX_1;
	case GX_VRAM_TEX_0_B:
		reg_GX_VRAMCNT_B = (u8)GX_VRAMCNT_B_TEX_0;
		break;
	case GX_VRAM_TEX_0123_ABCD:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_TEX_3;
	case GX_VRAM_TEX_012_ABC:
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_TEX_2;
	case GX_VRAM_TEX_01_AB:
		reg_GX_VRAMCNT_B = (u8)GX_VRAMCNT_B_TEX_1;
	case GX_VRAM_TEX_0_A:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_TEX_0;
		break;
	default:
		break;
	}
}

static inline void clearImageOn_ (void)
{
	reg_G3X_DISP3DCNT |= REG_G3X_DISP3DCNT_PRI_MASK;
}

static inline void clearImageOff_ (void)
{
	reg_G3X_DISP3DCNT &= ~REG_G3X_DISP3DCNT_PRI_MASK;
}

static inline void GX_VRAMCNT_SetCLRIMG_ (GXVRamClearImage clrImg)
{
	switch (clrImg) {
	case GX_VRAM_CLEARIMAGE_256_AB:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_TEX_2;
	case GX_VRAM_CLEARDEPTH_128_B:
		reg_GX_VRAMCNT_B = (u8)GX_VRAMCNT_B_TEX_3;
		clearImageOn_();
		break;
	case GX_VRAM_CLEARIMAGE_256_CD:
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_TEX_2;
	case GX_VRAM_CLEARDEPTH_128_D:
		reg_GX_VRAMCNT_D = (u8)GX_VRAMCNT_D_TEX_3;
		clearImageOn_();
		break;
	case GX_VRAM_CLEARIMAGE_NONE:
		clearImageOff_();
		break;
	case GX_VRAM_CLEARDEPTH_128_A:
		reg_GX_VRAMCNT_A = (u8)GX_VRAMCNT_A_TEX_3;
		clearImageOn_();
		break;
	case GX_VRAM_CLEARDEPTH_128_C:
		reg_GX_VRAMCNT_C = (u8)GX_VRAMCNT_C_TEX_3;
		clearImageOn_();
		break;
	default:
		break;
	}
}

static inline void GX_VRAMCNT_SetTEXPLTT_ (GXVRamTexPltt texPltt)
{
	switch (texPltt) {
	case GX_VRAM_TEXPLTT_01_FG:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_TEXPLTT_1;
	case GX_VRAM_TEXPLTT_0_F:
		reg_GX_VRAMCNT_F = (u8)GX_VRAMCNT_F_TEXPLTT_0;
		break;
	case GX_VRAM_TEXPLTT_0_G:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_TEXPLTT_0;
		break;
	case GX_VRAM_TEXPLTT_012345_EFG:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_TEXPLTT_5;
	case GX_VRAM_TEXPLTT_01234_EF:
		reg_GX_VRAMCNT_F = (u8)GX_VRAMCNT_F_TEXPLTT_4;
	case GX_VRAM_TEXPLTT_0123_E:
		reg_GX_VRAMCNT_E = (u8)GX_VRAMCNT_E_TEXPLTT_0123;
		break;
	case GX_VRAM_TEXPLTT_NONE:
		break;
	default:
		break;
	}
}

static inline void bgExtPlttOn_ (void)
{
	reg_GX_DISPCNT |= REG_GX_DISPCNT_BG_MASK;
}

static inline void bgExtPlttOff_ (void)
{
	reg_GX_DISPCNT &= ~REG_GX_DISPCNT_BG_MASK;
}

static inline void GX_VRAMCNT_SetBGEXTPLTT_ (GXVRamBGExtPltt bgExtPltt)
{
	switch (bgExtPltt) {
	case GX_VRAM_BGEXTPLTT_0123_E:
		bgExtPlttOn_();
		reg_GX_VRAMCNT_E = (u8)GX_VRAMCNT_E_BGEXTPLTT_0123;
		break;
	case GX_VRAM_BGEXTPLTT_23_G:
		bgExtPlttOn_();
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_BGEXTPLTT_23;
		break;
	case GX_VRAM_BGEXTPLTT_0123_FG:
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_BGEXTPLTT_23;
	case GX_VRAM_BGEXTPLTT_01_F:
		reg_GX_VRAMCNT_F = (u8)GX_VRAMCNT_F_BGEXTPLTT_01;
		bgExtPlttOn_();
		break;
	case GX_VRAM_BGEXTPLTT_NONE:
		bgExtPlttOff_();
		break;
	default:
		break;
	}
}

static inline void objExtPlttOn_ (void)
{
	reg_GX_DISPCNT |= REG_GX_DISPCNT_O_MASK;
}

static inline void objExtPlttOff_ (void)
{
	reg_GX_DISPCNT &= ~REG_GX_DISPCNT_O_MASK;
}

static inline void GX_VRAMCNT_SetOBJEXTPLTT_ (GXVRamOBJExtPltt objExtPltt)
{
	switch (objExtPltt) {
	case GX_VRAM_OBJEXTPLTT_0_F:
		objExtPlttOn_();
		reg_GX_VRAMCNT_F = (u8)GX_VRAMCNT_F_OBJEXTPLTT;
		break;
	case GX_VRAM_OBJEXTPLTT_0_G:
		objExtPlttOn_();
		reg_GX_VRAMCNT_G = (u8)GX_VRAMCNT_G_OBJEXTPLTT;
		break;
	case GX_VRAM_OBJEXTPLTT_NONE:
		objExtPlttOff_();
		break;
	default:
		break;
	}
}

static inline void GX_VRAMCNT_SetSubBG_ (GXVRamSubBG bg)
{
	switch (bg) {
	case GX_VRAM_SUB_BG_128_C:
		reg_GX_VRAMCNT_C = GX_VRAMCNT_C_SUBBG_0x06200000;
		break;
	case GX_VRAM_SUB_BG_48_HI:
		reg_GX_VRAMCNT_I = GX_VRAMCNT_I_SUBBG_0x06208000;
	case GX_VRAM_SUB_BG_32_H:
		reg_GX_VRAMCNT_H = GX_VRAMCNT_H_SUBBG_0x06200000;
		break;
	case GX_VRAM_SUB_BG_NONE:
		break;
	default:
		break;
	}
}

static inline void GX_VRAMCNT_SetSubOBJ_ (GXVRamSubOBJ obj)
{
	switch (obj) {
	case GX_VRAM_SUB_OBJ_128_D:
		reg_GX_VRAMCNT_D = GX_VRAMCNT_D_SUBOBJ_0x06600000;
		break;
	case GX_VRAM_SUB_OBJ_16_I:
		reg_GX_VRAMCNT_I = GX_VRAMCNT_I_SUBOBJ_0x06600000;
		break;
	case GX_VRAM_SUB_OBJ_NONE:
		break;
	default:
		break;
	}
}

static inline void subBGExtPlttOn_ (void)
{
	reg_GXS_DB_DISPCNT |= REG_GXS_DB_DISPCNT_BG_MASK;
}

static inline void subBGExtPlttOff_ (void)
{
	reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_BG_MASK;
}

static inline void GX_VRAMCNT_SetSubBGExtPltt_ (GXVRamSubBGExtPltt bgExtPltt)
{
	switch (bgExtPltt) {
	case GX_VRAM_SUB_BGEXTPLTT_0123_H:
		subBGExtPlttOn_();
		reg_GX_VRAMCNT_H = GX_VRAMCNT_H_SUBBGEXTPLTT_0123;
		break;
	case GX_VRAM_SUB_BGEXTPLTT_NONE:
		subBGExtPlttOff_();
		break;
	default:
		break;
	}
}

static inline void subOBJExtPlttOn_ (void)
{
	reg_GXS_DB_DISPCNT |= REG_GXS_DB_DISPCNT_O_MASK;
}

static inline void subOBJExtPlttOff_ (void)
{
	reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_O_MASK;
}

static inline void GX_VRAMCNT_SetSubOBJExtPltt_ (GXVRamSubOBJExtPltt objExtPltt)
{
	switch (objExtPltt) {
	case GX_VRAM_SUB_OBJEXTPLTT_0_I:
		subOBJExtPlttOn_();
		reg_GX_VRAMCNT_I = GX_VRAMCNT_I_SUBOBJEXTPLTT;
		break;
	case GX_VRAM_SUB_OBJEXTPLTT_NONE:
		subOBJExtPlttOff_();
		break;
	}
}

/* GX_SetBankForOBJ -- NitroSDK gx_vramcnt.c: map the banks to this use, the rest back to LCDC. */
static inline void GxSetBankForOBJ (GXVRamOBJ obj)
{

	gGXState.vramCnt.lcdc = (u16)(~obj & (gGXState.vramCnt.lcdc | gGXState.vramCnt.obj));
	gGXState.vramCnt.obj = obj;

	GX_StateCheck_VRAMCnt();

	GX_VRAMCNT_SetOBJ_(obj);
	GX_VRAMCNT_SetLCDC_(gGXState.vramCnt.lcdc);
}

void GX_SetBankForOBJ (GXVRamOBJ obj)
{
	GxSetBankForOBJ(obj);
}
