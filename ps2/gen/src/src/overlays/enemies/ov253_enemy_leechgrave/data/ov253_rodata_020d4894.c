/* PS2: mechanically prepared copy of src/overlays/enemies/ov253_enemy_leechgrave/data/ov253_rodata_020d4894.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov253 .rodata tables, 0x020d4894-0x020d4940.
 *
 * 5 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov253_RebuildCarriedLists (not yet decompiled) */

#include "nitro/types.h"

const int data_ov253_020d4894[13] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 3, 4, 5, 6, 7, 8,
    9, 10, 11, 12, 13,
};

/* read by Ov253_RebuildCarriedLists (not yet decompiled) */
const int data_ov253_020d48c8[13] __attribute__((aligned(__alignof__(int)))) = {
    15, 16, 17, 18, 19, 20, 21, 22,
    23, 24, 25, 26, 27,
};

/* read by recover entry: sends message data_ov253_020d48fc (kind 4) to the (020cd9e8): const struct hpair data_ov253_020d48fc; */
const u8 data_ov253_020d48fc[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 1,
};

/* read by Ov253_TickSpin (not yet decompiled) */
const u8 data_ov253_020d4900[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* read by facing tick: the +0x30 delay runs down while non-negative; the +0xc (020cd484): const struct Ov253Axes data_ov253_020d4910; */
const u8 data_ov253_020d4910[48] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 16, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 240, 255, 255, 0, 240, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0,
};
