/* Private declarations of the PS2 NitroSDK layer. */
#ifndef KH_NITRO_INTERNAL_H
#define KH_NITRO_INTERNAL_H

#include <stdint.h>
#include <kernel.h>

/* Renders everything the game recorded this frame (2D compositor, 3D batches) into the
 * current GS frame packet.  ps2/src/nitro/nitro_render.c */
void kh_nitro_render_frame(void);

/* Runs OSAlarm handlers whose fire time has passed (ps2/src/nitro/nitro_os.c). */
void kh_nitro_run_alarms(void);

extern void *_gp;

/* DS display model storage (nitro_hw.c) */
extern unsigned char kh_ds_io[];
extern unsigned char kh_ds_pal[];
extern unsigned char kh_ds_oam[];
extern unsigned char kh_ds_vram[];
extern unsigned char kh_ds_hiram[];
extern const uint32_t kh_ds_vram_bank_ofs[9];
/* Byte offset in kh_ds_vram of the first bank mapped to a BG/OBJ window (nitro_gx.c). */
uint32_t kh_nitro_vram_window_ofs(int window);

/* VRAM views (nitro_gx.c) */
uint32_t kh_nitro_view_to_vram(int view, uint32_t ofs);
unsigned char *kh_nitro_view_ptr(int view, uint32_t ofs);
uint32_t kh_nitro_view_banks(int view);

/* GS residency cache invalidation when the game reloads texture/palette VRAM (ps2/src/gfx) */
void kh_gfx_tex_dirty(uint32_t ofs, uint32_t size);
/* VRAM write generations, one per 16 KiB page of kh_ds_vram: bumped by every bulk CPU write
 * (MI fills/copies, GX loads), so caches built from VRAM can tell which pages changed */
#define KH_VRAM_PAGES (0xa4000 / 0x4000 + 1)
extern uint32_t kh_vram_page_gen[KH_VRAM_PAGES];
static inline void kh_vram_mark(uint32_t vofs, uint32_t size)
{
    uint32_t p = vofs >> 14, e = (vofs + (size ? size - 1 : 0)) >> 14;
    for (; p <= e && p < KH_VRAM_PAGES; p++)
        kh_vram_page_gen[p]++;
}
void kh_gfx_pltt_dirty(uint32_t ofs, uint32_t size);

/* DS divider / square-root unit, bit exact (nitro_cp.c) */
void kh_cp_div(int mode, s64 numer, s64 denom, s64 *quot, s64 *rem);
s64  kh_cp_div_q(int mode, s64 numer, s64 denom);
u32  kh_cp_sqrt(int mode, u64 param);

#endif
