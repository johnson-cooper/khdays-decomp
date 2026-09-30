/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndArcGetFileAddress.c (ps2/tools/prep_sources.py). Do not edit. */


/* khdays: shared-bss */

#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nnsys/snd.h"

NNSSndArc * data_0204ad4c;   /* sCurrent */

/* NNS_SndArcGetFileAddress -- NitroSystem sndarc.c: NNS_SndArcGetFileAddress. */
void * NNS_SndArcGetFileAddress (u32 fileId)
{
    NNSSndArc * arc = data_0204ad4c;

    if (fileId >= arc->fat->count) return NULL;
    return arc->fat->files[ fileId ].mem;
}
