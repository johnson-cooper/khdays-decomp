/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndArcStrmSetupPlayer.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

#define BLOCK_SIZE 512
#define BLOCK_NUM 4

void * NNS_SndHeapAlloc(NNSSndHeapHandle heap, u32 size, NNSSndHeapDisposeCallback callback, u32 data1, u32 data2);
const NNSSndArcStrmPlayerInfo * NNS_SndArcGetStrmPlayerInfo(int playerNo);
extern NNSSndStrmPlayer data_0204b62c[4 ];
extern void ForceStopStrm_2(NNSSndStrmPlayer * player);
extern void DisposeCallback_2(void * mem, u32 size, u32 data1, u32 data2);
extern void ForceStopStrm_2 (NNSSndStrmPlayer * player);
extern void DisposeCallback_2 (void * mem, u32, u32 data1, u32);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* NNS_SndArcStrmSetupPlayer -- NitroSystem sndarc_stream.c: NNS_SndArcStrmSetupPlayer. */
BOOL NNS_SndArcStrmSetupPlayer (NNSSndHeapHandle heap)
{
    int playerNo;
    const NNSSndArcStrmPlayerInfo * playerInfo;
    NNSSndStrmPlayer * player;
    void * buffer;
    u32 bufSize;
    int i;

    for (playerNo = 0; playerNo < NNS_SND_STRM_PLAYER_NUM; ++playerNo) {
        player = &data_0204b62c[ playerNo ];

        playerInfo = NNS_SndArcGetStrmPlayerInfo(playerNo);
        if (playerInfo == NULL) continue;

        player->numChannels = playerInfo->numChannels;
        for (i = 0; i < playerInfo->numChannels; i++) {
            player->chNoList[ i ] = playerInfo->chNoList[ i ];
        }

        if (heap != NNS_SND_HEAP_INVALID_HANDLE) {
            bufSize = (unsigned long)(BLOCK_SIZE * BLOCK_NUM * player->numChannels);
            buffer = NNS_SndHeapAlloc(heap, bufSize, DisposeCallback_2, (u32)player, 0);
            if (buffer == NULL) return FALSE;

            ForceStopStrm_2(player);

            player->buffer = buffer;
            player->bufSize = bufSize;
        }
    }

    return TRUE;
}
