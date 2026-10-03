/* PS2: mechanically prepared copy of src/overlays/scenes/ov011/data/ov011_tables_0205dc7c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov011 .rodata tables, 0x0205dc7c-0x0205dc90.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov011_StepPaneScroll (0205bb58): void data_ov011_0205dc7c(void); */

#include "nitro/types.h"

const u16 data_ov011_0205dc7c[3] __attribute__((aligned(__alignof__(u16)))) = {
    255, 65289, 0,
};

/* read by Ov011_DrawTitleLine (0205cda0): void data_ov011_0205dc82(void); */
const u16 data_ov011_0205dc82[7] __attribute__((aligned(__alignof__(u16)))) = {
    10, 22, 10, 255, 65280, 6144, 0,
};
