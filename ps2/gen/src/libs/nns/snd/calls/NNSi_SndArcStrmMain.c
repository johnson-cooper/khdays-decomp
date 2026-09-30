/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNSi_SndArcStrmMain.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

extern const s16 data_02041488[128 ];
static inline
s16 SND_CalcDecibel (int scale)
{
    return data_02041488[scale];
}
void NNS_SndStrmStart(NNSSndStrm * stream);
void NNS_SndStrmSetVolume(NNSSndStrm * stream, int volume);
int NNSi_SndFaderGet(const NNSSndFader * fader);
void NNSi_SndFaderUpdate(NNSSndFader * fader);
BOOL NNSi_SndFaderIsFinished(const NNSSndFader * fader);
extern NNSSndStrmPlayer data_0204b62c[4 ];
extern void ForceStopStrm_2(NNSSndStrmPlayer * player);
extern void ForceStopStrm_2 (NNSSndStrmPlayer * player);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* NNSi_SndArcStrmMain -- NitroSystem sndarc_stream.c: NNSi_SndArcStrmMain. */
void NNSi_SndArcStrmMain (void)
{
    NNSSndStrmPlayer * player;
    int playerNo;
    int volume;

    for (playerNo = 0; playerNo < NNS_SND_STRM_PLAYER_NUM; ++playerNo) {
        player = &data_0204b62c[ playerNo ];

        if (!player->activeFlag) continue;

        if (player->finishCounter == 0) {
            ForceStopStrm_2(player);
            continue;
        }

        if (player->startFlag) {
            if (player->prepareFlag) {
                NNS_SndStrmStart(&player->stream);

                player->playFlag = TRUE;
                player->startFlag = FALSE;
            }
        }

        if (!player->playFlag) continue;

        NNSi_SndFaderUpdate(&player->fader);

        volume
            = SND_CalcDecibel(NNSi_SndFaderGet(&player->fader) >> 8)
              + SND_CalcDecibel(player->initVolume)
              + SND_CalcDecibel(player->extVolume)
            ;
        if (volume != player->volume) {
            NNS_SndStrmSetVolume(&player->stream, volume);

            player->volume = volume;
        }

        if (player->fadeOutFlag) {
            if (NNSi_SndFaderIsFinished(&player->fader)) {
                ForceStopStrm_2(player);
            }
        }
    }
}
