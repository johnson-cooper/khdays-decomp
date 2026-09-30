/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndArcStrmInit.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

#define BLOCK_SIZE 512
#define BLOCK_NUM 4

void * NNS_SndHeapAlloc(NNSSndHeapHandle heap, u32 size, NNSSndHeapDisposeCallback callback, u32 data1, u32 data2);
const NNSSndArcStrmPlayerInfo * NNS_SndArcGetStrmPlayerInfo(int playerNo);
/* One queued stream request of the preparation thread (0x30 bytes), linked at +0. */
typedef struct StrmCommand {
    NNSFndLink link;
    u32 param[10];
} StrmCommand;

#define COMMAND_NUM 8

extern NNSSndStrmPlayer data_0204b62c[NNS_SND_STRM_PLAYER_NUM];   /* sStrmPlayer */
extern NNSFndList data_0204ad98;                                  /* sFreeCommandList */
extern OSMutex data_0204ada4;                                     /* sCommandMutex */
extern StrmCommand data_0204adbc[COMMAND_NUM];                    /* sCommandArray */
extern u8 data_0204af40[BLOCK_SIZE];                              /* sDecodeBufferArea */
extern NNSSndStrmThread data_0204b140;                            /* sStrmThread */
void NNS_FndInitList(NNSFndList * list, u16 offset);
void NNS_FndAppendListObject(NNSFndList * list, void * object);
void OS_InitMutex(OSMutex * mutex);
void FS_InitFile(FSFile * p_file);
void NNS_SndStrmInit(NNSSndStrm * stream);
BOOL NNS_SndArcStrmSetupPlayer(NNSSndHeapHandle heap);
void NNSi_SndStrmCreateThread(NNSSndStrmThread * thread, u32 threadPrio);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* Both loops are emitted unrotated (entry branch to the bottom test) and index their arrays
 * with an mla each pass: this routine was built without loop rotation or strength reduction. */
#pragma opt_rotateloops off
#pragma opt_strength_reduction off
/* NNS_SndArcStrmInit -- NitroSystem sndarc_stream.c: NNS_SndArcStrmInit. */
void NNS_SndArcStrmInit (u32 threadPrio, NNSSndHeapHandle heap)
{
    int i;
    int playerNo;
    NNSSndStrmPlayer * player;

    if (data_0204ad8c) {
        (void)NNS_SndArcStrmSetupPlayer(heap);
        return;
    }
    data_0204ad8c = TRUE;

    NNS_FndInitList(&data_0204ad98, 0);
    for (i = 0; i < COMMAND_NUM; i++) {
        NNS_FndAppendListObject(&data_0204ad98, &data_0204adbc[i]);
    }
    OS_InitMutex(&data_0204ada4);

    sDecodeBuffer = data_0204af40;

    for (playerNo = 0; playerNo < NNS_SND_STRM_PLAYER_NUM; playerNo++) {
        player = &data_0204b62c[playerNo];

        player->activeFlag = FALSE;
        FS_InitFile(&player->file);
        NNS_SndStrmInit(&player->stream);
        player->playerNo = playerNo;
        player->numChannels = 0;
        player->buffer = NULL;
        player->bufSize = 0;
        player->allocChannelCount = 0;
    }

    (void)NNS_SndArcStrmSetupPlayer(heap);

    NNSi_SndStrmCreateThread(&data_0204b140, threadPrio);
}
#pragma opt_strength_reduction reset
#pragma opt_rotateloops reset
