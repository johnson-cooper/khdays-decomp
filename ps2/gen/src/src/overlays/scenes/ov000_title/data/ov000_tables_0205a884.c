/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/data/ov000_tables_0205a884.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov000 .rodata tables, 0x0205a884-0x0205a95c.
 *
 * 7 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov000_SetupLogoTileSurfaces (020535b4): const TileSurfaceCfg data_ov000_0205a884; */

#include "nitro/types.h"

const int data_ov000_0205a884[10] __attribute__((aligned(__alignof__(int)))) = {
    6, 8, 16, 8, 37, 15, 0, 23,
    0, 32,
};

/* read by Ov000_SetupLogoTileSurfaces (020535b4): const TileSurfaceCfg data_ov000_0205a8ac; */
const int data_ov000_0205a8ac[10] __attribute__((aligned(__alignof__(int)))) = {
    18, 1, 30, 6, 165, 15, 0, 23,
    0, 32,
};

/* read by Ov000_SetupLogoTileSurfaces (020535b4): const TileSurfaceCfg data_ov000_0205a8d4; */
const int data_ov000_0205a8d4[10] __attribute__((aligned(__alignof__(int)))) = {
    0, 14, 18, 2, 1, 15, 0, 23,
    0, 32,
};

/* read by For each variable-stride record, build a sprite via 0205657c (kind-mapped priority) and pl (02055ee4): unsigned char data_ov000_0205a8fc; */
const u8 data_ov000_0205a8fc[4] __attribute__((aligned(__alignof__(u8)))) = {
    8, 9, 10, 11,
};

/* read by For each variable-stride record, build a sprite via 0205657c (kind-mapped priority) and pl (02055ee4): unsigned char data_ov000_0205a900; */
const u8 data_ov000_0205a900[4] __attribute__((aligned(__alignof__(u8)))) = {
    24, 25, 26, 27,
};

/* read by Advance 02030788, then map the current 020315c0 slot to a priority table, storing its inde (020569dc): int data_ov000_0205a904; */
const int data_ov000_0205a904[20] __attribute__((aligned(__alignof__(int)))) = {
    10, 0, 16, 11, 14, 12, 1, 4,
    5, 6, 7, 9, 13, 15, 17, 18,
    8, 2, 3, 10,
};

/* read by Ov000_TickListSceneInput (0205a19c): const u8 data_ov000_0205a954[4]; */
const u8 data_ov000_0205a954[8] __attribute__((aligned(__alignof__(u8)))) = {
    224, 16, 16, 160, 19, 0, 0, 0,
};
