/* PS2: mechanically prepared copy of libs/nitro/gx/calls/GX_SetBankForTexPltt.c (ps2/tools/prep_sources.py). Do not edit. */
/* NitroSDK gx (gx_vramcnt.c): GX_SetBankForTexPltt -- returns the old texPltt banks to LCDC and maps the new ones (VRAMCNT E/F/G). */

#include "nitro/types.h"

extern void GX_VRAMCNT_SetLCDC_(u32 bankBits);

struct VRAMState {
    u16 field0;
    u16 pad2;
    u16 pad4;
    u16 pad6;
    u16 pad8;
    u16 fieldA;
};

extern struct VRAMState data_020446d4;

static inline void writeE(u8 v) { *(volatile u8 *)((unsigned int)kh_ds_io + 0x244) = v; }
static inline void writeF(u8 v) { *(volatile u8 *)((unsigned int)kh_ds_io + 0x245) = v; }
static inline void writeG(u8 v) { *(volatile u8 *)((unsigned int)kh_ds_io + 0x246) = v; }

void GX_SetBankForTexPltt(int bank) {
    data_020446d4.field0 = (u16)(((data_020446d4.field0 | data_020446d4.fieldA)) & ~bank);
    data_020446d4.fieldA = (u16)bank;
    switch (bank) {
    case 0x00:
        break;
    case 0x60:
        writeG(0x8b);
        /* fallthrough */
    case 0x20:
        writeF(0x83);
        break;
    case 0x40:
        writeG(0x83);
        break;
    case 0x70:
        writeG(0x9b);
        /* fallthrough */
    case 0x30:
        writeF(0x93);
        /* fallthrough */
    case 0x10:
        writeE(0x83);
        break;
    }
    GX_VRAMCNT_SetLCDC_(data_020446d4.field0);
}
