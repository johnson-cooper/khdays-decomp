/* PS2: mechanically prepared copy of src/overlays/scenes/ov005_mission_result/data/ov005_pointers_0205b79c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov005 .data pointer tables, 0x0205b79c-0x0205b7a8.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int data_ov005_0205b7a8;
extern int data_ov005_0205b7b8;
extern int data_ov005_0205b7e0;

void *data_ov005_0205b79c[3] __attribute__((aligned(__alignof__(void *)))) = {

    &data_ov005_0205b7b8,

    &data_ov005_0205b7a8,

    &data_ov005_0205b7e0,

};
