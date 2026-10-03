/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/data/ov022_pointers_020b2880.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov022 .data pointer tables, 0x020b2880-0x020b2894.
 *
 * 2 tables, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int data_ov002_0207e720;
extern int data_ov022_020b28d4;
extern int data_ov022_020b28f8;

void *data_ov022_020b2880[2] __attribute__((aligned(__alignof__(void *)))) = {

    &data_ov022_020b28d4,

    &data_ov002_0207e720,

};

void *data_ov022_020b2888[3] __attribute__((aligned(__alignof__(void *)))) = {

    &data_ov022_020b28d4,

    &data_ov002_0207e720,

    &data_ov022_020b28f8,

};
