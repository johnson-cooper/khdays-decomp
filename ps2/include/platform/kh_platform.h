/* Kingdom Hearts 358/2 Days -- platform abstraction.
 *
 * The game and the re-implemented SDK layer (ps2/src/nitro, ps2/src/nns) talk to the machine
 * only through this interface.  The PS2 implementation lives in ps2/src/platform/.  Nothing in
 * here knows about Nintendo DS hardware, and nothing outside ps2/src/platform knows about PS2
 * hardware.
 */
#ifndef KH_PLATFORM_H
#define KH_PLATFORM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------------------------- logging */

enum { KH_LOG_ERROR = 0, KH_LOG_WARN = 1, KH_LOG_INFO = 2, KH_LOG_DEBUG = 3, KH_LOG_TRACE = 4 };

void kh_log(int level, const char *subsystem, const char *fmt, ...) __attribute__((format(printf, 3, 4)));
void kh_log_set_level(int level);
void kh_log_flush(void);
/* Unrecoverable error: logs, shows the message on screen (never a black screen) and halts. */
void kh_panic(const char *fmt, ...) __attribute__((noreturn, format(printf, 1, 2)));

#define KH_ERR(sub, ...)   kh_log(KH_LOG_ERROR, sub, __VA_ARGS__)
#define KH_WARN(sub, ...)  kh_log(KH_LOG_WARN, sub, __VA_ARGS__)
#define KH_INFO(sub, ...)  kh_log(KH_LOG_INFO, sub, __VA_ARGS__)
#define KH_DBG(sub, ...)   kh_log(KH_LOG_DEBUG, sub, __VA_ARGS__)

/* Logs "<what> not implemented on PS2" once per call site. */
#define KH_UNIMPLEMENTED_ONCE(what) do { static int kh__once; if (!kh__once) { kh__once = 1; \
    kh_log(KH_LOG_WARN, "stub", "%s: not implemented on PS2 yet", what); } } while (0)

/* ----------------------------------------------------------------- memory */

/* Lifetimes of PS2-side allocations.  Each has its own arena so that a whole lifetime can be
 * released at once and fragmentation stays inside it. */
typedef enum KhLifetime {
    KH_LIFE_GLOBAL = 0,  /* whole run */
    KH_LIFE_SCENE,       /* until the current scene ends */
    KH_LIFE_MISSION,
    KH_LIFE_ROOM,
    KH_LIFE_CHARACTER,
    KH_LIFE_ENEMY,
    KH_LIFE_EFFECT,
    KH_LIFE_FRAME,       /* reset every frame */
    KH_LIFE_COUNT
} KhLifetime;

/* Memory categories for accounting (what the bytes are for). */
typedef enum KhMemCat {
    KH_MEM_MISC = 0,
    KH_MEM_GAME_HEAP,    /* the DS-equivalent arena the game's own heaps live in */
    KH_MEM_TEXTURE,
    KH_MEM_GEOMETRY,
    KH_MEM_ANIMATION,
    KH_MEM_RESOURCE,     /* level/resource data */
    KH_MEM_AUDIO,
    KH_MEM_FILE_CACHE,
    KH_MEM_RENDER,       /* GIF/DMA packet buffers */
    KH_MEM_COUNT
} KhMemCat;

void  kh_mem_init(void);
void *kh_alloc(size_t size, size_t align, KhLifetime life, KhMemCat cat);
void  kh_free(void *p);                       /* only for KH_LIFE_GLOBAL allocations */
void  kh_mem_reset_lifetime(KhLifetime life); /* frees every allocation of that lifetime */
/* The block the DS game's OS arena (OS_GetArenaLo/Hi) is carved from. */
void *kh_mem_game_arena(size_t *size_out);

typedef struct KhMemStats {
    uint32_t ee_total;        /* EE RAM usable by the program (after the ELF) */
    uint32_t ee_elf;          /* ELF image (text+data+bss) */
    uint32_t heap_used;       /* malloc heap in use */
    uint32_t heap_free;       /* malloc heap remaining */
    uint32_t heap_largest;    /* largest single block malloc can still return (approx.) */
    uint32_t cat_used[KH_MEM_COUNT];
    uint32_t life_used[KH_LIFE_COUNT];
    uint32_t life_cap[KH_LIFE_COUNT];
    uint32_t gs_vram_used;    /* bytes of GS VRAM allocated (fb + z + resident textures) */
    uint32_t gs_vram_total;
} KhMemStats;

void kh_mem_get_stats(KhMemStats *out);
void kh_mem_report(void);                     /* logs the stats */
void kh_mem_account(KhMemCat cat, int32_t delta); /* for memory owned elsewhere (e.g. game heaps) */

/* ----------------------------------------------------------------- timing */

void     kh_time_init(void);
uint64_t kh_time_ticks(void);                 /* monotonic, KH_TICKS_PER_SEC */
#define  KH_TICKS_PER_SEC 147456000ull        /* EE bus clock */
uint64_t kh_time_us(void);
void     kh_time_sleep_us(uint32_t us);

