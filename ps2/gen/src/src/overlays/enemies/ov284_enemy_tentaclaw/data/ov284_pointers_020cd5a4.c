/* PS2: mechanically prepared copy of src/overlays/enemies/ov284_enemy_tentaclaw/data/ov284_pointers_020cd5a4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov284 .rodata pointer tables, 0x020cd5a4-0x020cd5b4.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int gOv284Bone05Name;
extern int gOv284Bone03Name;
extern int gOv284Bone02Name;
extern int gOv284Bone01Name;

void *const data_ov284_020cd5a4[4] __attribute__((aligned(__alignof__(void *)))) = {

    &gOv284Bone01Name,

    &gOv284Bone02Name,

    &gOv284Bone03Name,

    &gOv284Bone05Name,

};
