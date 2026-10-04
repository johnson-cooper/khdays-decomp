/* PS2 override: the original DS symbol data_0204be1c is an 8-byte tick value, but the
 * preserved DS BSS layout places it at +0x7b7c from main BSS -- only 4-byte aligned.
 *
 * A native EE 'unsigned long long' assignment lets GCC emit an aligned 64-bit store, which raises
 * AdES on real R5900 hardware at data_0204be1c (observed BadV 0x0076bd7c).  Preserve the exact DS
 * layout and write the low/high words separately, matching the ARM-side representation.
 */
#include "nitro/types.h"

extern unsigned long long OS_GetTick(void);
extern unsigned char data_0204be1c[];

void Ov000_LatchTick(void)
{
    unsigned long long t = OS_GetTick();
    volatile u32 *dst = (volatile u32 *)data_0204be1c;

    dst[0] = (u32)t;
    dst[1] = (u32)(t >> 32);
}
