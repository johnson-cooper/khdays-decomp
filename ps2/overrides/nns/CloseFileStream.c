/* PS2 override: there is no per-player physical file descriptor to close.
 *
 * OpenFileStream opens a NitroFS view only to resolve the sound archive's absolute pack offset.
 * Every header/sample read then goes through kh_nitrofs_read_stream(), whose dedicated KhFile is
 * owned globally by the PS2 NitroFS layer.  Running FS_CloseFile on player->file during audio
 * teardown unnecessarily enters the shared ROM archive scheduler and was part of the New Game
 * teardown/scene-load race on real hardware.
 */
#include "nnsys/snd.h"

void CloseFileStream(NNSSndStrmPlayer *player)
{
    (void)player;
}
