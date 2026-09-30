/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndArcSetFileAddress.c (ps2/tools/prep_sources.py). Do not edit. */


/* khdays: shared-bss */

#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nnsys/snd.h"

NNSSndArc * data_0204ad4c;   /* sCurrent */

/* NNS_SndArcSetFileAddress -- NitroSystem sndarc.c: NNS_SndArcSetFileAddress. */
void NNS_SndArcSetFileAddress (u32 fileId, void * address)
{
    NNSSndArc * arc = data_0204ad4c;

    arc->fat->files[ fileId ].mem = address;
}
