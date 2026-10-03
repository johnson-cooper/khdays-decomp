/* PS2: mechanically prepared copy of src/overlays/field/ov023_event_script/data/ov023_pointers_0208a70c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov023 .data pointer tables, 0x0208a70c-0x0208a730.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int gOv023RoDash00Name;
extern int gOv023RoWalk01Name;
extern int gOv023RoWalk00Name;
extern int gOv02313Dash01Name;
extern int gOv02313Dash00Name;
extern int gOv02313Walk01Name;
extern int gOv02313Walk00Name;
extern int gOv023RoDash01Name;
extern int gOv02313Hakusyu00Name;

void *data_ov023_0208a70c[9] __attribute__((aligned(__alignof__(void *)))) = {

    &gOv023RoWalk00Name,

    &gOv023RoWalk01Name,

    &gOv023RoDash00Name,

    &gOv023RoDash01Name,

    &gOv02313Walk00Name,

    &gOv02313Walk01Name,

    &gOv02313Dash00Name,

    &gOv02313Dash01Name,

    &gOv02313Hakusyu00Name,

};
