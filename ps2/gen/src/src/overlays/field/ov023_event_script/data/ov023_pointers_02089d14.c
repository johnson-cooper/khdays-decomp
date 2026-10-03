/* PS2: mechanically prepared copy of src/overlays/field/ov023_event_script/data/ov023_pointers_02089d14.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov023 .rodata pointer tables, 0x02089d14-0x02089d74.
 *
 * 2 tables, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int gOv023AwName;
extern int gOv023NmName;
extern int gOv023PiName;
extern int gOv023TtName;
extern int gOv023BbName;
extern int gOv023PpName;
extern int gOv023AlName;
extern int gOv023HeName;

void *const data_ov023_02089d14[12] __attribute__((aligned(__alignof__(void *)))) = {

    &gOv023TtName,

    &gOv023AwName,

    &gOv023HeName,

    &gOv023AlName,

    0,

    &gOv023NmName,

    0,

    0,

    &gOv023PpName,

    &gOv023BbName,

    0,

    &gOv023PiName,

};

void *const data_ov023_02089d44[12] __attribute__((aligned(__alignof__(void *)))) = {

    &gOv023TtName,

    &gOv023AwName,

    &gOv023HeName,

    &gOv023AlName,

    0,

    &gOv023NmName,

    0,

    0,

    &gOv023PpName,

    &gOv023BbName,

    0,

    &gOv023PiName,

};
