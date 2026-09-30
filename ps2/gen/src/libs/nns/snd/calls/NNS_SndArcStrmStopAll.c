/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndArcStrmStopAll.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

extern NNSSndStrmPlayer data_0204b62c[4 ];
extern void SNDi_FreeVoiceChannel(NNSSndStrmPlayer * player, int fadeFrame);
extern void SNDi_FreeVoiceChannel (NNSSndStrmPlayer * player, int fadeFrame);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* NNS_SndArcStrmStopAll -- NitroSystem sndarc_stream.c: NNS_SndArcStrmStopAll. */
void NNS_SndArcStrmStopAll (int fadeFrame)
{
    int playerNo;

    for (playerNo = 0; playerNo < NNS_SND_STRM_PLAYER_NUM; ++playerNo) {
        if (!data_0204b62c[ playerNo ].activeFlag) continue;

        SNDi_FreeVoiceChannel(&data_0204b62c[ playerNo ], fadeFrame);
    }
}
