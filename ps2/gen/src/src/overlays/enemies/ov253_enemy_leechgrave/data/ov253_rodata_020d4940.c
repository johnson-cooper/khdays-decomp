/* PS2: mechanically prepared copy of src/overlays/enemies/ov253_enemy_leechgrave/data/ov253_rodata_020d4940.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov253 .rodata pointer tables, 0x020d4940-0x020d4950.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int gOv253Bone05Name;
extern int gOv253Bone03Name;
extern int gOv253Bone02Name;
extern int gOv253Bone01Name;

void *const data_ov253_020d4940[4] __attribute__((aligned(__alignof__(void *)))) = {

    &gOv253Bone01Name,

    &gOv253Bone02Name,

    &gOv253Bone03Name,

    &gOv253Bone05Name,

};
