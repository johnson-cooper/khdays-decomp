/* PS2: mechanically prepared copy of src/overlays/enemies/ov115_enemy_yellow_opera/data/ov115_tables_020ceb38.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov115 .rodata tables, 0x020ceb38-0x020ceb7c.
 *
 * 4 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov115_EnemyConstruct (020cc020): void data_ov115_020ceb38(void); */

#include "nitro/types.h"

const int data_ov115_020ceb38[7] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 4, 5, 6, 7, 3,
};

/* read by Ov115_DropStrikeTick (020cc9bc): void data_ov115_020ceb54(void); */
const u16 data_ov115_020ceb54[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov115_DropStrikeTick (020cc9bc): void data_ov115_020ceb62(void); */
const u16 data_ov115_020ceb62[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 517, 0, 0, 0, 0, 0,
};

/* read by * Hit handler of the ov115 enemy (and its byte-identical twins): copies the hit point into (020cce38): const u8 data_ov115_020ceb70[];
 *   Ov115_EngageEnter (020ce5dc): void data_ov115_020ceb70(void); */
const u8 data_ov115_020ceb70[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 1, 2, 3, 0, 0, 5, 0, 0, 0, 0, 0,
};
