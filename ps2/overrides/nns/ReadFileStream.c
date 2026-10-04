/* PS2 override: keep streamed-music sample reads off the main NitroFS pack descriptor.
 *
 * The DS archive driver serialized card access.  On PS2, the game thread and NitroSystem stream
 * worker are genuine concurrent EE threads.  The original ReadFileStream goes through the same
 * archive callback as every scene load, so both threads mutate one KhFile's lseek position and
 * read-ahead cache.  OpenFileStream has a matching PS2 override for the stream header; together
 * they keep the complete stream on its own serialized descriptor.
 */
#include "nitro/types.h"
#include "nitro/fs.h"
#include "nnsys/snd.h"

extern s32 kh_nitrofs_read_stream(void *dst, u32 packPos, u32 size);

s32 ReadFileStream(NNSSndStrmPlayer *player, void *dest, u32 size, u32 offset)
{
    u32 packPos = player->file.prop.file.top + player->fileOffset + offset;
    return kh_nitrofs_read_stream(dest, packPos, size);
}
