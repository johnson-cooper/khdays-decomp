/* PS2: mechanically prepared copy of libs/nns/snd/calls/NNS_SndArcReadFile.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nnsys/snd.h"

#define offsetof(type, member) ((u32)&(((type *)0)->member))

s32 FS_ReadFile(FSFile * p_file, void * dst, s32 len);
BOOL FS_SeekFile(FSFile * p_file, s32 offset, FSSeekFileMode origin);

/* khdays: shared-bss */
NNSSndArc * data_0204ad4c;   /* data_0204ad4c */

/* NNS_SndArcReadFile -- NitroSystem sndarc.c: NNS_SndArcReadFile. The game's build waits for the card
 * thread (FSi_WaitForCardThread) before every FS_ReadFile of a block. */
s32 NNS_SndArcReadFile (u32 fileId, void * buffer, s32 size, s32 offset)
{
#ifndef SDK_SMALL_BUILD

    NNSSndArc * arc = data_0204ad4c;
    const NNSSndArcFileInfo * file;
    s32 totalReadSize;
    s32 readSize;
    s32 blockSize;
    s32 currentOffset;
    s32 requestSize;
    u8 * destAddress;

    if (fileId >= arc->fat->count) return -1;
    file = &arc->fat->files[ fileId ];

    currentOffset = offset;

    blockSize = arc->loadBlockSize;
    if (blockSize == 0) {
        blockSize = size;
    }
    totalReadSize = 0;
    destAddress = (u8 *)buffer;

    while (totalReadSize < size) {
        requestSize = size - totalReadSize;
        if (requestSize > blockSize) requestSize = blockSize;
        if (requestSize > file->size - currentOffset) {
            requestSize = (s32)(file->size - currentOffset);
        }
        if (requestSize == 0) {
            break;
        }

        if (!FS_SeekFile(&arc->file, (s32)(file->offset + currentOffset), FS_SEEK_SET)) {
            return -1;
        }
        FSi_WaitForCardThread();
        readSize = FS_ReadFile(&arc->file, destAddress, requestSize);
        if (readSize < 0) return readSize;

        totalReadSize += readSize;
        currentOffset += readSize;
        destAddress += readSize;
    }

    return totalReadSize;

#else

    return -1;

#endif
}
