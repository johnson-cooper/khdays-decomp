/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_LayoutMissionEntries.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov008_LayoutMissionEntries -- Ov008_LayoutMissionEntries (752 B, 51 relocs).
 * Builds the mission/pause menu entry layout. Copies two 4-word template arrays (a tag list from
 * data_ov008_0208f648 and a transform seed from data_ov008_0208f658), fetches the layout object,
 * and runs a long FindEntryById + slot-op sequence: seeds the transform (Ov008_InitFromDescAndMark),
 * loads and processes blocks (0x28/0x29 tags), positions entry 0x2b by (0xf0000 - entry.f4),
 * copies two 8-byte glyph blocks out of ctx+0x20c/0x214, and toggles entry 0x80 visible. When a
 * session exists and is active it shows entry 2 and hides entry 1 (each with the c08/c80 pair).
 * It binds the resolve callbacks Ov008_PageB_SelectPrev/0206e848 to entries 0x2b/0x2c, and -- only
 * when REG_POWCNT1 bit 15 (display swap) is clear -- pushes their subitem sets. Finally it copies
 * four 8-byte blocks from ctx+0x21c.. into the entries named by the tag list.
 *
 * Session_IsActive (session-active) takes Session_Exists's result implicitly: the ROM leaves it in r0,
 * so both read as zero-arg calls here. */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct W4 { u32 a, b, c, d; } W4;
typedef struct Point { int x, y; } Point;

extern W4 data_ov008_0208f648;
extern W4 data_ov008_0208f658;
extern void Ov008_PageB_SelectPrev(void);
extern void Ov008_PageB_SelectNext(void);

extern int   Ov008_GetCtxBlock4a80(void);
extern int   Ov008_PackSlotTag(int tag);
extern void *Ov008_PackHandleTagB(int a);
extern void  Ov008_InitFromDescAndMark(int obj, u32 *tmpl);
extern void  func_ov008_0205475c(int obj, void *p);
extern void  Ov008_LoadBlockProcessAndFree(int obj, void *p, int n);
extern int   Ov008_FindEntryById(int obj, int id);
extern void  Ov008_ReleaseTwoSlotsEx_2(int obj, int entry, int n);
extern int   Ov008_GetEntryBlock2c(int obj, int entry);
extern void  Ov008_ApplyOffsetSum(int obj, int entry, Point *pt);
extern void  Ov008_ReleaseTwoSlots(int obj, int entry);
extern void  Ov008_ReleaseTwoSlotsEx(int obj, int entry, int n);
extern void  Ov008_SetEntrySlotsVisible(int obj, int entry, int n);
extern void  Ov008_PushSubitemSet(int obj, int entry, int n);
extern void  Ov008_ResolveEntryStoreWord(int obj, int id, void *fn);
extern void  MI_CpuCopy8(void *dst, void *src, int n);

void Ov008_LayoutMissionEntries(int ctx)
{
    int    local_28[4];
    u32    block2[4];
    Point  pt;
    int    obj, entry, block, i;

    *(W4 *)local_28 = data_ov008_0208f648;
    *(W4 *)block2 = data_ov008_0208f658;
    obj = Ov008_GetCtxBlock4a80();
    block2[0] = Ov008_PackSlotTag(0x28);
    Ov008_InitFromDescAndMark(obj, block2);
    func_ov008_0205475c(obj, Ov008_PackHandleTagB(2));
    Ov008_LoadBlockProcessAndFree(obj, (void *)Ov008_PackSlotTag(0x29), 0x42);
    Ov008_ReleaseTwoSlotsEx_2(obj, Ov008_FindEntryById(obj, 0x51), 3);

    entry = Ov008_FindEntryById(obj, 0x2b);
    block = Ov008_GetEntryBlock2c(obj, entry);
    pt.x = 0;
    pt.y = 0xf0000 - *(int *)(block + 4);
    Ov008_ApplyOffsetSum(obj, Ov008_FindEntryById(obj, 0x2b), &pt);

    entry = Ov008_FindEntryById(obj, 0x4a);
    Ov008_ReleaseTwoSlots(obj, entry);
    Ov008_ReleaseTwoSlotsEx(obj, entry, 3);

    entry = Ov008_FindEntryById(obj, 0x29);
    MI_CpuCopy8((void *)Ov008_GetEntryBlock2c(obj, entry), (void *)(ctx + 0x20c), 8);
    entry = Ov008_FindEntryById(obj, 0x51);
    MI_CpuCopy8((void *)Ov008_GetEntryBlock2c(obj, entry), (void *)(ctx + 0x214), 8);

    Ov008_SetEntrySlotsVisible(obj, Ov008_FindEntryById(obj, 0x80), 1);

    if (Session_Exists() != 0 && Session_IsActive() != 0) {
        Ov008_SetEntrySlotsVisible(obj, Ov008_FindEntryById(obj, 2), 1);
        Ov008_ReleaseTwoSlots(obj, Ov008_FindEntryById(obj, 2));
        Ov008_ReleaseTwoSlotsEx(obj, Ov008_FindEntryById(obj, 2), 0);
        Ov008_SetEntrySlotsVisible(obj, Ov008_FindEntryById(obj, 1), 0);
        Ov008_ReleaseTwoSlots(obj, Ov008_FindEntryById(obj, 1));
        Ov008_ReleaseTwoSlotsEx(obj, Ov008_FindEntryById(obj, 1), 0);
    }

    Ov008_ResolveEntryStoreWord(obj, 0x2b, (void *)Ov008_PageB_SelectPrev);
    Ov008_ResolveEntryStoreWord(obj, 0x2c, (void *)Ov008_PageB_SelectNext);

    if ((int)(*(u16 *)((unsigned int)kh_ds_io + 0x304) & 0x8000) >> 0xf == 0) {
        Ov008_PushSubitemSet(obj, Ov008_FindEntryById(obj, 0x2b), 1);
        Ov008_PushSubitemSet(obj, Ov008_FindEntryById(obj, 0x2c), 1);
    }

    i = 0;
    do {
        entry = Ov008_FindEntryById(obj, local_28[i]);
        MI_CpuCopy8((void *)Ov008_GetEntryBlock2c(obj, entry),
                    (void *)(ctx + 0x21c + i * 8), 8);
        i++;
    } while (i < 4);
}
