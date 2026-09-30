/* PS2: mechanically prepared copy of libs/nitro/os/calls/OSi_ExitThread_Destroy.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os.h"

#define offsetof(type, member) ((u32)&(((type *)0)->member))

#define SDK_THREAD_INFINITY 1

void OS_Terminate();
extern void OS_Terminate(void);
void OS_RescheduleThread(void);
void OS_WakeupThread(OSThreadQueue * queue);
u32 OS_DisableScheduler(void);
u32 OS_EnableScheduler(void);
void OSi_UnlockAllMutex(OSThread * thread);
extern void OSi_RemoveThreadFromList(OSThread * thread);
extern OSThread * OSi_RemoveSpecifiedLinkFromQueue (OSThreadQueue * queue, OSThread * thread);
extern void OSi_RemoveThreadFromList (OSThread * thread);
extern void OS_WakeupThread (OSThreadQueue * queue);
extern void OS_RescheduleThread (void);
extern u32 OS_DisableScheduler (void);
extern u32 OS_EnableScheduler (void);

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

/* OSi_ExitThread_Destroy -- NitroSDK os_thread.c: OSi_ExitThread_Destroy. */
void OSi_ExitThread_Destroy (void)
{
    OSThread * currentThread = OSi_GetCurrentThread();

#ifdef SDK_THREAD_INFINITY
    (void)OS_DisableScheduler();
#endif

#ifndef SDK_THREAD_INFINITY

#endif

    OSi_UnlockAllMutex(currentThread);

    if (currentThread->queue) {
        (void)OSi_RemoveSpecifiedLinkFromQueue(currentThread->queue, currentThread);
    }

    OSi_RemoveThreadFromList(currentThread);

#ifndef SDK_THREAD_INFINITY
    data_02044330.entry[currentThread->id] = NULL;
#endif
    currentThread->state = OS_THREAD_STATE_TERMINATED;

#ifndef SDK_THREAD_INFINITY
    OS_WakeupThread(&currentThread->joinQueue);
#else
    OS_WakeupThread(&currentThread->joinQueue);
#endif

#ifdef SDK_THREAD_INFINITY
    (void)OS_EnableScheduler();
#endif

    OS_RescheduleThread();
    OS_Terminate();
}
