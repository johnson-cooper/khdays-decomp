/* PS2: mechanically prepared copy of libs/nns/snd/calls/CloseFileStream.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

BOOL FS_CloseFile(FSFile * p_file);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* CloseFileStream -- NitroSystem sndarc_stream.c: CloseFileStream. */
void CloseFileStream (NNSSndStrmPlayer * player)
{
    (void)FS_CloseFile(&player->file);
}
