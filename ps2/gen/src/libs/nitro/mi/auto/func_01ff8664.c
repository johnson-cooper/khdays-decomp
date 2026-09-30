/* PS2: mechanically prepared copy of libs/nitro/mi/auto/func_01ff8664.c (ps2/tools/prep_sources.py). Do not edit. */

#include "nitro/types.h"

void func_01ff8664(u32 channel, u32 source, u32 destination, u32 control)
{
    vu32 *registers = (vu32 *)(((unsigned int)kh_ds_io + 0xb0) + channel * 12);

    registers[0] = source;
    registers[1] = destination;
    registers[2] = control;

    (void)*(vu32 *)((unsigned int)kh_ds_io + 0xb0);
    (void)*(vu32 *)((unsigned int)kh_ds_io + 0xb0);

    if (channel == 0) {
        registers[0] = 0;
        registers[1] = 0;
        registers[2] = 0x81400001;
    }

    (void)*(vu32 *)((unsigned int)kh_ds_io + 0xb0);
    (void)*(vu32 *)((unsigned int)kh_ds_io + 0xb0);
}
