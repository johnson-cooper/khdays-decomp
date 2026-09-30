/* PS2: mechanically prepared copy of libs/nitro/os/calls/OS_ExitThread.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os.h"

#define offsetof(type, member) ((u32)&(((type *)0)->member))

#define SDK_THREAD_INFINITY 1

extern OSIntrMode OS_DisableInterrupts(void);
extern void OSi_ExitThread_ArgSpecified(OSThread * thread, void * arg);
extern void OSi_ExitThread_Destroy(void);
extern void OSi_ExitThread_ArgSpecified (OSThread * thread, void * arg);
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

static inline OSThreadInfo * OS_GetThreadInfo (void)
{
    return &data_02044330;
}
static inline OSThread * OS_GetCurrentThread (void)
{
    return OS_GetThreadInfo()->current;
}

/* OS_ExitThread -- NitroSDK os_thread.c: OS_ExitThread. */
void OS_ExitThread (void)
{
    (void)OS_DisableInterrupts();

#ifdef SDK_THREAD_INFINITY
    OSi_ExitThread_ArgSpecified(OS_GetCurrentThread(), 0);
#else
    OSi_ExitThread_Destroy();
#endif
}
