/* PS2: mechanically prepared copy of libs/nitro/os/calls/OS_GetLowEntropyData.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os.h"

#define NVRAM_CONFIG_SIZE  0x74      /* sizeof(NVRAMConfig) in this SDK version */
#define reg_GX_VCOUNT      (*(vu16 *)((unsigned int)kh_ds_io + 0x6))
#define reg_G3X_GXSTAT     (*(vu32 *)((unsigned int)kh_ds_io + 0x600))
#define reg_PAD_KEYINPUT   (*(vu16 *)((unsigned int)kh_ds_io + 0x130))
static inline u16 GX_GetVCount(void) { return reg_GX_VCOUNT; }

extern volatile u64 data_0204466c;   /* OSi_TickCounter */
#define OSi_TickCounter data_0204466c
extern u16 OS_GetTickLo(void);

/* OS_GetLowEntropyData -- NitroSDK os_entropy.c: fill 8 words of low-quality entropy
 * from the vcount, the tick counter, the MAC address, the vblank count, the 3D status,
 * the RTC, the microphone, the touch panel and the buttons. */
void OS_GetLowEntropyData(u32 buffer[OS_LOW_ENTROPY_DATA_SIZE / sizeof(u32)])
{
    const OSSystemWork *work = OS_GetSystemWork();
    const u8 *macAddress = (u8 *)((u32)(work->nvramUserInfo) + ((NVRAM_CONFIG_SIZE + 3) & ~0x00000003));

    buffer[0] = (u32)((GX_GetVCount() << 16) | OS_GetTickLo());
    buffer[1] = (u32)(*(u16 *)(macAddress + 4) << 16) ^ (u32)(OSi_TickCounter);
    buffer[2] = (u32)(OSi_TickCounter >> 32) ^ *(u32 *)macAddress ^ work->vblankCount;
    buffer[2] ^= kh_ge_port_read(0x600);
    buffer[3] = *(u32 *)(&work->real_time_clock[0]);
    buffer[4] = *(u32 *)(&work->real_time_clock[4]);
    buffer[5] = (((u32)work->mic_sampling_data) << 16) ^ work->mic_last_address;
    buffer[6] = (u32)((*(u16 *)(&work->touch_panel[0]) << 16) | *(u16 *)(&work->touch_panel[2]));
    buffer[7] = (u32)((work->wm_rssi_pool << 16) | (reg_PAD_KEYINPUT | *(vu16 *)HW_BUTTON_XY_BUF));
}
