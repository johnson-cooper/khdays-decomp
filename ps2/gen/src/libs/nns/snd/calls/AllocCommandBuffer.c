/* PS2: mechanically prepared copy of libs/nns/snd/calls/AllocCommandBuffer.c (ps2/tools/prep_sources.py). Do not edit. */


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
extern NNSFndList data_0204ad98;

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* AllocCommandBuffer -- NitroSystem sndarc_stream.c: AllocCommandBuffer. */
LoadCommand * AllocCommandBuffer (void)
{
    OSIntrMode old;
    LoadCommand * command;

    old = OS_DisableInterrupts();

    command = (LoadCommand *)NNS_FndGetNextListObject(&data_0204ad98, NULL);
    if (command != NULL) {
        NNS_FndRemoveListObject(&data_0204ad98, command);
    }

    (void)OS_RestoreInterrupts(old);

    return command;
}
