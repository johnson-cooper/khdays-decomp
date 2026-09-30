/* PS2: mechanically prepared copy of libs/nitro/os/calls/OSi_ExitThread.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os.h"

#define offsetof(type, member) ((u32)&(((type *)0)->member))

extern OSIntrMode OS_DisableInterrupts(void);
extern void OSi_ExitThread_Destroy(void);
extern void OSi_ExitThread_Destroy (void);

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

/* OSi_ExitThread -- NitroSDK os_thread.c: OSi_ExitThread. */
void OSi_ExitThread (void * arg)
{
    OSThread * currentThread = OSi_GetCurrentThread();
    OSThreadDestructor destructor;

    destructor = currentThread->destructor;
    if (destructor) {
        currentThread->destructor = NULL;
        destructor(arg);
        (void)OS_DisableInterrupts();
    }

    OSi_ExitThread_Destroy();
}
