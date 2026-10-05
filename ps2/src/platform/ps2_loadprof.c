/* Real-hardware load profiler and transition breadcrumbs.
 *
 * There is deliberately no per-read logging here.  USB/FAT logging would change the workload we
 * are trying to measure and can contend with audio on the IOP.  A fixed trace and all counters
 * live in EE RAM; only a compact report is appended to the deferred log after a stage completes.
 */
#include "platform/kh_platform.h"
#include "platform/kh_loadprof.h"

#include <stdio.h>
#include <string.h>

#define TRACE_COUNT       96
#define TRACE_DUMP_COUNT  24
#define REGION_SHIFT      20              /* 1 MiB rough pack regions */
#define REGION_COUNT      512             /* enough for a 512 MiB pack */

typedef struct KhLoadTrace {
    uint32_t req_offset, req_size;
    uint32_t raw_offset, raw_size;
    uint32_t actual_size, duration_us;
    int16_t window;
    int16_t result;
} KhLoadTrace;

typedef struct KhLoadState {
    int active;
    KhLoadProfileKind kind;
    int from_scene, to_scene;
    int scene_ready;
    uint32_t ready_vblank;
    uint64_t start_us, end_us, last_logical_us;

    uint64_t logical_bytes, physical_bytes, io_us;
    uint32_t read_calls, cache_hits, cache_misses;
    uint32_t window_refills, direct_reads, raw_calls, seeks;
    uint32_t io_min_us, io_max_us;
    uint32_t region_bits[REGION_COUNT / 32];
    uint32_t region_overflow;

    KhLoadTrace trace[TRACE_COUNT];
    uint32_t trace_head, trace_count;
    int pending_trace;
    int raw_active;
    uint64_t raw_start_us;
    uint32_t active_req_offset, active_req_size;
    uint32_t active_raw_offset, active_raw_size;
    int active_window;
} KhLoadState;

/* Also used by the release compositor: the lower screen's VRAM is incomplete between the
 * new-game sub-scene constructor and its ready callback. */
static volatile int g_new_game_loading;

#if KH_PS2_DEBUG
static KhLoadState g_load;
static KhLoadState g_last;
static int g_last_valid;
static volatile uint32_t g_write_seq;
static const uint32_t *g_pack_fat;
static uint32_t g_pack_files;

static void write_begin(void)
{
    g_write_seq++;
    __asm__ volatile("" ::: "memory");
}

static void write_end(void)
{
    __asm__ volatile("" ::: "memory");
    g_write_seq++;
}

static const char *kind_name(KhLoadProfileKind kind)
{
    switch (kind) {
    case KH_LOAD_PROFILE_NEW_GAME:  return "New Game";
    case KH_LOAD_PROFILE_START_GAME:return "Start Game";
    case KH_LOAD_PROFILE_SCENE:     return "scene";
    default:                        return "none";
    }
}

static int pack_file_at(uint32_t offset)
{
    uint32_t i;
    /* Only formatting paths call this.  Keeping file-id resolution out of the physical-read hot
     * path is cheaper than a search for every refill, and 1595 entries is tiny at dump time. */
    for (i = 0; i < g_pack_files; i++) {
        uint32_t start = g_pack_fat[i * 2], end = g_pack_fat[i * 2 + 1];
        if (end > start && offset >= start && offset < end)
            return (int)i;
    }
    return -1;
}

static uint32_t regions_touched(const KhLoadState *s)
{
    uint32_t i, n = 0;
    for (i = 0; i < REGION_COUNT / 32; i++)
        n += (uint32_t)__builtin_popcount(s->region_bits[i]);
    return n + (s->region_overflow != 0);
}

static uint32_t elapsed_ms(const KhLoadState *s, uint64_t now_us)
{
    uint64_t end = s->active ? now_us : s->end_us;
    uint64_t us = end > s->start_us ? end - s->start_us : 0;
    return (uint32_t)(us / 1000u);
}

