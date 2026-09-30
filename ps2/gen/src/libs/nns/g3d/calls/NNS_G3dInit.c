/* PS2: mechanically prepared copy of libs/nns/g3d/calls/NNS_G3dInit.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os.h"
#include "nitro/gx.h"

#define offsetof(type, member) ((u32)&(((type *)0)->member))

void G3X_Init(void);
static inline void G3X_SetFifoIntrCond (GXFifoIntrCond cond)
{
    (*( REGType32v *) (((unsigned int)kh_ds_io + 0x0) + 0x600)) = (((*( REGType32v *) (((unsigned int)kh_ds_io + 0x0) + 0x600)) & ~0xc0000000 ) |
                      (cond << 30 ));
}
void NNS_G3dGlbInit(void);

/* NNS_G3dInit -- NitroSystem util.c: NNS_G3dInit. */
void NNS_G3dInit (void)
{
    G3X_Init();

    NNS_G3dGlbInit();

    G3X_SetFifoIntrCond(GX_FIFOINTR_COND_EMPTY);
}
