/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndArcGetFileID.c (ps2/tools/prep_sources.py). Do not edit. */


/* khdays: shared-bss */

#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nnsys/snd.h"

NNSSndArc * data_0204ad4c;   /* sCurrent */

/* NNS_SndArcGetFileID -- NitroSystem sndarc.c: NNS_SndArcGetFileID. */
FSFileID NNS_SndArcGetFileID (void)
{
    NNSSndArc * arc = data_0204ad4c;

    return arc->fileId;
}
