/* PS2 override: tear down a stream player without DS file callbacks or layout tricks.
 *
 * The matching DS source reaches the two stream-thread command lists through address-coupled
 * globals and calls player->closeStreamFunc indirectly.  On PS2, file streams use a global
 * dedicated pack descriptor and their close/cancel callbacks are intentionally no-ops.  Use the
 * typed NitroSystem structures directly so teardown cannot jump through a stale callback pointer
 * or perturb the shared NitroFS ROM command queue while scene 11 is opening.
 */
#include "nitro/types.h"
#include "nnsys/snd.h"

extern NNSSndStrmThread data_0204b140;
extern NNSSndStrmThread *sPrepareThread;

extern void FreeChannel(NNSSndStrmPlayer *player);
extern void RemoveCommandByPlayer(NNSFndList *list, const NNSSndStrmPlayer *player);
extern void FreePlayer(NNSSndStrmPlayer *player);
extern volatile const char *kh_watchdog_mark;

void OSi_DestroyThread(NNSSndStrmPlayer *player)
{
    if (player == NULL)
        return;

    kh_watchdog_mark = "audio cleanup: free channel";
    FreeChannel(player);

    /*
     * No closeStreamFunc call here.  CloseMemoryStream is empty on the DS, and PS2
     * CloseFileStream is also empty because the real stream descriptor is global.
     */

    kh_watchdog_mark = "audio cleanup: remove load commands";
    RemoveCommandByPlayer(&data_0204b140.commandList, player);
    if (sPrepareThread != NULL)
        RemoveCommandByPlayer(&sPrepareThread->commandList, player);

    kh_watchdog_mark = "audio cleanup: free player";
    FreePlayer(player);
    kh_watchdog_mark = "audio cleanup: done";
}
