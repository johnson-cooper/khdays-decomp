/* PS2: mechanically prepared copy of src/overlays/field/ov023_event_script/Ov023_Teardown.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov023_Teardown -- ov023 teardown. In global mode 4 only: hand the screens back
 * (GX_SetGraphicsMode), map VRAM bank 8 to the LCDC, wipe 0xa4000 bytes of it from 0x06800000, and
 * release the sound mutex. Then drop the scene request (StoreToGlobalPtr4FieldE4IfSet(0)). */

#include "game/engine.h"

extern void GX_SetGraphicsMode(int a, int b, int c);
extern void GX_SetBankForLCDC(int bank);
extern void MIi_CpuClearFast(int value, void *dst, int size);
extern void GX_DisableBankForLCDC(void);

void Ov023_Teardown(void) {
    if (LoadGlobalU16At0() == 4) {
        GX_SetGraphicsMode(1, 0, 1);
        GX_SetBankForLCDC(8);
        MIi_CpuClearFast(0, (void *)((unsigned int)kh_ds_vram + 0x0), 0xa4000);
        GX_DisableBankForLCDC();
    }
    StoreToGlobalPtr4FieldE4IfSet(0);
}
