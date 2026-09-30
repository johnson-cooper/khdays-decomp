/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndArcInit.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nnsys/snd.h"

void FS_InitFile(FSFile * p_file);
BOOL FS_OpenFileFast(FSFile * p_file, FSFileID file_id);
BOOL FS_ConvertPathToFileID(FSFileID * p_file_id, const char * path);
BOOL NNS_SndArcSetup(NNSSndArc * arc, NNSSndHeapHandle heap, BOOL symbolLoadFlag);
extern BOOL NNS_SndArcSetup (NNSSndArc * arc, NNSSndHeapHandle heap, BOOL symbolLoadFlag);

/* khdays: shared-bss */
NNSSndArc * data_0204ad4c;   /* sCurrent */

/* NNS_SndArcInit -- NitroSystem sndarc.c: NNS_SndArcInit. */
void NNS_SndArcInit (NNSSndArc * arc, const char * filePath, NNSSndHeapHandle heap, BOOL symbolLoadFlag)
{
    BOOL result;

    arc->info = NULL;
    arc->fat = NULL;
    arc->symbol = NULL;
    arc->loadBlockSize = 0;

    result = FS_ConvertPathToFileID(&arc->fileId, filePath);
    if (!result) return;

    FS_InitFile(&arc->file);
    result = FS_OpenFileFast(&arc->file, arc->fileId);
    if (!result) return;

    arc->file_open = TRUE;

    result = NNS_SndArcSetup(arc, heap, symbolLoadFlag);
    if (!result) return;

    data_0204ad4c = arc;
}
