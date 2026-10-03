/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/data/ov000_tables_0205a970.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov000 .rodata tables, 0x0205a970-0x0205a9b4.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov000_QueueResourceTransfers (02059d78): const void *data_ov000_0205a970[7]; */

#include "nitro/types.h"

const int data_ov000_0205a970[7] __attribute__((aligned(__alignof__(int)))) = {
    9, 10, 11, 24, 25, 26, 27,
};

/* read by Builds the scene's two tiled graphics surfaces. (0205906c): const TileSurfaceCfg data_ov000_0205a98c; */
const int data_ov000_0205a98c[10] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 32, 24, 0, 15, 0, 5,
    0, 32,
};
