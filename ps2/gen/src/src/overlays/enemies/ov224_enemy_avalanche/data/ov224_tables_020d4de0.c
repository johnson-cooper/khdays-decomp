/* PS2: mechanically prepared copy of src/overlays/enemies/ov224_enemy_avalanche/data/ov224_tables_020d4de0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov224 .rodata, 0x020d4de0-0x020d4e18: the enemy's sub-item poses, its hit reaction
 * modes, four tuning constants and the kind of the saved sub-item.
 *
 * Readers: 020d4048 (Ov224_Construct) copies the saved kind and builds the sub-item from
 * it; 020d0710 (Ov224_HandleHit) picks the reaction mode from the idle/hurt pair by the
 * work's +0x77 side; 020cfdbc copies the eight poses to its frame and builds one sub-item per
 * non-negative entry. */

#include "nitro/types.h"

struct Ov224ModePair { u8 nFirst; u8 nSecond; };
struct Ov224ReactionModes { struct Ov224ModePair idle; struct Ov224ModePair hurt; };
struct Ov224Saved { int w; };

/* pose of each of the eight sub-items; -1 leaves the slot empty */
const int data_ov224_020d4de0[8] __attribute__((aligned(__alignof__(int)))) = { 25, 28, 29, 30, 31, -1, 34, 33 };

/* reaction 0x14a modes: idle side 0/1, hurt side 0/1 */
const struct Ov224ReactionModes data_ov224_020d4e00 __attribute__((aligned(__alignof__(struct Ov224ReactionModes)))) = { { 0, 1 }, { 2, 3 } };

/* tuning constants (fx32): 9.0, 0.873, 3.195, 8.520 */
const int data_ov224_020d4e04[4] __attribute__((aligned(__alignof__(int)))) = { 0x9000, 0xdf6, 0x331f, 0x8852 };

/* pose of the saved sub-item */
const struct Ov224Saved data_ov224_020d4e14 __attribute__((aligned(__alignof__(struct Ov224Saved)))) = { 0x1b };
