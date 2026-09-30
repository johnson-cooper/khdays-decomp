/* PS2: mechanically prepared copy of libs/nitro/snd/calls/SND_RecvCommandReply.c (ps2/tools/prep_sources.py). Do not edit. */


/* NitroSDK SND library (ARM9 side, snd_command.c): the command queue to the ARM7 sound driver. */

#include "nitro/types.h"
#include "nitro/os.h"

typedef int PXIFifoTag;
typedef struct SNDCommand {
    struct SNDCommand *next;      /* 0x00 */
    u32 id;                       /* 0x04 */
    u32 arg[4];                   /* 0x08 */
} SNDCommand;                     /* 0x18 */
typedef struct SNDSharedWork SNDSharedWork;
#define SND_COMMAND_NUM 256
#define SND_PXI_FIFO_MESSAGE_BUFSIZE 8
#define SND_COMMAND_NOBLOCK 0
#define SND_COMMAND_BLOCK 1
#define SND_COMMAND_SHARED_WORK 0x1d
#define PXI_FIFO_TAG_SOUND 7
#define PXI_PROC_ARM7 1

/* snd_command.c's zero-initialised statics are its .bss scalars, addressed off the block's base
 * (data_02044748 = sFreeList): defined here in the order that makes mwcc lay them out as the ROM
 * does (reverse declaration order, the last one appended); the module's delinked bss keeps the
 * symbols. The aggregates are addressed by their own symbols and stay extern. */
/* khdays: shared-bss */
int sWaitingCommandListCount;
int sWaitingCommandListQueueWrite;
int sWaitingCommandListQueueRead;
SNDCommand *sFreeListEnd;
SNDCommand *sReserveListEnd;
SNDCommand *sReserveList;
u32 sFinishedTag;
SNDCommand *data_02044748;   /* sFreeList: the base of the block */
u32 sCurrentTag;
#define sFreeList data_02044748
extern SNDCommand *data_0204476c[SND_PXI_FIFO_MESSAGE_BUFSIZE + 1];   /* sWaitingCommandListQueue */
#define sWaitingCommandListQueue data_0204476c
extern SNDSharedWork data_020447a0;   /* sSharedWork (0x280 bytes in this SDK, unaligned) */
#define sSharedWork data_020447a0
extern SNDCommand data_02044a20[SND_COMMAND_NUM] __attribute__((aligned(32)));   /* sCommandArray */
#define sCommandArray data_02044a20
extern SNDSharedWork *data_02046280;   /* SNDi_SharedWork */
#define SNDi_SharedWork data_02046280

extern OSIntrMode OS_DisableInterrupts(void);
extern OSIntrMode OS_RestoreInterrupts(OSIntrMode state);
extern void OS_SpinWait(u32 cycles);   /* OS_SpinWait */
extern void PXI_SetFifoRecvCallback(int fifotag, void (*callback)(PXIFifoTag, u32, BOOL));
extern BOOL PXI_IsCallbackReady(int fifotag, int proc);
extern void PxiFifoCallback(PXIFifoTag tag, u32 data, BOOL err);
extern BOOL IsCommandAvailable(void);
extern void InitPXI(void);
extern void SNDi_InitSharedWork(SNDSharedWork *work);
extern u32 SNDi_GetFinishedCommandTag(void);
extern SNDCommand *SND_AllocCommand(u32 flags);
extern void SND_PushCommand(SNDCommand *command);   /* SND_PushCommand */
extern BOOL SND_FlushCommand(u32 flags);              /* SND_FlushCommand */

/* SND_RecvCommandReply -- NitroSDK snd_command.c: SND_RecvCommandReply. */
const SNDCommand * SND_RecvCommandReply (u32 flags)
{
    OSIntrMode bak_psr = OS_DisableInterrupts();
    SNDCommand * commandList;
    SNDCommand * commandListEnd;

    if (flags & SND_COMMAND_BLOCK) {
        while (sFinishedTag == SNDi_GetFinishedCommandTag()) {
            (void)OS_RestoreInterrupts(bak_psr);
            OS_SpinWait(100);
            bak_psr = OS_DisableInterrupts();
        }
    } else {
        if (sFinishedTag == SNDi_GetFinishedCommandTag()) {
            (void)OS_RestoreInterrupts(bak_psr);
            return NULL;
        }
    }

    commandList = sWaitingCommandListQueue[sWaitingCommandListQueueRead];
    sWaitingCommandListQueueRead++;
    if (sWaitingCommandListQueueRead > SND_PXI_FIFO_MESSAGE_BUFSIZE)
        sWaitingCommandListQueueRead = 0;

    commandListEnd = commandList;
    while (commandListEnd->next != NULL) {
        commandListEnd = commandListEnd->next;
    }

    if (sFreeListEnd != NULL) {
        sFreeListEnd->next = commandList;
    } else {
        sFreeList = commandList;
    }

    sFreeListEnd = commandListEnd;

    sWaitingCommandListCount--;
    sFinishedTag++;

    (void)OS_RestoreInterrupts(bak_psr);
    return commandList;
}
