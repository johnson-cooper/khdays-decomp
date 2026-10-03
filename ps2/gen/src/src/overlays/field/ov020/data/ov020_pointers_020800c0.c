/* PS2: mechanically prepared copy of src/overlays/field/ov020/data/ov020_pointers_020800c0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov020 .data pointer tables, 0x020800c0-0x020800d8.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov020_InitTripleAndDispatch(void);
extern void Ov020_MarshalAndDispatch(void);
extern void Ov020_ScriptOpPostScoreAndClearBgPriority(void);

Ov_Fn data_ov020_020800c0[6] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov020_InitTripleAndDispatch,

    0,

    Ov020_MarshalAndDispatch,

    0,

    Ov020_ScriptOpPostScoreAndClearBgPriority,

    0,

};
