/* PS2: mechanically prepared copy of src/overlays/field/ov019/data/ov019_pointers_0207fd60.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov019 .data pointer tables, 0x0207fd60-0x0207fd78.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov019_RecordStatHighWater(void);
extern void Ov019_ShowMessageWithCounters(void);
extern void Ov019_PollMenuFlow(void);
extern void Ov019_StoreSlotIndex(void);
extern void Ov019_AdvancePlayTime(void);

Ov_Fn data_ov019_0207fd60[6] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov019_RecordStatHighWater,

    0,

    Ov019_ShowMessageWithCounters,

    Ov019_PollMenuFlow,

    Ov019_StoreSlotIndex,

    Ov019_AdvancePlayTime,

};