void     kh_vblank_wait(void);                /* blocks until the next VBlank start */
uint32_t kh_vblank_count(void);               /* VBlanks since boot */
int      kh_video_refresh_hz(void);           /* 60 (NTSC) or 50 (PAL) */

/* ------------------------------------------------------------- filesystem */

typedef struct KhFile KhFile;

/* Boot device / directory the ELF was started from, e.g. "mass0:/khdays/". */
const char *kh_vfs_boot_dir(void);
const char *kh_vfs_boot_device(void);         /* "mass", "mmce", "hdd", "host", "cdrom", ... */
int      kh_vfs_init(int argc, char **argv);
/* Resolve a path relative to the boot directory ("ps2data/khdays.pak" -> "mass0:/khdays/ps2data/khdays.pak"). */
void     kh_vfs_resolve(const char *rel, char *out, size_t outsz);
KhFile  *kh_file_open(const char *path, int write); /* absolute or boot-relative */
int32_t  kh_file_read(KhFile *f, void *dst, uint32_t size);
int32_t  kh_file_write(KhFile *f, const void *src, uint32_t size);
int32_t  kh_file_seek(KhFile *f, uint32_t pos);
uint32_t kh_file_tell(KhFile *f);
uint32_t kh_file_size(KhFile *f);
void     kh_file_close(KhFile *f);
int      kh_file_exists(const char *path);
int      kh_file_rename(const char *from, const char *to);
int      kh_file_remove(const char *path);
int      kh_file_mkdir(const char *path);

/* ------------------------------------------------------------------ input */

enum {
    KH_BTN_SELECT = 1 << 0,  KH_BTN_L3 = 1 << 1,     KH_BTN_R3 = 1 << 2,     KH_BTN_START = 1 << 3,
    KH_BTN_UP = 1 << 4,      KH_BTN_RIGHT = 1 << 5,  KH_BTN_DOWN = 1 << 6,   KH_BTN_LEFT = 1 << 7,
    KH_BTN_L2 = 1 << 8,      KH_BTN_R2 = 1 << 9,     KH_BTN_L1 = 1 << 10,    KH_BTN_R1 = 1 << 11,
    KH_BTN_TRIANGLE = 1 << 12, KH_BTN_CIRCLE = 1 << 13, KH_BTN_CROSS = 1 << 14, KH_BTN_SQUARE = 1 << 15,
};

typedef struct KhPadState {
    int      connected;
    uint32_t held;           /* KH_BTN_* */
    uint32_t pressed;        /* went down this poll */
    uint32_t released;
    int8_t   lx, ly, rx, ry; /* analog, -128..127, dead zone applied; 0 when digital-only */
} KhPadState;

void kh_input_init(void);
void kh_input_poll(void);
const KhPadState *kh_input_pad(int port);

/* ------------------------------------------------------------------ video */

typedef enum { KH_VIDEO_AUTO = 0, KH_VIDEO_NTSC, KH_VIDEO_PAL } KhVideoMode;

int  kh_video_init(KhVideoMode mode);
int  kh_video_width(void);
int  kh_video_height(void);
void kh_video_begin_frame(uint32_t clear_rgb);
void kh_video_end_frame(void);               /* submit + wait for the next VBlank + flip */
void kh_video_submit_frame(void);            /* kick the frame, wait for the GS to finish drawing */
void kh_video_flip(void);                    /* show the submitted frame; call right after a VBlank */
/* Debug text drawn on top of the frame (small built-in font); cleared every frame. */
void kh_video_debug_text(int x, int y, uint32_t rgb, const char *fmt, ...) __attribute__((format(printf, 4, 5)));

/* ------------------------------------------------------------------ audio */

int  kh_audio_init(void);
void kh_audio_update(void);

/* -------------------------------------------------------------- profiling */

typedef enum KhProfZone {
    KH_PROF_FRAME = 0, KH_PROF_UPDATE, KH_PROF_RENDER_SUBMIT, KH_PROF_GS_WAIT,
    KH_PROF_LOAD, KH_PROF_AUDIO, KH_PROF_COUNT
} KhProfZone;

typedef struct KhProfStats {
    float    fps;
    uint32_t zone_us[KH_PROF_COUNT];  /* last frame */
    uint32_t draw_calls, triangles, vertices, tex_uploads, tex_upload_bytes;
} KhProfStats;

void kh_prof_begin(KhProfZone z);
void kh_prof_end(KhProfZone z);
void kh_prof_frame(void);                     /* closes a frame */
void kh_prof_count(uint32_t draw_calls, uint32_t tris, uint32_t verts);
void kh_prof_tex_upload(uint32_t bytes);
const KhProfStats *kh_prof_stats(void);

/* ---------------------------------------------------------- platform init */

/* Brings the whole platform up in dependency order; panics with a readable message on failure. */
void kh_platform_init(int argc, char **argv);
void kh_platform_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* KH_PLATFORM_H */
