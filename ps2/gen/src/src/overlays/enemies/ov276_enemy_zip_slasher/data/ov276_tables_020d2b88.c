/* PS2: mechanically prepared copy of src/overlays/enemies/ov276_enemy_zip_slasher/data/ov276_tables_020d2b88.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov276 .rodata tables, 0x020d2b88-0x020d2c04.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov276_EnemyConstruct (020cfcf8): void data_ov276_020d2b88(void); */

#include "nitro/types.h"

const int data_ov276_020d2b88[6] __attribute__((aligned(__alignof__(int)))) = {
    0, 24, 25, 26, 27, 28,
};

/* read by Rebuild the +0x384 work list for the actor at *(+0x3a8): copy the const pose table to (020d044c): const struct Buf_020ce7fc data_ov276_020d2ba0; */
const int data_ov276_020d2ba0[22] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 3, 4, 5, 6, 7, 8,
    9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22,
};

/* read by Hit handler of the ov276 enemy: ignored while the +0x21a stamina is spent. With a source (020d0560): const struct ModeTable data_ov276_020d2bf8; */
const u8 data_ov276_020d2bf8[12] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0,
};
