/* PS2: mechanically prepared copy of libs/nns/snd/calls/FreeChannel.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

void NNS_SndStrmFreeChannel(NNSSndStrm * stream);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* FreeChannel -- NitroSystem sndarc_stream.c: FreeChannel. */
void FreeChannel (NNSSndStrmPlayer * player)
{

    if (player->allocChannelCount == 0) {
        return;
    }

    player->allocChannelCount--;

    if (player->allocChannelCount == 0) {
        NNS_SndStrmFreeChannel(&player->stream);
    }
}
