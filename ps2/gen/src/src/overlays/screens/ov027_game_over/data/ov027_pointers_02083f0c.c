/* PS2: mechanically prepared copy of src/overlays/screens/ov027_game_over/data/ov027_pointers_02083f0c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov027 .data pointer tables, 0x02083f0c-0x02083f24.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int gOv027JpUnusedName;
extern int data_ov027_02084104;
extern int gOv027StuckYouCanAlwaysText;
extern int gOv027BloccatoPerDeiSuggerimentiText;
extern int data_ov027_02084238;
extern int data_ov027_02084294;

void *data_ov027_02083f0c[6] __attribute__((aligned(__alignof__(void *)))) = {

    &gOv027JpUnusedName,

    &gOv027StuckYouCanAlwaysText,

    &data_ov027_02084238,

    &data_ov027_02084104,

    &gOv027BloccatoPerDeiSuggerimentiText,

    &data_ov027_02084294,

};
