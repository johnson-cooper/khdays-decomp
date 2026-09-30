/* PS2: mechanically prepared copy of libs/nns/snd/calls/ReadFileStream.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

s32 FS_ReadFile(FSFile * p_file, void * dst, s32 len);
BOOL FS_SeekFile(FSFile * p_file, s32 offset, FSSeekFileMode origin);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* ReadFileStream -- NitroSystem sndarc_stream.c: ReadFileStream. */
s32 ReadFileStream (NNSSndStrmPlayer * player, void * dest, u32 size, u32 offset)
{
    BOOL result;

    result = FS_SeekFile(
        &player->file,
        (s32)(player->fileOffset + offset),
        FS_SEEK_SET
        );
    return FS_ReadFile(
        &player->file,
        dest,
        (s32)size
        );
}
