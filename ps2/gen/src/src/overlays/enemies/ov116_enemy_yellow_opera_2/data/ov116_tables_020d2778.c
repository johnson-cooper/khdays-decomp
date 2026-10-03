/* PS2: mechanically prepared copy of src/overlays/enemies/ov116_enemy_yellow_opera_2/data/ov116_tables_020d2778.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov116 .rodata tables, 0x020d2778-0x020d27bc.
 *
 * 4 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov116_EnemyConstruct (020cfc60): void data_ov116_020d2778(void); */

#include "nitro/types.h"

const int data_ov116_020d2778[7] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 4, 5, 6, 7, 3,
};

/* read by Ov116_DropStrikeTick (020d05fc): void data_ov116_020d2794(void); */
const u16 data_ov116_020d2794[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov116_DropStrikeTick (020d05fc): void data_ov116_020d27a2(void); */
const u16 data_ov116_020d27a2[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 517, 0, 0, 0, 0, 0,
};

/* read by * Hit handler of the ov115 enemy (and its byte-identical twins): copies the hit point into (020d0a78): const u8 data_ov116_020d27b0[];
 *   Ov116_EngageEnter (020d221c): void data_ov116_020d27b0(void); */
const u8 data_ov116_020d27b0[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 1, 2, 3, 0, 0, 5, 0, 0, 0, 0, 0,
};
