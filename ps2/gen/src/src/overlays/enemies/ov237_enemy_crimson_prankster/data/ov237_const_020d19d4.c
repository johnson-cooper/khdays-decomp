/* PS2: mechanically prepared copy of src/overlays/enemies/ov237_enemy_crimson_prankster/data/ov237_const_020d19d4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov237 .rodata 0x020d19d4-0x020d19f8: three points 0xa00 ahead on x forming an equilateral
 * triangle of radius 0x500 in the y/z plane (at 90, -30 and 210 degrees). No code references
 * it: an initializer whose copy the compiler dropped. */

#include "nitro/fx_types.h"

const VecFx32 data_ov237_020d19d4[3] __attribute__((aligned(__alignof__(VecFx32)))) = {
    { 0xa00, 0, 0x500 },
    { 0xa00, 0x454, -0x280 },
    { 0xa00, -0x454, -0x280 },
};
