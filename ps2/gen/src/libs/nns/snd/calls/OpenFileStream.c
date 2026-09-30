/* PS2: mechanically prepared copy of libs/nns/snd/calls/OpenFileStream.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

BOOL FS_OpenFileFast(FSFile * p_file, FSFileID file_id);
s32 NNS_SndArcReadFile(u32 fileId, void * buffer, s32 size, s32 offset);
FSFileID NNS_SndArcGetFileID(void);
u32 NNS_SndArcGetFileOffset(u32 fileId);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* OpenFileStream -- NitroSystem sndarc_stream.c: OpenFileStream. */
BOOL OpenFileStream (NNSSndStrmPlayer * player, u32 fileId)
{
    if (NNS_SndArcReadFile(fileId, &player->info, sizeof(player->info), 0) != sizeof(player->info)) {
        return FALSE;
    }

    if (!FS_OpenFileFast(&player->file, NNS_SndArcGetFileID())) {
        return FALSE;
    }
    player->fileOffset = NNS_SndArcGetFileOffset(fileId);

    return TRUE;
}
