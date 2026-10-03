/* PS2: mechanically prepared copy of src/overlays/enemies/ov237_enemy_crimson_prankster/data/ov237_pointers_020d19f8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov237 .rodata pointer tables, 0x020d19f8-0x020d1a20.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int gOv237RYubiName;
extern int gOv237LHandName;
extern int gOv237LYubiName;
extern int gOv237RHandName;
extern int gOv237LArm01Name;
extern int gOv237LArm02Name;
extern int gOv237RArm00Name;
extern int gOv237LArm00Name;
extern int gOv237RArm01Name;
extern int gOv237RArm02Name;

void *const data_ov237_020d19f8[10] __attribute__((aligned(__alignof__(void *)))) = {

    &gOv237LArm00Name,

    &gOv237LArm01Name,

    &gOv237LArm02Name,

    &gOv237LHandName,

    &gOv237LYubiName,

    &gOv237RArm00Name,

    &gOv237RArm01Name,

    &gOv237RArm02Name,

    &gOv237RHandName,

    &gOv237RYubiName,

};
