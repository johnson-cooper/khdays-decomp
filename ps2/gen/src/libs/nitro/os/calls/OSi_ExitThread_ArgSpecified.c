/* PS2: mechanically prepared copy of libs/nitro/os/calls/OSi_ExitThread_ArgSpecified.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os.h"

#define offsetof(type, member) ((u32)&(((type *)0)->member))

void OS_InitContext(OSContext * context, u32 newpc, u32 newsp);
void OS_LoadContext(OSContext * context);
extern void OSi_ExitThread(void * arg);
extern void OSi_ExitThread (void * arg);

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

/* OSi_ExitThread_ArgSpecified -- NitroSDK os_thread.c: OSi_ExitThread_ArgSpecified. */
void OSi_ExitThread_ArgSpecified (OSThread * thread, void * arg)
{
    if (OSi_StackForDestructor) {
        OS_InitContext(&thread->context, (u32)OSi_ExitThread, (u32)OSi_StackForDestructor);
        thread->context.r[0] = (u32)arg;
        thread->context.cpsr |= HW_PSR_IRQ_DISABLE;
        thread->state = OS_THREAD_STATE_READY;
        OS_LoadContext(&thread->context);

    } else {
        OSi_ExitThread(arg);

    }
}
