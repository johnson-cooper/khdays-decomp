/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndArcSetLoadBlockSize.c (ps2/tools/prep_sources.py). Do not edit. */


/* khdays: shared-bss */

#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nnsys/snd.h"

NNSSndArc * data_0204ad4c;   /* sCurrent */

/* NNS_SndArcSetLoadBlockSize -- NitroSystem sndarc.c: NNS_SndArcSetLoadBlockSize. */
void NNS_SndArcSetLoadBlockSize (s32 loadBlockSize)
{
    NNSSndArc * arc = data_0204ad4c;

    arc->loadBlockSize = loadBlockSize;
}
