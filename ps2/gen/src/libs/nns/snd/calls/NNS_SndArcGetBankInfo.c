/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndArcGetBankInfo.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nnsys/snd.h"

static inline
const void * GetPtrConst (const void * base, u32 offset)
{
    if (offset == 0) return NULL ;
    return (const u8 *)base + offset;
}
static inline
const NNSSndArcOffsetTable * GetOffsetTable (const NNSSndArcInfo * info, u32 offset)
{
    return (const NNSSndArcOffsetTable *)GetPtrConst(info, offset);
}

/* khdays: shared-bss */
NNSSndArc * data_0204ad4c;   /* sCurrent */

/* NNS_SndArcGetBankInfo -- NitroSystem sndarc.c: NNS_SndArcGetBankInfo. */
const NNSSndArcBankInfo * NNS_SndArcGetBankInfo (int bankNo)
{
    NNSSndArc * arc = data_0204ad4c;
    const NNSSndArcOffsetTable * table;

    table = GetOffsetTable(arc->info, arc->info->bankOffset);
    if (table == NULL) return NULL;

    if (bankNo < 0) return NULL;
    if (bankNo >= table->count) return NULL;

    return (const NNSSndArcBankInfo *)GetPtrConst(arc->info, table->offset[ bankNo ]);
}
