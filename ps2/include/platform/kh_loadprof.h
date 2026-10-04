/* Low-overhead scene/resource loading diagnostics for real PS2 hardware.
 *
 * Counters and the physical-read trace stay in EE RAM.  Completed profiles are copied into the
 * normal deferred log buffer; a stalled profile can be formatted directly by the watchdog without
 * asking the IOP or the filesystem to do more work.
 */
#ifndef KH_LOADPROF_H
#define KH_LOADPROF_H

#include <stddef.h>
#include <stdint.h>

typedef enum KhLoadProfileKind {
    KH_LOAD_PROFILE_NONE = 0,
    KH_LOAD_PROFILE_NEW_GAME,
    KH_LOAD_PROFILE_START_GAME,
    KH_LOAD_PROFILE_SCENE
} KhLoadProfileKind;

void kh_loadprof_set_pack_fat(const uint32_t *fat_words, uint32_t file_count);
void kh_loadprof_begin(KhLoadProfileKind kind, int from_scene, int to_scene);
void kh_loadprof_end(const char *reason);
int  kh_loadprof_is_active(void);
int  kh_newgame_loading(void);
void kh_loadprof_scene_ready(int scene);
void kh_loadprof_frame(void);

void kh_loadprof_logical(uint32_t offset, uint32_t size);
void kh_loadprof_cache_hit(void);
void kh_loadprof_cache_miss(void);
void kh_loadprof_window_refill(void);
void kh_loadprof_direct_read(void);
void kh_loadprof_seek(void);
void kh_loadprof_raw_begin(uint32_t req_offset, uint32_t req_size,
                           uint32_t raw_offset, uint32_t raw_size, int window);
void kh_loadprof_raw_end(int result, uint32_t duration_us);

/* Returns 1 when line `index` exists, 0 at the end.  Safe while a physical read is stuck. */
int kh_loadprof_watchdog_line(int index, char *out, size_t out_size);

#endif
