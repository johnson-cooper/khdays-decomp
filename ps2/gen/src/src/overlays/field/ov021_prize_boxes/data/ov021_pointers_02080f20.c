/* PS2: mechanically prepared copy of src/overlays/field/ov021_prize_boxes/data/ov021_pointers_02080f20.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov021 .data pointer tables, 0x02080f20-0x02080f40.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov021_VmCmdEntryList(void);
extern void Ov021_MarshalFxAndDispatch(void);
extern void Ov021_VmCmd0e30(void);
extern void Ov021_MarshalFxAndDispatch2(void);

Ov_Fn data_ov021_02080f20[8] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov021_VmCmdEntryList,

    0,

    Ov021_MarshalFxAndDispatch,

    0,

    Ov021_VmCmd0e30,

    0,

    Ov021_MarshalFxAndDispatch2,

    0,

};
