/* PS2: mechanically prepared copy of libs/nitro/spi/calls/PM_ForceToPowerOffAsync.c (ps2/tools/prep_sources.py). Do not edit. */

#include "nitro/types.h"
#include "nitro/os_types.h"

typedef void *OSMessage;

#define HW_MAIN_MEM 0x02000000

/* NitroSDK SPI library, power manager (pm.c): the ARM9 side of the PMIC PXI protocol. */
typedef int PXIFifoTag;
typedef void (*PMCallback)(u32 result, void *arg);
#define PMIC_REG_NUMS 5
#define PM_RESULT_SUCCESS 0
#define PM_RESULT_ERROR 2
#define SPI_PXI_RESULT_COMMAND_MASK         0x00007f00
#define SPI_PXI_RESULT_COMMAND_SHIFT        8
#define SPI_PXI_RESULT_DATA_MASK            0x000000ff
#define SPI_PXI_RESULT_DATA_SHIFT           0
#define SPI_PXI_COMMAND_PM_SYNC             0x0060
#define SPI_PXI_COMMAND_PM_SLEEP_END        0x0062
#define SPI_PXI_COMMAND_PM_GET_BLINK        0x0067
#define SPI_PXI_COMMAND_PM_REG0VALUE        0x0070
#define SPI_PXI_COMMAND_PM_REG4VALUE        0x0074

typedef struct {
    BOOL lock;                    /* 0x00 */
    PMCallback callback;          /* 0x04 */
    void *callbackArg;            /* 0x08 */
    void *work;                   /* 0x0c */
} PMiWork;
/* This SDK's register reply slot: the flag first, then the buffer pointer. */
typedef struct {
    u16 flag;                     /* 0x00 */
    u16 pad;
    u16 *buffer;                  /* 0x04 */
} PMData16;

/* pm.c statics, one .bss block at data_020463cc. */
extern struct {
    u32 reserved0;                /* 0x00 */
    volatile BOOL syncFlag;       /* 0x04: PMi_SyncFlag */
    volatile BOOL sleepEndFlag;   /* 0x08: PMi_SleepEndFlag */
    u32 reserved0c[4];            /* 0x0c */
    PMiWork work;                 /* 0x1c: PMi_Work */
} data_020463cc;
#define PMi_SyncFlag data_020463cc.syncFlag
#define PMi_SleepEndFlag data_020463cc.sleepEndFlag
#define PMi_Work data_020463cc.work
extern PMData16 data_02046410[PMIC_REG_NUMS];   /* PMi_RegisterBuffer */
#define PMi_RegisterBuffer data_02046410
extern void PMi_CallCallbackAndUnlock(u32 result);   /* PMi_CallCallbackAndUnlock */
/* PM_GoSleepMode's world. */
typedef u32 OSIrqMask;
typedef u32 PMWakeUpTrigger;
typedef u32 PMLogic;
typedef int PMBackLightSwitch;
typedef int PMLCDPower;
typedef int PMLEDStatus;
typedef int PMLCDTarget;
typedef struct PMSleepCallbackInfo PMSleepCallbackInfo;
#define PM_SUCCESS 0
#define PM_TRIGGER_CARD             (1 << 3)
#define PM_TRIGGER_CARTRIDGE        (1 << 4)
#define PM_BACKLIGHT_RECOVER_TOP_SHIFT      5
#define PM_BACKLIGHT_RECOVER_BOTTOM_SHIFT   6
#define PM_LCD_ALL 2
#define PM_BACKLIGHT_OFF 0
#define PM_LCD_POWER_ON 1
#define PM_LED_ON 1
#define OS_IRQ_TABLE_MAX 22
#define OS_IE_TIMER0 (1UL << 3)
#define OS_IE_FIFO_RECV (1UL << 18)
#define OS_IE_CARD_IREQ (1UL << 20)
#define REG_IME_ADDR ((unsigned int)kh_ds_io + 0x208)
#define REG_IF_ADDR ((unsigned int)kh_ds_io + 0x214)
#define reg_OS_IME (*(vu16 *)REG_IME_ADDR)
#define reg_OS_IF (*(vu32 *)REG_IF_ADDR)
#define OS_IME_ENABLE 1
#define OS_IME_DISABLE 0
#define REG_DISPCNT_ADDR ((unsigned int)kh_ds_io + 0x0)
#define REG_DB_DISPCNT_ADDR ((unsigned int)kh_ds_io + 0x1000)
#define reg_GX_DISPCNT (*(vu32 *)REG_DISPCNT_ADDR)
#define reg_GXS_DB_DISPCNT (*(vu32 *)REG_DB_DISPCNT_ADDR)
#define REG_GX_DISPCNT_MODE_MASK 0x00030000
#define REG_GXS_DB_DISPCNT_MODE_MASK 0x00010000
#define HW_WM_BOOT_BUF 0x027ffc40
#define MB_TYPE_MULTIBOOT 2
#define HW_VBLANK_COUNT_BUF 0x027ffc3c
#define HW_SYSTEM_CLOCK 33513982
#define OS_MilliSecondsToTicks(msec) ((msec) * (HW_SYSTEM_CLOCK / 64) / 1000)
#define PMi_LCD_SLEEP_WAIT_MSEC  110
#define PMi_LCD_SLEEP_WAIT_TICK  (OS_MilliSecondsToTicks(PMi_LCD_SLEEP_WAIT_MSEC) * (64 * 2))
/* The sleep callback lists sit in the pm.c block too (+0xc pre, +0x18 post). */
#define PMi_PreSleepCallbackList (*(PMSleepCallbackInfo **)&data_020463cc.reserved0c[0])
#define PMi_PostSleepCallbackList (*(PMSleepCallbackInfo **)&data_020463cc.reserved0c[3])

