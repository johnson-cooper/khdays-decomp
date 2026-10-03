/* PS2: mechanically prepared copy of libs/nns/g3d/auto/g3d_nsbca_pivot_table.c (ps2/tools/prep_sources.py). Do not edit. */
/* NitroSystem nsbca.c: pivotUtil_, the four off-pivot matrix cells written by a pivot-compressed
 * joint rotation (getRotData_ / getRotDataEx_), indexed by the pivot position 0-8. */

#include "nitro/types.h"

const u8 data_02041ae0[9][4] __attribute__((aligned(__alignof__(u8)))) = {
    {4, 5, 7, 8},
    {3, 5, 6, 8},
    {3, 4, 6, 7},

    {1, 2, 7, 8},
    {0, 2, 6, 8},
    {0, 1, 6, 7},

    {1, 2, 4, 5},
    {0, 2, 3, 5},
    {0, 1, 3, 4}
};
