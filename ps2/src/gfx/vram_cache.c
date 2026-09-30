/* GS VRAM texture residency cache.
 *
 * Every texture/palette the game "uploads to VRAM" becomes an entry here: GS address, PSM,
 * size, CLUT, owner, generation and last-used frame.  Misses upload by DMA; eviction is LRU
 * among entries not referenced in the current frame.  Framebuffers/Z (ps2_gs.c) sit below
 * kh_gs_texpool_base() and are never evicted.
 *
 * Bring-up state: accounting only; the cache itself lands with the renderer.
 */
#include "platform/kh_platform.h"
#include "platform/ps2/ps2_gs.h"

static uint32_t g_used;

uint32_t kh_gs_texpool_used(void) { return g_used; }

/* Texture / palette VRAM was reloaded by the game: cached GS copies of that range are stale. */
static uint32_t g_tex_generation, g_pltt_generation;

extern void kh_tex3d_invalidate(void);
extern uint32_t kh_tex3d_used(void);

void kh_gfx_tex_dirty(uint32_t ofs, uint32_t size) { (void)ofs; (void)size; g_tex_generation++; kh_tex3d_invalidate(); }
void kh_gfx_pltt_dirty(uint32_t ofs, uint32_t size) { (void)ofs; (void)size; g_pltt_generation++; kh_tex3d_invalidate(); }
