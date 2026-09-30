/* PS2: mechanically prepared copy of libs/nns/snd/calls/OpenMemoryStream.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

void MI_CpuCopy8(const void * src, void * dest, u32 size);
void * NNS_SndArcGetFileAddress(u32 fileId);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* OpenMemoryStream -- NitroSystem sndarc_stream.c: OpenMemoryStream. */
BOOL OpenMemoryStream (NNSSndStrmPlayer * player, u32 fileId)
{
    player->fileOffset = (u32)NNS_SndArcGetFileAddress(fileId);

    MI_CpuCopy8(
        (const void *)(player->fileOffset),
        &player->info,
        sizeof(player->info)
        );

    return TRUE;
}
