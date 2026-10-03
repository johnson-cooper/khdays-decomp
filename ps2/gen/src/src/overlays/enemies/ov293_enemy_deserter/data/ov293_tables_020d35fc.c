/* PS2: mechanically prepared copy of src/overlays/enemies/ov293_enemy_deserter/data/ov293_tables_020d35fc.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov293 .rodata tables, 0x020d35fc-0x020d3618.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov293 enemy: installs the handlers (+8 tick, +0xc draw, +0x1c message, (020d1a24): struct v2 data_ov293_020d35fc; */

#include "nitro/types.h"

const int data_ov293_020d35fc[2] __attribute__((aligned(__alignof__(int)))) = {
    2, 3,
};

/* read by Idle tick of the ov293 enemy: the +0x14 turn step is 30 x dt / 20; the +4 target is the (020d2cc8): const struct Msg4 data_ov293_020d3604; */
const u8 data_ov293_020d3604[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 1,
};

/* read by Swing tick of the ov293 enemy: the +0x14 turn step is 30 x dt / 10, the closest target (020d2e54): struct Ov293AreaMsg data_ov293_020d3608; */
const u8 data_ov293_020d3608[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
