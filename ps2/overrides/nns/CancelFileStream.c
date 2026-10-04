/* PS2 override: stream sample reads bypass NitroFS after OpenFileStream.
 *
 * ReadFileStream uses kh_nitrofs_read_stream() and therefore has no asynchronous FS command
 * attached to player->file to cancel.  Calling the DS FS_CancelFile path during teardown mutates
 * the shared ROM archive command queue from the cleanup thread and can strand a later synchronous
 * scene open (observed as FS_OpenFile("/db/db.p2") sleeping forever on hardware).
 *
 * The memory-stream implementation is also a no-op; the PS2 file-stream path has the same
 * teardown semantics because its actual storage descriptor is the global dedicated stream pack.
 */
#include "nnsys/snd.h"

void CancelFileStream(NNSSndStrmPlayer *player)
{
    (void)player;
}
