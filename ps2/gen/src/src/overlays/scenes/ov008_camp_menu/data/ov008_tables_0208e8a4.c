/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_tables_0208e8a4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 .rodata tables, 0x0208e8a4-0x0208e8fc.
 *
 * 3 tables before the layout templates 02058df0.c owns (0x0208e8fc-0x0208e958),
 * each written in the width its contents are in: words where the values are
 * small integers, bytes where the words are packed bytes.
 *
 * Readers:
 *   data_ov008_0208e8a4: Ov008_LookupTypeCode
 *   data_ov008_0208e8f4: Ov008_LoadElemsFromLayout
 *   data_ov008_0208e8f8: Ov008_LoadElemsFromLayout
 */

#include "nitro/types.h"

const int data_ov008_0208e8a4[20] __attribute__((aligned(__alignof__(int)))) = {
    10, 0, 16, 11, 14, 12, 1, 4,
    5, 6, 7, 9, 13, 15, 17, 18,
    8, 2, 3, 10,
};

const u8 data_ov008_0208e8f4[4] __attribute__((aligned(__alignof__(u8)))) = {
    8, 9, 10, 11,
};

const u8 data_ov008_0208e8f8[4] __attribute__((aligned(__alignof__(u8)))) = {
    24, 25, 26, 27,
};
