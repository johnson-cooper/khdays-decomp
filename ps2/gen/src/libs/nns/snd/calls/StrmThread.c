/* PS2: mechanically prepared copy of libs/nns/snd/calls/StrmThread.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

void OS_SleepThread(OSThreadQueue * queue);
void OS_LockMutex(OSMutex * mutex);
void OS_UnlockMutex(OSMutex * mutex);
typedef struct LoadCommand {
    NNSFndLink link;
    NNSSndStrmPlayer * player;
    NNSSndStrmCallbackStatus status;
    int numChannels;
    void * buffer[6 ];
    u32 bufLen;
} LoadCommand;
extern LoadCommand * ReadCommandBuffer(NNSFndList * commandList);
extern void FreeCommandBuffer(LoadCommand * command);
extern void NNSi_SndArcStrm_MakeWaveData(LoadCommand * command);
extern LoadCommand * ReadCommandBuffer (NNSFndList * commandList);
extern void FreeCommandBuffer (LoadCommand * command);
extern void NNSi_SndArcStrm_MakeWaveData (LoadCommand * command);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* StrmThread -- NitroSystem sndarc_stream.c: StrmThread. */
void StrmThread (void * arg)
{
    NNSSndStrmThread * thread = (NNSSndStrmThread *)arg;

    while (1) {
        LoadCommand * command;

        OS_SleepThread(&thread->threadQ);

        while (1) {
            OS_LockMutex(&thread->mutex);

            command = ReadCommandBuffer(&thread->commandList);
            if (command == NULL) {
                OS_UnlockMutex(&thread->mutex);
                break;
            }

            NNSi_SndArcStrm_MakeWaveData(command);
            FreeCommandBuffer(command);
            OS_UnlockMutex(&thread->mutex);
        }
    }
}
