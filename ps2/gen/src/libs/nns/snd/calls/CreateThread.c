/* PS2: mechanically prepared copy of libs/nns/snd/calls/CreateThread.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

#define offsetof(type, member) ((u32)&(((type *)0)->member))

void OS_CreateThread(OSThread * thread, void (*func)(void *), void * arg, void * stack, u32 stackSize, u32 prio);
void OS_WakeupThreadDirect(OSThread * thread);
static inline void OS_InitThreadQueue (OSThreadQueue * queue)
{
    queue->head = queue->tail = NULL ;
}
void OS_InitMutex(OSMutex * mutex);
void NNS_FndInitList(NNSFndList * list, u16 offset);
typedef struct LoadCommand {
    NNSFndLink link;
    NNSSndStrmPlayer * player;
    NNSSndStrmCallbackStatus status;
    int numChannels;
    void * buffer[6 ];
    u32 bufLen;
} LoadCommand;
extern void StrmThread(void * arg);
extern void StrmThread (void * arg);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* NNSi_SndStrmCreateThread -- NitroSystem sndarc_stream.c: NNSi_SndStrmCreateThread. */
void NNSi_SndStrmCreateThread (NNSSndStrmThread * thread, u32 threadPrio)
{
    OS_CreateThread(
        &thread->thread,
        StrmThread,
        thread,
        thread->stack + NNS_SND_STRM_THREAD_STACK_SIZE / sizeof(u64),
        NNS_SND_STRM_THREAD_STACK_SIZE,
        threadPrio
        );
    NNS_FND_INIT_LIST(&thread->commandList, LoadCommand, link);
    OS_InitMutex(&thread->mutex);
    OS_InitThreadQueue(&thread->threadQ);
    OS_WakeupThreadDirect(&thread->thread);
}
