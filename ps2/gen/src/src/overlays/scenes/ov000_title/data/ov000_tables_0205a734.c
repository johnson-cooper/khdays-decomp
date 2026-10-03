/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/data/ov000_tables_0205a734.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov000 .rodata tables, 0x0205a734-0x0205a858.
 *
 * 7 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Scene 1: build the three text surfaces of the page. (020500d0): const TileSurfaceCfg data_ov000_0205a734; */

#include "nitro/types.h"

const int data_ov000_0205a734[10] __attribute__((aligned(__alignof__(int)))) = {
    0, 14, 18, 2, 97, 15, 0, 23,
    0, 32,
};

/* read by Scene 1: build the three text surfaces of the page. (020500d0): const TileSurfaceCfg data_ov000_0205a75c; */
const int data_ov000_0205a75c[10] __attribute__((aligned(__alignof__(int)))) = {
    4, 0, 32, 13, 133, 15, 0, 23,
    0, 32,
};

/* read by Scene 1: build the three text surfaces of the page. (020500d0): const TileSurfaceCfg data_ov000_0205a784; */
const int data_ov000_0205a784[10] __attribute__((aligned(__alignof__(int)))) = {
    17, 0, 32, 6, 549, 15, 0, 23,
    0, 32,
};

/* read by Ov000_LayoutSelectionPages (0204fdac): const int data_ov000_0205a7ac[4][8];
 *   Ov000_RefreshSelectionGroupDraw (020506d0): Ov000EntryIdGrid data_ov000_0205a7ac;
 *   Page-scroll tick for the ov000 title/menu stack: eases each of the four selection (02050ec4): const int data_ov000_0205a7ac[4][8];
 *   Ov000_UpdateNumberDisplays (02051470): const NumberDisplayConfig data_ov000_0205a7ac[3]; */
const int data_ov000_0205a7ac[32] __attribute__((aligned(__alignof__(int)))) = {
    1, 31, 34, 37, 40, 50, 51, 52,
    2, 32, 35, 38, 41, 53, 54, 55,
    3, 33, 36, 39, 42, 56, 57, 58,
    4, 0, 0, 0, 0, 0, 0, 0,
};

/* read by Ov000_ModeSelect_ShowGroup (02053b0c): EntryIdGroup3 data_ov000_0205a82c; */
const int data_ov000_0205a82c[3] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4,
};

/* read by flush dirty logo BG layers to VRAM, ov000. For each of the 4 (02054668): int  data_ov000_0205a838[]; */
const int data_ov000_0205a838[4] __attribute__((aligned(__alignof__(int)))) = {
    24, 25, 26, 27,
};

/* read by Ov000_ModeSelect_ShowGroup (02053b0c): EntryIdGroup4 data_ov000_0205a848; */
const int data_ov000_0205a848[4] __attribute__((aligned(__alignof__(int)))) = {
    5, 6, 7, 8,
};
