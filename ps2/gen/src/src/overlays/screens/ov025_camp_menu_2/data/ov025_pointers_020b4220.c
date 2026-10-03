/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_pointers_020b4220.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 .rodata pointer tables, 0x020b4220-0x020b4228.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int gOv025RptName;
extern int gOv025EnmName;

void *const data_ov025_020b4220[2] __attribute__((aligned(__alignof__(void *)))) = {

    &gOv025RptName,

    &gOv025EnmName,

};
