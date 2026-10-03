/* PS2: mechanically prepared copy of src/overlays/enemies/ov242_enemy_lumiere_2/data/ov242_pointers_020d48c4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov242 .rodata pointer tables, 0x020d48c4-0x020d48d0.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int gOv242Tag02Name;
extern int gOv242Tag01Name;
extern int gOv242Tag00Name;

void *const data_ov242_020d48c4[3] __attribute__((aligned(__alignof__(void *)))) = {

    &gOv242Tag00Name,

    &gOv242Tag01Name,

    &gOv242Tag02Name,

};
