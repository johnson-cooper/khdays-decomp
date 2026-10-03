/* PS2: mechanically prepared copy of src/overlays/system/ov107_enemy_common/data/ov107_tables_020cb64c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov107 .rodata tables, 0x020cb64c-0x020cb68c.
 *
 * 5 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov107_AiState_ApplyHit (020c5cfc): const struct Message12 data_ov107_020cb64c; */

#include "nitro/types.h"

const u8 data_ov107_020cb64c[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 8, 7, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* read by * Throttles creature updates by distance and update phase, advances timers and (020c6980): const VecFx32 data_ov107_020cb658; */
const u8 data_ov107_020cb658[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 128, 254, 255, 255, 0, 0, 0, 0,
};

/* read by Collect sphere contacts against polygon fans, then visit child spatial groups. (020c9f64): const ChildOffset data_ov107_020cb664[4];
 *   Ov107_CollectShapeContacts (020ca4b4): const ChildOffset data_ov107_020cb664[4]; */
const u8 data_ov107_020cb664[16] __attribute__((aligned(__alignof__(u8)))) = {
    255, 255, 255, 255, 1, 0, 255, 255, 255, 255, 1, 0, 1, 0, 1, 0,
};

/* read by Ov107_InvokeHitCallback (020ca918): const struct HitMsg data_ov107_020cb674; */
const u8 data_ov107_020cb674[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 8, 6, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* read by Ov107_InvokeHitCallback (020ca918): const struct HitMsg data_ov107_020cb680; */
const u8 data_ov107_020cb680[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 8, 5, 0, 0, 0, 0, 0, 0, 0, 0,
};