extern OSIntrMode OS_DisableInterrupts(void);
extern OSIntrMode OS_RestoreInterrupts(OSIntrMode state);
extern OSIrqMask OS_DisableIrqMask(OSIrqMask intr);
extern OSIrqMask OS_SetIrqMask(OSIrqMask intr);
extern BOOL OS_IsTickAvailable(void);
extern void OS_Halt(void);
extern void PM_ForceToPowerOff(void);
extern void OS_SpinWait(u32 cycles);   /* OS_SpinWait */
extern BOOL CTRDG_IsExisting(void);   /* CTRDG_IsExisting */
extern void PMi_ExecuteList(PMSleepCallbackInfo *listp);   /* PMi_ExecuteList */
extern PMLCDPower PM_GetLCDPower(void);   /* PM_GetLCDPower */
extern u32 PM_GetBackLight(PMBackLightSwitch *top, PMBackLightSwitch *bottom);   /* PM_GetBackLight */
extern u32 PM_SetBackLight(PMLCDTarget target, PMBackLightSwitch sw);   /* PM_SetBackLight */
extern u32 PMi_SendSleepStart(unsigned int trigger, unsigned int keyIntrData);   /* PMi_SendSleepStart */
extern BOOL PMi_SetLCDPower(PMLCDPower sw, PMLEDStatus led, BOOL skip, BOOL isSync);   /* PMi_SetLCDPower */
extern u32 PMi_SetLED(PMLEDStatus status);   /* PMi_SetLED */

#define PM_LCD_TOP 0
#define PM_LCD_BOTTOM 1
#define PM_LCD_POWER_OFF 0
#define PMi_LCD_POWER_WAIT_MSEC  150
#define PMi_LCD_POWER_WAIT_TICK  (OS_MilliSecondsToTicks(PMi_LCD_POWER_WAIT_MSEC) * (64 * 2))
enum { PM_UTIL_DUMMY = 0, PM_UTIL_LED_ON, PM_UTIL_LED_BLINK_HIGH_SPEED, PM_UTIL_LED_BLINK_LOW_SPEED,
       PM_UTIL_LCD1_BACKLIGHT_ON, PM_UTIL_LCD1_BACKLIGHT_OFF, PM_UTIL_LCD2_BACKLIGHT_ON, PM_UTIL_LCD2_BACKLIGHT_OFF,
       PM_UTIL_LCD12_BACKLIGHT_ON, PM_UTIL_LCD12_BACKLIGHT_OFF, PM_UTIL_SOUND_POWER_ON, PM_UTIL_SOUND_POWER_OFF,
       PM_UTIL_SOUND_VOL_CTRL_ON, PM_UTIL_SOUND_VOL_CTRL_OFF, PM_UTIL_FORCE_POWER_OFF, PM_UTIL_FORCE_POWER_ON };
extern BOOL PM_SetLCDPower(PMLCDPower sw);   /* PM_SetLCDPower */
extern u32 PM_SendUtilityCommandAsync(u32 number, PMCallback callback, void *arg);   /* PM_SendUtilityCommandAsync */

static inline BOOL OS_DisableIrq(void)
{
    u16 prep = reg_OS_IME;
    reg_OS_IME = OS_IME_DISABLE;
    return (BOOL)prep;
}

static inline BOOL OS_EnableIrq(void)
{
    u16 prep = reg_OS_IME;
    reg_OS_IME = OS_IME_ENABLE;
    return (BOOL)prep;
}

static inline BOOL OS_RestoreIrq(BOOL enable)
{
    u16 prep = reg_OS_IME;
    reg_OS_IME = (u16)enable;
    return (BOOL)prep;
}

static inline OSIrqMask OS_GetRequestIrqMask(void)
{
    return reg_OS_IF;
}

static inline BOOL MB_IsMultiBootChild(void)
{
    return *(const u16 *)HW_WM_BOOT_BUF == MB_TYPE_MULTIBOOT;
}

static inline u32 OS_GetVBlankCount(void)
{
    return *(vu32 *)HW_VBLANK_COUNT_BUF;
}

static inline void GXS_DispOff(void)
{
    reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_MODE_MASK;
}

/* PM_ForceToPowerOffAsync -- NitroSDK pm.c: PM_ForceToPowerOffAsync: with the LCD back on and both backlights off,
 * ask the ARM7 for the forced power-off. */
u32 PM_ForceToPowerOffAsync (PMCallback callback, void * arg)
{
    PMLCDPower LCDResult;
    PMBackLightSwitch top;
    PMBackLightSwitch bottom;

    OS_SpinWait(PMi_LCD_POWER_WAIT_TICK);
    LCDResult = PM_GetLCDPower();

    if (LCDResult != PM_LCD_POWER_ON) {
        (void)PM_GetBackLight(&top, &bottom);
        if (top != PM_BACKLIGHT_OFF)
            (void)PM_SetBackLight(PM_LCD_TOP, PM_BACKLIGHT_OFF);
        if (bottom != PM_BACKLIGHT_OFF)
            (void)PM_SetBackLight(PM_LCD_BOTTOM, PM_BACKLIGHT_OFF);

        while (!PM_SetLCDPower(PM_LCD_POWER_ON)) {
            OS_SpinWait(PMi_LCD_POWER_WAIT_TICK);
        }
    }

    return PM_SendUtilityCommandAsync(PM_UTIL_FORCE_POWER_OFF, callback, arg);
}
