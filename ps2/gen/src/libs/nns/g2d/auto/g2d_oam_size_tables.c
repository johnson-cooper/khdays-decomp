/* PS2: mechanically prepared copy of libs/nns/g2d/auto/g2d_oam_size_tables.c (ps2/tools/prep_sources.py). Do not edit. */
/* NitroSystem g2d (g2d_Oam_data.h, instantiated by g2d_CellAnimation.c): the OBJ height / width
 * tables indexed by [shape][size] (NNS_G2D_DEFINE_NNSI_OBJSIZEHTBL / ...WTBL). */

/* NNSi_objSizeHTbl */

#include "nitro/types.h"

const u16 data_020419c4[3][4] __attribute__((aligned(__alignof__(u16)))) = {
    {  8, 16, 32, 64 },
    {  8,  8, 16, 32 },
    { 16, 32, 32, 64 }
};

/* NNSi_objSizeWTbl */
const u16 data_020419dc[3][4] __attribute__((aligned(__alignof__(u16)))) = {
    {  8, 16, 32, 64 },
    { 16, 32, 32, 64 },
    {  8,  8, 16, 32 }
};
