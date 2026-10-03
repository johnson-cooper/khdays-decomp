/* PS2: mechanically prepared copy of src/overlays/enemies/ov241_enemy_lumiere/data/ov241_pointers_020d0c84.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov241 .rodata pointer tables, 0x020d0c84-0x020d0c90.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int gOv241Tag02Name;
extern int gOv241Tag01Name;
extern int gOv241Tag00Name;

void *const data_ov241_020d0c84[3] __attribute__((aligned(__alignof__(void *)))) = {

    &gOv241Tag00Name,

    &gOv241Tag01Name,

    &gOv241Tag02Name,

};
