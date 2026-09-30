/* PS2: mechanically prepared copy of libs/nitro/os/calls/OS_DisableScheduler.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os.h"

#define offsetof(type, member) ((u32)&(((type *)0)->member))

extern OSIntrMode OS_DisableInterrupts(void);
extern OSIntrMode OS_RestoreInterrupts(OSIntrMode state);

/* khdays: shared-bss */
u32 OSi_ThreadIdCount;   /* OSi_ThreadIdCount */
void * OSi_StackForDestructor;   /* OSi_StackForDestructor */
u32 OSi_SystemStackBuffer;   /* OSi_SystemStackBuffer */
vu32 killThreadStatus;   /* killThreadStatus */
vu32 exitThreadStatus;   /* exitThreadStatus */
BOOL OSi_IsThreadInitialized;   /* OSi_IsThreadInitialized */
OSThread ** OSi_CurrentThreadPtr;   /* OSi_CurrentThreadPtr */
u32 OSi_RescheduleCount;   /* OSi_RescheduleCount */
void * data_0204430c;   /* data_0204430c */
OSThreadInfo data_02044330;   /* data_02044330 */

/* OS_DisableScheduler -- NitroSDK os_thread.c: OS_DisableScheduler. */
u32 OS_DisableScheduler (void)
{
    OSIntrMode enabled = OS_DisableInterrupts();
    u32 count;

    if (OSi_RescheduleCount < (u32)(0 - 1)) {
        count = OSi_RescheduleCount++;
    }
    (void)OS_RestoreInterrupts(enabled);

    return count;
}
