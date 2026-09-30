/* PS2: mechanically prepared copy of libs/nitro/os/calls/OS_GetTick.c (ps2/tools/prep_sources.py). Do not edit. */

#include "nitro/types.h"
#include "nitro/os_types.h"

typedef void *OSMessage;

#define HW_MAIN_MEM 0x02000000

#define offsetof(type, member) ((u32)&(((type *)0)->member))

#define REG_TM0CNT_L_OFFSET 0x100
#define REG_TM0CNT_L_ADDR (HW_REG_BASE + REG_TM0CNT_L_OFFSET)
#define REG_IF_OFFSET 0x214
#define REG_IF_ADDR (HW_REG_BASE + REG_IF_OFFSET)
#define reg_OS_IF (*( REGType32v *) REG_IF_ADDR)
#define REG_OS_IE_T0_SHIFT 3
#define HW_IOREG ((unsigned int)kh_ds_io + 0x0)
#define HW_REG_BASE HW_IOREG        // alias
#define OS_IE_TIMER0 (1UL << REG_OS_IE_T0_SHIFT)
#define OSi_TICK_TIMER OS_TIMER_0
#define OSi_TICK_IE_TIMER OS_IE_TIMER0

typedef u16 REGType16;
extern OSIntrMode OS_DisableInterrupts(void);
extern OSIntrMode OS_RestoreInterrupts(OSIntrMode state);
typedef enum {
    OS_TIMER_0 = 0,
    OS_TIMER_1 = 1,
    OS_TIMER_2 = 2,
    OS_TIMER_3 = 3
} OSTimer;

/* khdays: shared-bss */
BOOL OSi_NeedResetTimer;   /* OSi_NeedResetTimer */
u16 data_02044664;   /* data_02044664 */
vu64 data_0204466c;   /* data_0204466c */

/* OS_GetTick -- NitroSDK os_tick.c: OS_GetTick. */
OSTick OS_GetTick (void)
{
    vu16 countL;
    vu64 countH;

    OSIntrMode prev = OS_DisableInterrupts();

    countL = *(REGType16 *)((u32)REG_TM0CNT_L_ADDR + OSi_TICK_TIMER * 4);
    countH = data_0204466c & 0xffffffffffffULL;

    if (reg_OS_IF & OSi_TICK_IE_TIMER && !(countL & 0x8000)) {
        countH++;
    }

    (void)OS_RestoreInterrupts(prev);

    return (countH << 16) | countL;
}
