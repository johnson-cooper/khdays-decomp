/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/data/ov000_tables_0205a6b0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov000 .rodata tables, 0x0205a6b0-0x0205a6d0.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov000_StepMenuSelection (0204d244): const OverlayCountTable data_ov000_0205a6b0; */

#include "nitro/types.h"

const int data_ov000_0205a6b0[3] __attribute__((aligned(__alignof__(int)))) = {
    2, 2, 2,
};

/* read by Ov000_LayoutSelectionPages (0204fdac): const int data_ov000_0205a6bc[2];
 *   Ov000_UpdateMenuMarkers (0205042c): const int data_ov000_0205a6bc[2];
 *   park the two selection markers on their side of the screen. (02051b98): const int data_ov000_0205a6bc[2];
 *   Ov000_TickSelectionScene (02052124): const int data_ov000_0205a6bc[2]; */
const int data_ov000_0205a6bc[2] __attribute__((aligned(__alignof__(int)))) = {
    20, 21,
};

/* read by flush dirty logo sub-layers to VRAM, ov000. For each of the 3 (02052fdc): int  data_ov000_0205a6c4[]; */
const int data_ov000_0205a6c4[3] __attribute__((aligned(__alignof__(int)))) = {
    24, 25, 26,
};