static uint32_t amplification_x10(const KhLoadState *s)
{
    if (!s->logical_bytes)
        return 0;
    return (uint32_t)(s->physical_bytes * 10u / s->logical_bytes);
}

static void dump_state(const KhLoadState *s, const char *reason)
{
    uint32_t amp = amplification_x10(s);
    uint32_t avg = s->raw_calls ? (uint32_t)(s->io_us / s->raw_calls) : 0;
    uint32_t n = s->trace_count < TRACE_DUMP_COUNT ? s->trace_count : TRACE_DUMP_COUNT;
    uint32_t first = (s->trace_head + TRACE_COUNT - n) % TRACE_COUNT;
    uint32_t i;

    KH_INFO("loadprof", "%s %d->%d: %u ms (%s)", kind_name(s->kind), s->from_scene,
            s->to_scene, (unsigned)elapsed_ms(s, s->end_us), reason ? reason : "done");
    KH_INFO("loadprof", "logical %u KiB physical %u KiB amp %u.%ux; calls %u hit %u miss %u",
            (unsigned)(s->logical_bytes / 1024u), (unsigned)(s->physical_bytes / 1024u),
            (unsigned)(amp / 10), (unsigned)(amp % 10), (unsigned)s->read_calls,
            (unsigned)s->cache_hits, (unsigned)s->cache_misses);
    KH_INFO("loadprof", "raw %u seek %u refill %u direct %u regions %u; io %u ms avg %u us min %u max %u",
            (unsigned)s->raw_calls, (unsigned)s->seeks, (unsigned)s->window_refills,
            (unsigned)s->direct_reads, (unsigned)regions_touched(s),
            (unsigned)(s->io_us / 1000u), (unsigned)avg,
            (unsigned)(s->io_min_us == UINT32_MAX ? 0 : s->io_min_us), (unsigned)s->io_max_us);
    for (i = 0; i < n; i++) {
        const KhLoadTrace *t = &s->trace[(first + i) % TRACE_COUNT];
        int fid = pack_file_at(t->raw_offset);
        KH_INFO("loadprof", "trace req %08x+%x raw %08x+%x got %x w%d fid %d %u us",
                (unsigned)t->req_offset, (unsigned)t->req_size, (unsigned)t->raw_offset,
                (unsigned)t->raw_size, (unsigned)t->actual_size, (int)t->window, fid,
                (unsigned)t->duration_us);
    }
}

static void finish_current(const char *reason)
{
    uint64_t now;
    if (!g_load.active)
        return;
    now = kh_time_us();
    write_begin();
    g_load.active = 0;
    g_load.end_us = now;
    memcpy(&g_last, &g_load, sizeof g_last);
    g_last_valid = 1;
    write_end();
    dump_state(&g_last, reason);
}

static int snapshot(KhLoadState *out)
{
    uint32_t a, b;
    int tries;
    for (tries = 0; tries < 8; tries++) {
        a = g_write_seq;
        if (a & 1)
            continue;
        __asm__ volatile("" ::: "memory");
        if (g_load.active)
            memcpy(out, &g_load, sizeof *out);
        else if (g_last_valid)
            memcpy(out, &g_last, sizeof *out);
        else
            return 0;
        __asm__ volatile("" ::: "memory");
        b = g_write_seq;
        if (a == b && !(b & 1))
            return 1;
    }
    return 0;
}
#endif

void kh_loadprof_set_pack_fat(const uint32_t *fat_words, uint32_t file_count)
{
#if KH_PS2_DEBUG
    g_pack_fat = fat_words;
    g_pack_files = file_count;
#else
    (void)fat_words; (void)file_count;
#endif
}

