/* PS2: mechanically prepared copy of src/overlays/enemies/ov121_enemy_soldier_2/data/ov121_tables_020cfd04.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov121 .rodata tables, 0x020cfd04-0x020cfd30.
 *
 * 4 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by ov121 actor initializer: install the callback table, seed the camera pose, (020cdde4): struct Ov120Vec3 data_ov121_020cfd04; */

#include "nitro/types.h"

const int data_ov121_020cfd04[3] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4,
};

/* read by * Ov121_AreaAttack_Broadcast -- Ov121_AreaAttack_Broadcast. (020cf11c): struct Ov120AreaOpener data_ov121_020cfd10; */
const u8 data_ov121_020cfd10[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 1,
};

/* read by * Ov121_AreaAttack_Broadcast -- Ov121_AreaAttack_Broadcast. (020cf11c): struct Ov120AreaMsg data_ov121_020cfd14; */
const u16 data_ov121_020cfd14[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by Ov121_SweepAttack_Step (020cf4d4): struct Ov120AreaMsg data_ov121_020cfd22; */
const u16 data_ov121_020cfd22[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};
