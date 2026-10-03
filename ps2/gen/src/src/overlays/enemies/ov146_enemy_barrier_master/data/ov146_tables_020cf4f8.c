/* PS2: mechanically prepared copy of src/overlays/enemies/ov146_enemy_barrier_master/data/ov146_tables_020cf4f8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov146 .rodata 0x020cf4f8-0x020cf514: the two initializer templates of the ov146 actor. */

#include "nitro/types.h"

typedef struct PartIds {
    int id[5];
} PartIds;

/* Ids of the hidden parts the constructor (Ov146_Actor_Construct, Ov146_Actor_Construct) attaches. */
const PartIds data_ov146_020cf4f8 __attribute__((aligned(__alignof__(PartIds)))) = { { 0, 3, 0, 0x16, 0x17 } };

/* Reaction mode pairs of the hit filter (Ov146_OnDamage): {2, 3} while hurt, {0, 1}
 * otherwise, followed by the word 0x60 of an initializer whose copy the compiler dropped. */
const u8 data_ov146_020cf50c[8] __attribute__((aligned(__alignof__(u8)))) = { 2, 3, 0, 1, 0x60, 0, 0, 0 };
