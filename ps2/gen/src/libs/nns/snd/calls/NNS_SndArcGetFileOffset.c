/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndArcGetFileOffset.c (ps2/tools/prep_sources.py). Do not edit. */


/* khdays: shared-bss */

#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nnsys/snd.h"

NNSSndArc * data_0204ad4c;   /* sCurrent */

/* NNS_SndArcGetFileOffset -- NitroSystem sndarc.c: NNS_SndArcGetFileOffset. */
u32 NNS_SndArcGetFileOffset (u32 fileId)
{
    NNSSndArc * arc = data_0204ad4c;

    if (fileId >= arc->fat->count) return 0;
    return arc->fat->files[ fileId ].offset;
}
