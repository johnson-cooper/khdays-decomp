/* PS2: mechanically prepared copy of libs/nitro/gx/calls/GX_Init.c (ps2/tools/prep_sources.py). Do not edit. */
/* GX_Init: power the 2D/3D engines on, reset the software GX state, claim the
 * VRAM spin-lock id, blank both engines' display registers and reset both
 * background affine matrices to identity.
 *
 * Algorithm cross-checked against a matched C version of the NitroSDK's GX.c
 * (GX_Init); verified byte-exact against this ROM.
 */

#include "nitro/types.h"
#include "nitro/os.h"

#define reg_GX_POWCNT        (*(vu16 *)((unsigned int)kh_ds_io + 0x304))
#define reg_GX_DISPCNT       (*(vu32 *)((unsigned int)kh_ds_io + 0x0))
#define reg_GX_DISPSTAT      (*(vu16 *)((unsigned int)kh_ds_io + 0x4))
#define reg_GX_MASTER_BRIGHT (*(vu16 *)((unsigned int)kh_ds_io + 0x6c))
#define reg_G2_BG0CNT        ((void *)((unsigned int)kh_ds_io + 0x8))
#define reg_GXS_DB_DISPCNT   ((void *)((unsigned int)kh_ds_io + 0x1000))

/* Enable 2D engine A, its 2D graphics, the rendering engine and the geometry
 * engine in one go. */
#define POWCNT_INIT_MASK 0x20e
#define POWCNT_DSEL      0x8000
#define POWCNT_LCD       0x0001

extern void GX_InitGXState(void);
extern s32 OS_GetLockID(void);
extern void OS_Terminate(void);
extern void MI_DmaFill32(u32 dmaNo, void *dest, u32 data, u32 size);
extern void INITi_CpuClear32_0x01ff86fc(u32 data, void *dest, u32 size);

/* [0] is the saved display mode (see GX_DispOff/DispCnt_ApplyPendingMode),
   [1] is the VRAM spin-lock id. */
extern vu16 data_020446d0[];
/* [0] is the display-on flag (see GX_SetGraphicsMode), [1] is the DMA channel GX
   uses for its own fills, or -1 for "no DMA". */
extern u32 data_020422b4[];

void GX_Init(void)
{
    s32 lockId;

    reg_GX_POWCNT |= POWCNT_DSEL;
    reg_GX_POWCNT = (u16)((reg_GX_POWCNT & ~POWCNT_INIT_MASK) | POWCNT_INIT_MASK);
    reg_GX_POWCNT = (u16)(reg_GX_POWCNT | POWCNT_LCD);

    GX_InitGXState();

    while (data_020446d0[1] == 0) {
        lockId = OS_GetLockID();
        if (lockId == OS_LOCK_ID_ERROR) {
            OS_Terminate();
        }
        data_020446d0[1] = (u16)lockId;
    }

    reg_GX_DISPSTAT = 0;
    reg_GX_DISPCNT = 0;

    if (data_020422b4[1] != (u32)-1) {
        MI_DmaFill32(data_020422b4[1], reg_G2_BG0CNT, 0, 0x60);
        reg_GX_MASTER_BRIGHT = 0;
        MI_DmaFill32(data_020422b4[1], reg_GXS_DB_DISPCNT, 0, 0x70);
    } else {
        INITi_CpuClear32_0x01ff86fc(0, reg_G2_BG0CNT, 0x60);
        reg_GX_MASTER_BRIGHT = 0;
        INITi_CpuClear32_0x01ff86fc(0, reg_GXS_DB_DISPCNT, 0x70);
    }

    *(vu16 *)((unsigned int)kh_ds_io + 0x20) = 0x100; /* BG2PA */
    *(vu16 *)((unsigned int)kh_ds_io + 0x26) = 0x100; /* BG2PD */
    *(vu16 *)((unsigned int)kh_ds_io + 0x30) = 0x100; /* BG3PA */
    *(vu16 *)((unsigned int)kh_ds_io + 0x36) = 0x100; /* BG3PD */
    *(vu16 *)((unsigned int)kh_ds_io + 0x1020) = 0x100; /* sub BG2PA */
    *(vu16 *)((unsigned int)kh_ds_io + 0x1026) = 0x100; /* sub BG2PD */
    *(vu16 *)((unsigned int)kh_ds_io + 0x1030) = 0x100; /* sub BG3PA */
    *(vu16 *)((unsigned int)kh_ds_io + 0x1036) = 0x100; /* sub BG3PD */
}