void kh_loadprof_begin(KhLoadProfileKind kind, int from_scene, int to_scene)
{
#if KH_PS2_DEBUG
    if (g_load.active)
        finish_current("superseded");
    write_begin();
    memset(&g_load, 0, sizeof g_load);
    g_load.active = 1;
    g_load.kind = kind;
    g_load.from_scene = from_scene;
    g_load.to_scene = to_scene;
    g_load.start_us = g_load.last_logical_us = kh_time_us();
    g_load.io_min_us = UINT32_MAX;
    g_load.pending_trace = -1;
    write_end();
    KH_INFO("loadprof", "begin %s %d->%d", kind_name(kind), from_scene, to_scene);
#else
    (void)kind; (void)from_scene; (void)to_scene;
#endif
}

void kh_loadprof_end(const char *reason)
{
#if KH_PS2_DEBUG
    finish_current(reason);
#else
    (void)reason;
#endif
}

int kh_loadprof_is_active(void)
{
#if KH_PS2_DEBUG
    return g_load.active;
#else
    return 0;
#endif
}

int kh_newgame_loading(void)
{
    return g_new_game_loading;
}

void kh_loadprof_scene_ready(int scene)
{
#if KH_PS2_DEBUG
    if (g_load.active && (!g_load.to_scene || g_load.to_scene == scene)) {
        write_begin();
        g_load.scene_ready = 1;
        g_load.ready_vblank = kh_vblank_count();
        write_end();
    }
#else
    (void)scene;
#endif
}

void kh_loadprof_frame(void)
{
#if KH_PS2_DEBUG
    uint64_t now;
    uint32_t ready_frames, cap;
    if (!g_load.active || !g_load.scene_ready || g_load.kind == KH_LOAD_PROFILE_NEW_GAME)
        return;
    now = kh_time_us();
    ready_frames = kh_vblank_count() - g_load.ready_vblank;
    cap = g_load.kind == KH_LOAD_PROFILE_START_GAME ? 120u : 180u;
    /* End when the startup I/O has gone quiet.  A continuously streamed movie will not go quiet,
     * so cap that startup profile after two/three seconds of successfully presented frames. */
    if ((!g_load.raw_active && now - g_load.last_logical_us >= 500000u) || ready_frames >= cap)
        finish_current(ready_frames >= cap ? "startup window complete" : "I/O quiescent");
#endif
}

void kh_loadprof_logical(uint32_t offset, uint32_t size)
{
#if KH_PS2_DEBUG
    if (!g_load.active)
        return;
    write_begin();
    g_load.read_calls++;
    g_load.logical_bytes += size;
    g_load.last_logical_us = kh_time_us();
    g_load.active_req_offset = offset;
    g_load.active_req_size = size;
    write_end();
#else
    (void)offset; (void)size;
#endif
}

void kh_loadprof_cache_hit(void)
{
#if KH_PS2_DEBUG
    if (g_load.active) { write_begin(); g_load.cache_hits++; write_end(); }
#endif
}

void kh_loadprof_cache_miss(void)
{
#if KH_PS2_DEBUG
    if (g_load.active) { write_begin(); g_load.cache_misses++; write_end(); }
#endif
}

void kh_loadprof_window_refill(void)
{
#if KH_PS2_DEBUG
    if (g_load.active) { write_begin(); g_load.window_refills++; write_end(); }
#endif
}

void kh_loadprof_direct_read(void)
{
#if KH_PS2_DEBUG
    if (g_load.active) { write_begin(); g_load.direct_reads++; write_end(); }
#endif
}

void kh_loadprof_seek(void)
{
#if KH_PS2_DEBUG
    if (g_load.active) { write_begin(); g_load.seeks++; write_end(); }
#endif
}

