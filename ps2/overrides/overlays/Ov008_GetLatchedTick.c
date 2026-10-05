/* PS2 override (ARM9 vs R5900 alignment): data_0204be1c is the 8-byte play-time start tick, but
 * the preserved DS .bss layout puts it at main .bss +0x7b7c - only 4-byte aligned.  GCC accesses a
 * global declared 'long long' with aligned 64-bit ld/sd, which raise an address error on the EE
 * (save crash: EE exception 4, BadV = &data_0204be1c, right after "card request complete").
 * prep_sources.py already makes the `*(long long *)&x` spellings safe (kh_unaligned_s64); this
 * spelling is not, so the access goes through kh_unaligned.h.  Same DS layout, same value.
 * Otherwise identical to the (prepared) original. */
#include "platform/kh_unaligned.h"
/* Return the 64-bit value stored at data_0204be1c. */

extern unsigned char data_0204be1c[];
long long Ov008_GetLatchedTick(void)
{
    return kh_read_s64_le_unaligned(data_0204be1c);
}
