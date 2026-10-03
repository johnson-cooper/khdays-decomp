/* PS2: mechanically prepared copy of src/overlays/system/ov107_enemy_common/data/ov107_pointers_020cb5f8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov107 .rodata pointer tables, 0x020cb5f8-0x020cb610.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int data_ov108_020cbf40;
extern int data_ov109_020cdd60;
extern int data_ov110_020cfb80;
extern int data_ov111_020d19a0;
extern int data_ov112_020d37c0;
extern int data_ov113_020d55e0;

void *const data_ov107_020cb5f8[6] __attribute__((aligned(__alignof__(void *)))) = {

    &data_ov108_020cbf40,

    &data_ov109_020cdd60,

    &data_ov110_020cfb80,

    &data_ov111_020d19a0,

    &data_ov112_020d37c0,

    &data_ov113_020d55e0,

};
