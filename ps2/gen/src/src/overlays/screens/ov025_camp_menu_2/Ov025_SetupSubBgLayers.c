/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/Ov025_SetupSubBgLayers.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov025_SetupSubBgLayers -- Ov008_SetupSubBgLayers (176 B, reloc-free).
 * Configures the 2D sub-engine (regs at 0x04001000) for the menu: enables all layers in DISPCNT
 * (bits 8-12), then for each of the four BGnCNT registers keeps the low mode bits (& 0x43) and
 * programs the screen/char base (BG0 = 0x4080; BG1/2/3 = 0x4080 + 0x210/0x410/0x610). A second
 * pass sets the layer priorities in ascending order (BG0=0, BG1=1, BG2=2, BG3=3). */

#include "nitro/types.h"

typedef struct GfxRegsB {
    u32 dispcnt;   /* 0x00: 0x04001000 */
    u8  pad_04[4];
    u16 bg0cnt;    /* 0x08 */
    u16 bg1cnt;    /* 0x0a */
    u16 bg2cnt;    /* 0x0c */
    u16 bg3cnt;    /* 0x0e */
} GfxRegsB;

void Ov025_SetupSubBgLayers(void)
{
    volatile GfxRegsB *r = (volatile GfxRegsB *)((unsigned int)kh_ds_io + 0x1000);
    int base = 0x4080;

    r->dispcnt = (r->dispcnt & ~0x1f00) | 0x1f00;
    r->bg0cnt = (r->bg0cnt & 0x43) | 0x80 | 0x4000;
    r->bg1cnt = (r->bg1cnt & 0x43) | (base + 0x210);
    r->bg2cnt = (r->bg2cnt & 0x43) | (base + 0x410);
    r->bg3cnt = (r->bg3cnt & 0x43) | (base + 0x610);
    r->bg0cnt = r->bg0cnt & ~3;
    r->bg1cnt = (r->bg1cnt & ~3) | 1;
    r->bg2cnt = (r->bg2cnt & ~3) | 2;
    r->bg3cnt = (r->bg3cnt & ~3) | 3;
}
