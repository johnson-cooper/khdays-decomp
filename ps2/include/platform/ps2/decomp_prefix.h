/* Force-included (-include) in front of every decomp source compiled for the PS2.
 *
 * It adapts CodeWarrior-for-DS spellings to the EE gcc without touching the
 * matching sources.  It must stay small and must not hide real portability
 * problems: anything hardware-specific is replaced in ps2/ (see docs/PS2_PORT.md),
 * never papered over here.
 */
#ifndef KH_PS2_DECOMP_PREFIX_H
#define KH_PS2_DECOMP_PREFIX_H

#ifndef PLATFORM_PS2
#define PLATFORM_PS2 1
#endif

/* ARM CLZ: count leading zeros, 32 for zero (gcc's __builtin_clz(0) is undefined).  Used by the
 * prepared copies of the few functions that used `asm { clz }` (ps2/tools/prep_sources.py). */
static inline unsigned int kh_clz(unsigned int x) { return x ? (unsigned int)__builtin_clz(x) : 32u; }

/* Decode base of the game's packed-pointer handles (0x01ff8000 on the DS: RAM base 0x02000000
 * minus the 0x8000 bias the encoder adds).  On the PS2 everything the game packs lives in the
 * first 16 MiB (checked by kh_mem_init), so the base is 0 - 0x8000.  prep_sources.py rule R6. */
#define KH_DS_PACKED_PTR_BASE 0xffff8000u

/* PS2-side DS display state and memories (ps2/src/nitro/nitro_hw.c); literal DS addresses in
 * game code are rewritten to these by prep_sources.py rule R5. */
extern unsigned char kh_ds_io[];
extern unsigned char kh_ds_pal[];
extern unsigned char kh_ds_oam[];
extern unsigned char kh_ds_vram[];
extern unsigned char kh_ds_hiram[];
extern unsigned char *kh_ds_vram_win(int window);

/* Geometry-engine register access (prep_sources.py rule R8, ps2/src/nitro/nitro_g3.c). */
void kh_ge_port_write1(unsigned int io_offset, unsigned int value);
unsigned int kh_ge_port_read(unsigned int io_offset);

#endif
