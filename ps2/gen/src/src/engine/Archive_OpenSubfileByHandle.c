/* PS2: mechanically prepared copy of src/engine/Archive_OpenSubfileByHandle.c (ps2/tools/prep_sources.py). Do not edit. */
/* Opens a file inside a packed archive by its handle: the handle encodes the archive header's
 * address and the entry index, whose offset and size are read from the header's tables. */

#include "nitro/types.h"

typedef struct FSArchive FSArchive;
typedef struct FSFile FSFile;
typedef struct FSFileID FSFileID;

extern BOOL FS_OpenFileDirect(FSFile *file, FSArchive *archive, u32 image_top, u32 image_bot, FSFileID *id);

BOOL Archive_OpenSubfileByHandle(FSFile *file, u32 id)
{
    u32 mask = 0x00fffffc;
    u32 base = ((id >> 7) & mask) + KH_DS_PACKED_PTR_BASE;
    u16 idx  = (u16)(id & (mask >> 15));
    u8 *p = (u8 *)base;
    u32 top = *(u32 *)(p + 0xc) + ((u32)*(u16 *)(p + idx * 2 + 0x10) << 9);
    FSArchive *arc = *(FSArchive **)(p + 8);
    s32 half = (s32)((*(u16 *)(p + 2) & (mask >> 15)) + 1) / 2;
    u8 *q = p + (u32)((u16)(half * 2)) * 2;
    u32 raw = *(u32 *)(q + idx * 4 + 0x10) & 0x7fffffff;
    return FS_OpenFileDirect(file, arc, top, top + raw, (FSFileID *)0);
}
