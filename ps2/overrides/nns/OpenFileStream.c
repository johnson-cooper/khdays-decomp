/* PS2 override: open a streamed sound entirely through the stream pack descriptor.
 *
 * NitroSystem normally reads the 64-byte STRM header with NNS_SndArcReadFile, then opens the
 * archive file used for the later sample reads.  On the PS2 the first operation went through the
 * scene-data descriptor while the refills went through the dedicated stream descriptor.  Starting
 * the New Game transition stream could therefore race the loader on the scene descriptor before
 * the title fade had even begun.  Real USB/MMCE hardware can leave the game thread parked in that
 * race; PCSX2's effectively instant host reads hide it.
 *
 * Open the archive view first so its absolute pack offset is known, then read the header through
 * the same serialized stream descriptor as every later refill.
 */
#include "nitro/types.h"
#include "nitro/fs.h"
#include "nnsys/snd.h"

extern BOOL FS_OpenFileFast(FSFile *file, FSFileID id);
extern BOOL FS_CloseFile(FSFile *file);
extern FSFileID NNS_SndArcGetFileID(void);
extern u32 NNS_SndArcGetFileOffset(u32 fileId);
extern s32 kh_nitrofs_read_stream(void *dst, u32 packPos, u32 size);

BOOL OpenFileStream(NNSSndStrmPlayer *player, u32 fileId)
{
    u32 packPos;

    if (!FS_OpenFileFast(&player->file, NNS_SndArcGetFileID()))
        return FALSE;

    player->fileOffset = NNS_SndArcGetFileOffset(fileId);
    packPos = player->file.prop.file.top + player->fileOffset;
    if (kh_nitrofs_read_stream(&player->info, packPos, sizeof player->info) !=
        (s32)sizeof player->info) {
        (void)FS_CloseFile(&player->file);
        return FALSE;
    }

    return TRUE;
}
