/* PS2: mechanically prepared copy of libs/nns/snd/calls/ReadMemoryStream.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

void MI_CpuCopy8(const void * src, void * dest, u32 size);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* ReadMemoryStream -- NitroSystem sndarc_stream.c: ReadMemoryStream. */
s32 ReadMemoryStream (NNSSndStrmPlayer * player, void * dest, u32 size, u32 offset)
{

    const u8 * src = (const u8 *)(player->fileOffset);

    MI_CpuCopy8(src + offset, dest, size);

    return (s32)size;
}