void kh_loadprof_raw_begin(uint32_t req_offset, uint32_t req_size,
                           uint32_t raw_offset, uint32_t raw_size, int window)
{
#if KH_PS2_DEBUG
    KhLoadTrace *t;
    uint32_t first, last, r;
    if (!g_load.active)
        return;
    write_begin();
    g_load.raw_calls++;
    g_load.raw_active = 1;
    g_load.raw_start_us = kh_time_us();
    g_load.active_req_offset = req_offset;
    g_load.active_req_size = req_size;
    g_load.active_raw_offset = raw_offset;
    g_load.active_raw_size = raw_size;
    g_load.active_window = window;
    g_load.pending_trace = (int)g_load.trace_head;
    t = &g_load.trace[g_load.trace_head];
    memset(t, 0, sizeof *t);
    t->req_offset = req_offset;
    t->req_size = req_size;
    t->raw_offset = raw_offset;
    t->raw_size = raw_size;
    t->window = (int16_t)window;
    t->duration_us = UINT32_MAX;       /* still in flight if the IOP never returns */
    g_load.trace_head = (g_load.trace_head + 1) % TRACE_COUNT;
    if (g_load.trace_count < TRACE_COUNT)
        g_load.trace_count++;
    if (raw_size) {
        first = raw_offset >> REGION_SHIFT;
        last = (raw_offset + raw_size - 1) >> REGION_SHIFT;
        for (r = first; r <= last && r < REGION_COUNT; r++)
            g_load.region_bits[r >> 5] |= 1u << (r & 31);
        if (last >= REGION_COUNT)
            g_load.region_overflow = 1;
    }
    write_end();
#else
    (void)req_offset; (void)req_size; (void)raw_offset; (void)raw_size; (void)window;
#endif
}

void kh_loadprof_raw_end(int result, uint32_t duration_us)
{
#if KH_PS2_DEBUG
    KhLoadTrace *t;
    if (!g_load.active || !g_load.raw_active)
        return;
    write_begin();
    if (result > 0)
        g_load.physical_bytes += (uint32_t)result;
    g_load.io_us += duration_us;
    if (duration_us < g_load.io_min_us)
        g_load.io_min_us = duration_us;
    if (duration_us > g_load.io_max_us)
        g_load.io_max_us = duration_us;
    if (g_load.pending_trace >= 0) {
        t = &g_load.trace[g_load.pending_trace];
        t->result = (int16_t)(result < -32768 ? -32768 : result > 32767 ? 32767 : result);
        t->actual_size = result > 0 ? (uint32_t)result : 0;
        t->duration_us = duration_us;
    }
    g_load.pending_trace = -1;
    g_load.raw_active = 0;
    write_end();
#else
    (void)result; (void)duration_us;
#endif
}

int kh_loadprof_watchdog_line(int index, char *out, size_t out_size)
{
#if KH_PS2_DEBUG
    KhLoadState s;
    uint64_t now = kh_time_us();
    uint32_t amp, avg;
    int fid;
    if (!snapshot(&s))
        return 0;
    amp = amplification_x10(&s);
    avg = s.raw_calls ? (uint32_t)(s.io_us / s.raw_calls) : 0;
    switch (index) {
    case 0:
        snprintf(out, out_size, "load: %s %d->%d %s %u.%us", kind_name(s.kind), s.from_scene,
                 s.to_scene, s.active ? "ACTIVE" : "last", elapsed_ms(&s, now) / 1000,
                 (elapsed_ms(&s, now) / 100) % 10);
        return 1;
    case 1:
        snprintf(out, out_size, "I/O: log %uK phys %uK amp %u.%ux calls %u raw %u seek %u",
                 (unsigned)(s.logical_bytes / 1024u), (unsigned)(s.physical_bytes / 1024u),
                 (unsigned)(amp / 10), (unsigned)(amp % 10), (unsigned)s.read_calls,
                 (unsigned)s.raw_calls, (unsigned)s.seeks);
        return 1;
    case 2:
        snprintf(out, out_size, "cache: hit %u miss %u refill %u direct %u regions %u io %ums avg %uus max %uus",
                 (unsigned)s.cache_hits, (unsigned)s.cache_misses, (unsigned)s.window_refills,
                 (unsigned)s.direct_reads, (unsigned)regions_touched(&s),
                 (unsigned)(s.io_us / 1000u), (unsigned)avg, (unsigned)s.io_max_us);
        return 1;
    case 3:
        if (s.raw_active) {
            fid = pack_file_at(s.active_raw_offset);
            snprintf(out, out_size, "READING req %08x+%x raw %08x+%x w%d fid %d for %ums",
                     (unsigned)s.active_req_offset, (unsigned)s.active_req_size,
                     (unsigned)s.active_raw_offset, (unsigned)s.active_raw_size,
                     s.active_window, fid, (unsigned)((now - s.raw_start_us) / 1000u));
        } else if (s.trace_count) {
            const KhLoadTrace *t = &s.trace[(s.trace_head + TRACE_COUNT - 1) % TRACE_COUNT];
            fid = pack_file_at(t->raw_offset);
            snprintf(out, out_size, "last req %08x+%x raw %08x+%x w%d fid %d %uus",
                     (unsigned)t->req_offset, (unsigned)t->req_size, (unsigned)t->raw_offset,
                     (unsigned)t->raw_size, (int)t->window, fid, (unsigned)t->duration_us);
        } else {
            snprintf(out, out_size, "no physical reads recorded in this stage");
        }
        return 1;
    default:
        return 0;
    }
#else
    (void)index; (void)out; (void)out_size;
    return 0;
#endif
}

