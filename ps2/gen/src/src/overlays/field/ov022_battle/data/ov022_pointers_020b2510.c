/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/data/ov022_pointers_020b2510.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov022 .rodata pointer tables, 0x020b2510-0x020b2520.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int gOv022BaEfIa3PackPath;
extern int gOv022BaEfIa2PackPath;
extern int gOv022BaEfIa1PackPath;
extern int gOv022BaEfIa0PackPath;

void *const data_ov022_020b2510[4] __attribute__((aligned(__alignof__(void *)))) = {

    &gOv022BaEfIa0PackPath,

    &gOv022BaEfIa1PackPath,

    &gOv022BaEfIa2PackPath,

    &gOv022BaEfIa3PackPath,

};
