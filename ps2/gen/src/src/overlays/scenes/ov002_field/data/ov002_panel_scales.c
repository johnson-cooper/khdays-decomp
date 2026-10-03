/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/data/ov002_panel_scales.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov002 panel field scales, 0x0207dcc0-0x0207dd14.
 *
 * Twenty-one Q12 values read by Ov002_GetPanelField0134, running about 0.96 to 2.37
 * with 8192 meaning exactly 2.0. The last five are zero and unused.
 */

#include "nitro/fx_types.h"

const fx32 data_ov002_0207dcc0[21] __attribute__((aligned(__alignof__(fx32)))) = {
     8888,  7987,  9380,  9011,  9708,  7905,  8192,
     8806,  8233,  8192,  8438,  7455,  6758,  6308,
     3932,  7086,     0,     0,     0,     0,     0,
};
