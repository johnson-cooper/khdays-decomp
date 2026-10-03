/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/data/ov002_pointers_0207eed8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov002 .data pointer tables, 0x0207eed8-0x0207eee8.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov002_ClosePageIfAllowed(void);
extern void func_ov002_020673d4(void);
extern void Ov002_HandleHudPageKeys(void);

Ov_Fn data_ov002_0207eed8[4] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov002_HandleHudPageKeys,

    Ov002_ClosePageIfAllowed,

    func_ov002_020673d4,

    0,

};
