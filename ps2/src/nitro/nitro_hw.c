/* PS2-side storage for the DS display model the game is written against.
 *
 *   kh_ds_io     2D engine A (0x000-0x06f) and B (0x1000-0x106f) control registers, display
 *                capture, master brightness, 3D display control, KEYINPUT mirror.  Plain state:
 *                the GS compositor (ps2/src/gfx) reads it every frame; writing it has no side
 *                effects.  Registers WITH side effects (geometry engine, divider, DMA, timers,
 *                IRQ) are never mapped here -- their users are overridden (prep_sources.py R5).
 *   kh_ds_pal    standard palettes: BG/OBJ, engine A then B (0x800 bytes)
 *   kh_ds_oam    OAM, engine A then B (0x800 bytes)
 *   kh_ds_vram   the nine VRAM banks A..I in LCDC order (656 KiB); the BG/OBJ/texture views are
 *                bank mappings over it (GX_SetBankFor*, nitro_gx.c)
 *   kh_ds_hiram  DTCM (0x027e0000) and the OS/ARM7 shared area up to 0x02800000
 */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

#include <string.h>

unsigned char kh_ds_io[0x1100] __attribute__((aligned(16)));
unsigned char kh_ds_pal[0x800] __attribute__((aligned(16)));
unsigned char kh_ds_oam[0x800] __attribute__((aligned(16)));
unsigned char kh_ds_vram[0xa4000] __attribute__((aligned(64)));
unsigned char kh_ds_hiram[0x20000] __attribute__((aligned(16)));

/* Byte offsets of each VRAM bank inside kh_ds_vram (LCDC layout). */
const uint32_t kh_ds_vram_bank_ofs[9] = {
    0x00000, 0x20000, 0x40000, 0x60000,   /* A-D 128 KiB */
    0x80000,                              /* E 64 KiB */
    0x90000, 0x94000,                     /* F, G 16 KiB */
    0x98000,                              /* H 32 KiB */
    0xa0000,                              /* I 16 KiB */
};

/* CPU view of a BG/OBJ window (0 BG-A 0x06000000, 1 BG-B 0x06200000, 2 OBJ-A 0x06400000,
 * 3 OBJ-B 0x06600000): the first bank mapped there.  nitro_gx.c keeps the mapping. */
unsigned char *kh_ds_vram_win(int window)
{
    return kh_ds_vram + kh_nitro_vram_window_ofs(window);
}
