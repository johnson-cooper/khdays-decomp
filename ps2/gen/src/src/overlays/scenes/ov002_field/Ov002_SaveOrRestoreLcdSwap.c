/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_SaveOrRestoreLcdSwap.c (ps2/tools/prep_sources.py). Do not edit. */
/* Park the LCD swap bit, or put it back.
 *
 * POWCNT1 bit 15 chooses which screen the main engine drives. Asked to save, this remembers the
 * bit and clears it, but only if nothing is remembered yet, which the -1 sentinel marks. Asked to
 * restore, it merges the remembered bit back in and forgets it again.
 *
 * The register address is held in a pointer rather than cast at every use, which is how the
 * original keeps it live -- and it earns that twice, since the compiler builds the 0x8000 mask by
 * shifting the address 0x04000304 right by eleven, and the -1 sentinel by shifting the 0xffff7fff
 * mask right by sixteen. In the merge, the preserved bits go on the LEFT of the or; the other way
 * round costs two instructions.
 */

#include "nitro/types.h"

extern int data_ov002_0207f408;

void Ov002_SaveOrRestoreLcdSwap(int hide) {
    volatile u16 *reg304 = (volatile u16 *)((unsigned int)kh_ds_io + 0x304);

    if (hide != 0) {
        if (data_ov002_0207f408 < 0) {
            data_ov002_0207f408 = (*reg304 & 0x8000) >> 15;
            *reg304 = (u16)(*reg304 & ~0x8000);
        }
    } else {
        if (data_ov002_0207f408 >= 0) {
            *reg304 = (u16)((*reg304 & ~0x8000) | (data_ov002_0207f408 << 15));
            data_ov002_0207f408 = -1;
        }
    }
}
