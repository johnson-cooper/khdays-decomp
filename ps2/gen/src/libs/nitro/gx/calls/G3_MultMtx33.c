/* PS2: mechanically prepared copy of libs/nitro/gx/calls/G3_MultMtx33.c (ps2/tools/prep_sources.py). Do not edit. */
/* Geometry command 0x1a (MTX_MULT_3x3) followed by the matrix's 36 bytes into the geometry FIFO. */
#include "nitro/fx.h"

extern void MI_Copy36B(const void *src, void *dst);

void G3_MultMtx33(const MtxFx33 *m) {kh_ge_port_write1(0x400, (unsigned int)(0x1a));
    MI_Copy36B(m, (void *)((unsigned int)kh_ds_io + 0x400));
}