/* Linker wrappers give profiles real gameplay boundaries without changing matching DS sources. */
typedef struct KhSceneCtl {
    void *obj;
    void *entry;
    int cur_id, pending_id, pending_arg;
} KhSceneCtl;

extern char gSceneCtl[];
extern int __real_Scene_AdvanceToPending(void);

int __wrap_Scene_AdvanceToPending(void)
{
    KhSceneCtl *s = (KhSceneCtl *)gSceneCtl;
    int before = s->cur_id;
    int pending = s->pending_id;
    int result;
#if KH_PS2_DEBUG
    extern volatile const char *kh_watchdog_mark;
    if (pending && before && !kh_loadprof_is_active())
        kh_loadprof_begin(KH_LOAD_PROFILE_SCENE, before, pending);
    if (pending)
        kh_watchdog_mark = "scene: unload/load/instantiate";
#endif
    result = __real_Scene_AdvanceToPending();
    if (s->cur_id && s->cur_id != before)
        kh_loadprof_scene_ready(s->cur_id);
    return result;
}

extern void *__real_Ov000_WaitLoadThenBuildMenu(void);

extern void *__real_Ov000_InitSubScene(void);

void *__wrap_Ov000_InitSubScene(void)
{
    g_new_game_loading = 1;
    kh_loadprof_begin(KH_LOAD_PROFILE_NEW_GAME, 1, 1);
    return __real_Ov000_InitSubScene();
}

void *__wrap_Ov000_WaitLoadThenBuildMenu(void)
{
    void *next;
    if (!g_new_game_loading) {
        /* Fallback for any call path that did not enter through the normal constructor. */
        g_new_game_loading = 1;
        kh_loadprof_begin(KH_LOAD_PROFILE_NEW_GAME, 1, 1);
    }
    next = __real_Ov000_WaitLoadThenBuildMenu();
    if (next) {
        g_new_game_loading = 0;
        kh_loadprof_end("difficulty menu ready");
    }
    return next;
}

extern int __real_Ov000_BootRunSelector(void);
extern void *NNSi_FndGetCurrentRootHeap(void);

int __wrap_Ov000_BootRunSelector(void)
{
    unsigned char *ctx = NNSi_FndGetCurrentRootHeap();
    int requested = ctx && *(int *)(ctx + 0x4c40) != 0;
    /* to_scene 0: end at whichever scene the start path reaches (the opening, or the field when
     * a save is loaded); a fixed 11 never closed on the load path and kept profiling forever */
    if (requested)
        kh_loadprof_begin(KH_LOAD_PROFILE_START_GAME, 1, 0);
    return __real_Ov000_BootRunSelector();
}
