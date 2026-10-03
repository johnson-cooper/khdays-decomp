/* PS2: mechanically prepared copy of src/overlays/enemies/ov245_enemy_infernal_engine/data/ov245_tables_020d7198.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov245 .rodata tables, 0x020d7198-0x020d71c4.
 *
 * 4 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

#include "nitro/types.h"

const u8 data_ov245_020d7198[12] __attribute__((aligned(__alignof__(u8)))) = {
    15, 228, 0, 0, 202, 2, 0, 0, 0, 128, 254, 255,
};

/* read by Ov245_StockHitFilter (Ov245_StockHitFilter): reaction kinds by parity */
const u8 data_ov245_020d71a4[8] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1, 0, 4, 0, 0,
};

/* read by Ov245_HopperConstruct (Ov245_HopperConstruct): pool entries of the three slot items */
const u8 data_ov245_020d71ac[4] __attribute__((aligned(__alignof__(u8)))) = {
    28, 27, 37, 0,
};

/* read by constructor of the ov245 rider: installs the handlers (+8 tick, +0xc (020d5538): const struct PoolIds data_ov245_020d71b0; */
const int data_ov245_020d71b0[5] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 51, 52, 53,
};
