/* PS2: mechanically prepared copy of libs/nns/snd/calls/RemoveCommandByPlayer.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

extern OSIntrMode OS_DisableInterrupts(void);
extern OSIntrMode OS_RestoreInterrupts(OSIntrMode state);
void NNS_FndRemoveListObject(NNSFndList * list, void * object);
void * NNS_FndGetNextListObject(NNSFndList * list, void * object);
typedef struct LoadCommand {
    NNSFndLink link;
    NNSSndStrmPlayer * player;
    NNSSndStrmCallbackStatus status;
    int numChannels;
    void * buffer[6 ];
    u32 bufLen;
} LoadCommand;
extern void FreeCommandBuffer(LoadCommand * command);
extern void FreeCommandBuffer (LoadCommand * command);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* RemoveCommandByPlayer -- NitroSystem sndarc_stream.c: RemoveCommandByPlayer. */
void RemoveCommandByPlayer (NNSFndList * commandList, const NNSSndStrmPlayer * player)
{
    OSIntrMode old;
    LoadCommand * command;
    LoadCommand * next;

    old = OS_DisableInterrupts();

    for (command = (LoadCommand *)NNS_FndGetNextListObject(commandList, NULL);
         command != NULL; command = next) {
        next = (LoadCommand *)NNS_FndGetNextListObject(commandList, command);

        if (command->player == player) {
            NNS_FndRemoveListObject(commandList, command);
            FreeCommandBuffer(command);
        }
    }

    (void)OS_RestoreInterrupts(old);
}
