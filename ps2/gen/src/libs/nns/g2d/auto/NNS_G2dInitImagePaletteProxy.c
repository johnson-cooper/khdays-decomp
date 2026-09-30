/* PS2: mechanically prepared copy of libs/nns/g2d/auto/NNS_G2dInitImagePaletteProxy.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"

static inline void InitializeVRamLocation_ (NNSG2dVRamLocation * pVramLocation)
{
    int i;
    for (i = 0; i < NNS_G2D_VRAM_TYPE_MAX; i++) {
        pVramLocation->baseAddrOfVram[ i ] = 0xFFFFFFFF ;
    }
}

/* NNS_G2dInitImagePaletteProxy -- NitroSystem g2d_Image.c: NNS_G2dInitImagePaletteProxy. */
void NNS_G2dInitImagePaletteProxy (NNSG2dImagePaletteProxy * pImg)
{
    InitializeVRamLocation_(&pImg->vramLocation);
}
