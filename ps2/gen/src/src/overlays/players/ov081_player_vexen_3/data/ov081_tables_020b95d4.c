/* PS2: mechanically prepared copy of src/overlays/players/ov081_player_vexen_3/data/ov081_tables_020b95d4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov081 .rodata tables, 0x020b95d4-0x020b961c.
 *
 * 1 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by * Attack step: the per-frame body of the ov042 enemy's attack state. (020b87b4): PermTable data_ov081_020b95d4; */

#include "nitro/types.h"

const int data_ov081_020b95d4[18] __attribute__((aligned(__alignof__(int)))) = {
    0, 1, 2, 0, 2, 1, 1, 0,
    2, 1, 2, 0, 2, 0, 1, 2,
    1, 0,
};
