/* PS2: mechanically prepared copy of libs/nitro/card/auto/card_rom_header_addr.c (ps2/tools/prep_sources.py). Do not edit. */
/* NitroSDK card_rom.c: cardi_rom_header_addr, where the card ROM header copy lives (HW_ROM_HEADER_BUF). */

#include "nitro/types.h"
#include "nitro/hw.h"

u32 data_020423e8 __attribute__((aligned(__alignof__(u32)))) = HW_ROM_HEADER_BUF;
