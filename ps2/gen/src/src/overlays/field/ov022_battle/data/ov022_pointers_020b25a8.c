/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/data/ov022_pointers_020b25a8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov022 .rodata pointer tables, 0x020b25a8-0x020b25b4.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int gOv022BaEfSBurnPackPath;
extern int gOv022BaEfSShockPackPath;
extern int gOv022BaEfSFrostPackPath;

void *const data_ov022_020b25a8[3] __attribute__((aligned(__alignof__(void *)))) = {

    &gOv022BaEfSBurnPackPath,

    &gOv022BaEfSShockPackPath,

    &gOv022BaEfSFrostPackPath,

};
