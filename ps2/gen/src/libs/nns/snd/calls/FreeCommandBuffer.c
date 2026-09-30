/* PS2: mechanically prepared copy of libs/nns/snd/calls/FreeCommandBuffer.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

extern OSIntrMode OS_DisableInterrupts(void);
extern OSIntrMode OS_RestoreInterrupts(OSIntrMode state);
void NNS_FndAppendListObject(NNSFndList * list, void * object);
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

/* FreeCommandBuffer -- NitroSystem sndarc_stream.c: FreeCommandBuffer. */
void FreeCommandBuffer (LoadCommand * command)
{
    OSIntrMode old;

    old = OS_DisableInterrupts();

    NNS_FndAppendListObject(&data_0204ad98, command);

    (void)OS_RestoreInterrupts(old);
}
