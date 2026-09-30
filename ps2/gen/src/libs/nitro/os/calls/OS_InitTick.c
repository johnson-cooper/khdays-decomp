/* PS2: mechanically prepared copy of libs/nitro/os/calls/OS_InitTick.c (ps2/tools/prep_sources.py). Do not edit. */
/* NitroSDK OS: once, reserves timer 0, starts it with IRQ and installs the tick count-up handler.
 */

extern void OSi_SetTimerReserved(int timer);
extern void OS_SetIrqFunction(unsigned int mask, void (*callback)(void));
extern unsigned int OS_EnableIrqMask(unsigned int mask);
extern void OSi_CountUpTick(void);

typedef struct {
    unsigned short active;
    unsigned short _pad;
    int field_4;
    int field_8;
    int field_c;
} Data_02044664;

extern Data_02044664 data_02044664;

#define TM0CNT_L (*(volatile unsigned short *)((unsigned int)kh_ds_io + 0x100))
#define TM0CNT_H (*(volatile unsigned short *)((unsigned int)kh_ds_io + 0x102))

void OS_InitTick(void)
{
    if (data_02044664.active != 0)
        return;

    data_02044664.active = 1;
    OSi_SetTimerReserved(0);
    data_02044664.field_8 = 0;
    data_02044664.field_c = 0;
    TM0CNT_H = 0;
    TM0CNT_L = 0;
    TM0CNT_H = 0xC1;
    OS_SetIrqFunction(8, OSi_CountUpTick);
    OS_EnableIrqMask(8);
    data_02044664.field_4 = 0;
}
