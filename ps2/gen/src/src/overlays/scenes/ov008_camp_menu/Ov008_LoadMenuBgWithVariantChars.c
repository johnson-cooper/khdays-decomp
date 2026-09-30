/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_LoadMenuBgWithVariantChars.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov008_LoadMenuBgWithVariantChars -- Ov008_LoadMenuBgWithVariantChars (300 B, 19 relocs).
 * Loads a menu background from a fixed archive descriptor (gOv008UiCmSavB000Path, passed to
 * Archive_LoadFile by address), uploads its BG palette, then selects the BG3 character source:
 * if Ov008_PackHandleTag(2) is non-zero it unpacks that alternate subfile, resolves its
 * character block (GetResourceSubBlock_CHAR2), flushes the data cache and uploads it; otherwise it uses
 * the cell's own character block. After freeing the temp resource it programs the BG2 scroll
 * registers (a raw write of 0x01e600e3 to 0x04000018 -- both operands are pool literals, no
 * relocation), attaches a second descriptor (gOv008UiCmSavePath) to the cell-list context via
 * Ov008_LoadLayoutResource, and registers cells for tags {0,1}. Resource-cell / character-block
 * layout matches Ov008_SetupMenuBgCells; Res_LoadSpriteSet takes five args. */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct Ov008CharacterBlock { u8 pad_0000[0x10]; u32 size; void *data; } Ov008CharacterBlock;
typedef struct Ov008PaletteBlock   { u8 pad_0000[0x08]; u32 size; void *data; } Ov008PaletteBlock;
typedef struct Ov008ScreenBlock    { u8 pad_0000[0x08]; u32 size; u8 data[1]; } Ov008ScreenBlock;
typedef struct Ov008ResourceCell {
    Ov008ScreenBlock    *screen;
    Ov008CharacterBlock *character;
    Ov008PaletteBlock   *palette;
} Ov008ResourceCell;

extern u8 gOv008UiCmSavB000Path[];
extern u8 gOv008UiCmSavePath[];
extern void *Ov008_GetCtxBlock9500(void);
extern u32   Ov008_PackHandleTag(int subfile);
extern void *Archive_LoadFile(u32 handle, int heapId);
extern void  GetResourceSubBlock_CHAR2(void *resource, Ov008CharacterBlock **block);
extern void  GX_LoadBGPltt(const void *source, u32 offset, u32 size);
extern void  GX_LoadBG3Char(const void *source, u32 offset, u32 size);
extern void  DC_FlushRange(const void *address, u32 size);
extern void  NNSi_FndFreeFromDefaultHeap(void *allocation);
extern void  Ov008_LoadLayoutResource(void *ctx, void *desc);
extern void *Ov008_FindEntryByTag(void *ctx, int tag);
extern void  Ov008_TagTracker_InvokeCallback(void *ctx, void *cell);

void Ov008_LoadMenuBgWithVariantChars(void)
{
    void *ctx;
    void *resource;
    void *alternate;
    u32 altHandle;
    Ov008ResourceCell cell;
    Ov008CharacterBlock *altBlock;

    ctx = Ov008_GetCtxBlock9500();
    resource = Archive_LoadFile((u32)gOv008UiCmSavB000Path, 0xe);
    Res_LoadSpriteSet(&cell, resource, 0, 0, 0);
    GX_LoadBGPltt(cell.palette->data, 0, cell.palette->size);

    altHandle = Ov008_PackHandleTag(2);
    if (altHandle != 0) {
        alternate = Archive_LoadFile(altHandle, 0xe);
        GetResourceSubBlock_CHAR2(alternate, &altBlock);
        DC_FlushRange(altBlock->data, altBlock->size);
        GX_LoadBG3Char(altBlock->data, 0, altBlock->size);
        if (alternate != 0) {
            NNSi_FndFreeFromDefaultHeap(alternate);
        }
    } else {
        GX_LoadBG3Char(cell.character->data, 0, cell.character->size);
    }
    if (resource != 0) {
        NNSi_FndFreeFromDefaultHeap(resource);
    }

    *(volatile u32 *)((unsigned int)kh_ds_io + 0x18) = 0x01e600e3;
    Ov008_LoadLayoutResource(ctx, gOv008UiCmSavePath);
    Ov008_TagTracker_InvokeCallback(ctx, Ov008_FindEntryByTag(ctx, 0));
    Ov008_TagTracker_InvokeCallback(ctx, Ov008_FindEntryByTag(ctx, 1));
}
