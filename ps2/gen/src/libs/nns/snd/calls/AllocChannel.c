/* PS2: mechanically prepared copy of libs/nns/snd/calls/AllocChannel.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

BOOL NNS_SndStrmAllocChannel(NNSSndStrm * stream, int numChannels, const u8 chNoList[]);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* AllocChannel -- NitroSystem sndarc_stream.c: AllocChannel. */
BOOL AllocChannel (NNSSndStrmPlayer * player, int numChannels, const u8 chNoList[])
{

    if (player->allocChannelCount == 0) {
        if (!NNS_SndStrmAllocChannel(&player->stream, numChannels, chNoList)) {
            return FALSE;
        }
    }

    player->allocChannelCount++;

    return TRUE;
}
