/* PS2: mechanically prepared copy of src/overlays/scenes/ov012_opening/data/ov012_pointers_0205c2a4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov012 .data pointer tables, 0x0205c2a4-0x0205c2bc.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov012_LoadOpeningBgCharacters(void);
extern void Ov012_LoadOpeningBgScreen(void);
extern void Ov012_InvokeIndexedTableEntry(void);
extern void Ov012_BlendOpeningBgPlanes(void);
extern void Ov012_FadeOutOpeningBgPlanes(void);
extern void Ov012_OpeningStepReturnZero(void);

Ov_Fn data_ov012_0205c2a4[6] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov012_LoadOpeningBgCharacters,

    Ov012_LoadOpeningBgScreen,

    Ov012_InvokeIndexedTableEntry,

    Ov012_BlendOpeningBgPlanes,

    Ov012_FadeOutOpeningBgPlanes,

    Ov012_OpeningStepReturnZero,

};
