/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_limits_020b37ac.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 header limits data_ov025_020b37ac, 0x020b37ac-0x020b37b0 (.rodata): the
 * width / height pair (12 x 2) copied by Ov025_SetupContext 02083e84.
 */

#include "nitro/types.h"

typedef struct Ov008HeaderLimits {
    u16 width;                /* 0x00 */
    u16 height;               /* 0x02 */
} Ov008HeaderLimits;

const Ov008HeaderLimits data_ov025_020b37ac __attribute__((aligned(__alignof__(Ov008HeaderLimits)))) = { 12, 2 };
