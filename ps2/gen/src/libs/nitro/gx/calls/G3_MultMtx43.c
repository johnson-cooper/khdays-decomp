/* PS2: mechanically prepared copy of libs/nitro/gx/calls/G3_MultMtx43.c (ps2/tools/prep_sources.py). Do not edit. */
/* Geometry command 0x19 (MTX_MULT_4x3) followed by the matrix's 48 bytes into the geometry FIFO. */
#include "nitro/fx.h"

extern void GX_SendFifo48B(const void *src, void *dst);

void G3_MultMtx43(const MtxFx43 *m) {kh_ge_port_write1(0x400, (unsigned int)(0x19));
    GX_SendFifo48B(m, (void *)((unsigned int)kh_ds_io + 0x400));
}
