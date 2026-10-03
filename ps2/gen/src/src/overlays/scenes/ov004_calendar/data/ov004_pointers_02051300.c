/* PS2: mechanically prepared copy of src/overlays/scenes/ov004_calendar/data/ov004_pointers_02051300.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov004 .data pointer tables, 0x02051300-0x02051328.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int gOv004UiCal7APackPath;
extern int gOv004UiCal8APackPath;
extern int gOv004UiCal1APackPath;
extern int gOv004UiCal6APackPath;
extern int gOv004UiCal4APackPath;
extern int gOv004UiCal5APackPath;
extern int gOv004UiCal3APackPath;
extern int gOv004UiCal2APackPath;
extern int gOv004UiCal9APackPath;
extern int gOv004UiCal0APackPath;

void *data_ov004_02051300[10] __attribute__((aligned(__alignof__(void *)))) = {

    &gOv004UiCal0APackPath,

    &gOv004UiCal1APackPath,

    &gOv004UiCal2APackPath,

    &gOv004UiCal3APackPath,

    &gOv004UiCal4APackPath,

    &gOv004UiCal5APackPath,

    &gOv004UiCal6APackPath,

    &gOv004UiCal7APackPath,

    &gOv004UiCal8APackPath,

    &gOv004UiCal9APackPath,

};
