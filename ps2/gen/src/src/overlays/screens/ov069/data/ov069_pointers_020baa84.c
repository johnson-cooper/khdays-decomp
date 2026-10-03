/* PS2: mechanically prepared copy of src/overlays/screens/ov069/data/ov069_pointers_020baa84.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov069 .data pointer tables, 0x020baa84-0x020baab4.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov069_ScriptCmd_SetupBgLayer(void);
extern void Ov069_InitPairAndDispatch(void);
extern void Ov069_OpGiveItem(void);
extern void Ov069_OpTakeItem(void);
extern void Ov069_MarkCurrentItemEquipped(void);
extern void Ov069_ShiftGlobalHalfword(void);

Ov_Fn data_ov069_020baa84[12] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov069_ScriptCmd_SetupBgLayer,

    0,

    Ov069_InitPairAndDispatch,

    0,

    Ov069_OpGiveItem,

    0,

    Ov069_OpTakeItem,

    0,

    Ov069_MarkCurrentItemEquipped,

    0,

    Ov069_ShiftGlobalHalfword,

    0,

};
