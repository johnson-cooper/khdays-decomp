/* PS2: mechanically prepared copy of src/engine/data/main_pointers_020427d4.c (ps2/tools/prep_sources.py). Do not edit. */
/* main .data pointer tables, 0x020427d4-0x020427f0.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern int gZhName;
extern int gEnName;
extern int gItName;
extern int gDeName;
extern int gFrName;
extern int gEsName;
extern int gJaName;

void *data_020427d4[7] __attribute__((aligned(__alignof__(void *)))) = {

    &gJaName,

    &gEnName,

    &gFrName,

    &gDeName,

    &gItName,

    &gEsName,

    &gZhName,

};
