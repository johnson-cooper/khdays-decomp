/* NitroSDK FS on the PS2: the "rom" archive and overlays.
 *
 * The SDK's FS library (libs/nitro/fs: archives, FNT/FAT walking, FSFile, async commands) is the
 * decomp's C and runs unchanged.  Only its bottom is replaced:
 *   - FSi_InitRom (func_0200afb8) took the FAT/FNT position from the ROM header in shared RAM and
 *     read through the card driver.  Here the "rom" archive is loaded over ps2data/khdays.pak:
 *     the NitroFS tables and file data of the user's ROM, re-laid out 2 KiB-aligned by
 *     ps2/tools/make_ps2data.py.  File ids and FAT semantics are unchanged, so FS_OpenFile(path),
 *     FS_OpenFileDirect(image offsets) and the game's archive handles work as on the DS.
 *   - Reads are synchronous VFS reads (the VFS reads ahead in large aligned blocks), completed
 *     immediately (FS_RESULT_SUCCESS), which the SDK's archive layer supports.
 *   - Overlays: all overlay code is linked into the ELF.  FS_LoadOverlay keeps the DS semantics
 *     that matter -- the overlay's .bss is zeroed on every load -- and tracks what is loaded.
 */
#include "platform/kh_platform.h"
#include "platform/kh_loadprof.h"
#include "nitro_internal.h"

#include <string.h>
#include <stdio.h>

/* ---- SDK entry points kept from libs/nitro/fs ---- */
typedef struct FSArchive FSArchive;
typedef int (*FSReadFunc)(FSArchive *arc, void *dst, u32 pos, u32 size);
extern void FS_InitArchive(FSArchive *arc);
extern int FS_RegisterArchiveName(FSArchive *arc, const char *name, u32 len);
extern void FS_SetArchiveProc(FSArchive *arc, void *proc, u32 flags);
extern int FS_LoadArchive(FSArchive *arc, u32 base, u32 fat, u32 fat_size, u32 fnt, u32 fnt_size,
                          FSReadFunc read, FSReadFunc write);

/* the SDK's "rom" archive object and init flag (main .bss) */
extern unsigned char data_02046334[];

enum { FS_RESULT_SUCCESS = 0, FS_RESULT_FAILURE = 1, FS_RESULT_UNSUPPORTED = 4, FS_RESULT_ERROR = 5,
       FS_RESULT_PROC_UNKNOWN = 8 };

#define PAK_MAGIC 0x4b50484bu   /* "KHPK" */

typedef struct KhPakHeader {
    u32 magic;
    u32 version;
    u32 fat_offset, fat_size;
    u32 fnt_offset, fnt_size;
    u32 file_count;
    char gamecode[4];
    u32 data_offset;
    u32 reserved[7];
} KhPakHeader;

static KhFile *g_pak;
static KhFile *g_stream_pak;
static KhPakHeader g_hdr;
#if KH_PS2_DEBUG
static u32 *g_profile_fat;
#endif

static int rom_read(FSArchive *arc, void *dst, u32 pos, u32 size)
{
    (void)arc;
    if (size >= 0x1000 && kh_vblank_count() < KH_BOOT_TRACE_VBLANKS)
        KH_INFO("fs", "read pos 0x%x size 0x%x -> %p", (unsigned)pos, (unsigned)size, dst);
    if (!g_pak || kh_file_read_at(g_pak, pos, dst, size) != (int32_t)size) {
        KH_ERR("fs", "rom read failed: pos 0x%x size 0x%x", (unsigned)pos, (unsigned)size);
        return FS_RESULT_ERROR;
    }
    return FS_RESULT_SUCCESS;
}

static int rom_write(FSArchive *arc, const void *src, u32 pos, u32 size)
{
    (void)arc; (void)src; (void)pos; (void)size;
    return FS_RESULT_FAILURE;
}

/* Streamed music uses a descriptor and cache independent from the main NitroFS archive.  The
 * NitroSystem worker and game thread run concurrently; sharing g_pak would race its physical
 * seek position and cache metadata. */
s32 kh_nitrofs_read_stream(void *dst, u32 pos, u32 size)
{
    if (!g_stream_pak || kh_file_read_at(g_stream_pak, pos, dst, size) != (s32)size) {
        KH_ERR("fs", "stream read failed: pos 0x%x size 0x%x", (unsigned)pos, (unsigned)size);
        return -1;
    }
    return (s32)size;
}

/* archive proc: accept activate/idle (the DS locked the card bus there) */
static int rom_proc(void *file, int command)
{
    (void)file;
    switch (command) {
    case 9:     /* FS_COMMAND_ACTIVATE */
    case 10:    /* FS_COMMAND_IDLE */
        return FS_RESULT_SUCCESS;
    case 1:     /* FS_COMMAND_WRITEFILE */
        return FS_RESULT_UNSUPPORTED;
    default:
        return FS_RESULT_PROC_UNKNOWN;   /* FS_RESULT_PROC_UNKNOWN: use the SDK default */
    }
}

/* Reads one 256 KiB stretch of the pack three ways - one big read, 1 KiB buffered reads, one big
 * read into a misaligned buffer - and logs whether they agree: on real hardware a cache or DMA
 * problem in the device read path shows here, where the emulator cannot reproduce it. */
#if KH_PS2_DEBUG
static void read_self_test(void)
{
    enum { N = 256 * 1024 };   /* above the 128 KiB read-ahead: the big reads go straight to the buffer */
    u8 *a = kh_alloc(N, 64, KH_LIFE_GLOBAL, KH_MEM_FILE_CACHE);
    u8 *b = kh_alloc(N, 64, KH_LIFE_GLOBAL, KH_MEM_FILE_CACHE);
    u8 *c = kh_alloc(N + 64, 64, KH_LIFE_GLOBAL, KH_MEM_FILE_CACHE);
    u32 pos = g_hdr.data_offset ? g_hdr.data_offset : 0x1000, i, bad_b = 0, bad_c = 0;
    if (!a || !b || !c)
        goto out;
    memset(a, 0x11, N); memset(b, 0x22, N); memset(c, 0x33, N + 64);
    kh_file_seek(g_pak, pos);
    kh_file_read(g_pak, a, N);
    for (i = 0; i < N; i += 1024) {
        kh_file_seek(g_pak, pos + i);
        kh_file_read(g_pak, b + i, 1024);
    }
    kh_file_seek(g_pak, pos);
    kh_file_read(g_pak, c + 3, N);
    for (i = 0; i < N; i++) {
        bad_b += a[i] != b[i];
        bad_c += a[i] != c[3 + i];
    }
    if (bad_b || bad_c)
        KH_ERR("fs", "read self-test at 0x%x: %u bytes differ (buffered), %u (misaligned)", (unsigned)pos,
               (unsigned)bad_b, (unsigned)bad_c);
    else
        KH_INFO("fs", "read self-test ok (first words %08x %08x)", ((u32 *)a)[0], ((u32 *)a)[1]);
out:
    kh_free(a); kh_free(b); kh_free(c);
}

/* Creates, writes (64 KiB + 16 unaligned bytes, as a save slot), reads back and removes a test
 * file next to the ELF while the pack is open, logging and committing the log after every step:
 * if the device hangs on creating or writing a file, the log names the step. */
static void write_probe(void)
{
    static const char name[] = "khdays_probe.tmp";
    u8 *buf = kh_alloc(0x10000 + 64, 64, KH_LIFE_GLOBAL, KH_MEM_FILE_CACHE);
    KhFile *f;
    int ok;
    if (!buf)
        return;
    memset(buf, 0x5a, 0x10000 + 64);
#define STEP(...) do { KH_INFO("fs", "write probe: " __VA_ARGS__); kh_log_flush(); } while (0)
    STEP("create %s", name);
    f = kh_file_open(name, 1);
    if (!f) {
        STEP("create failed");
        goto out;
    }
    STEP("write 64 KiB");
    ok = kh_file_write(f, buf, 0x10000) == 0x10000;
    STEP("write 16 bytes (misaligned), first write %s", ok ? "ok" : "FAILED");
    ok = kh_file_write(f, buf + 3, 16) == 16;
    STEP("close, second write %s", ok ? "ok" : "FAILED");
    ok = kh_file_close(f) == 0;
    STEP("read back, close %s", ok ? "ok" : "FAILED");
    f = kh_file_open(name, 0);
    ok = f && kh_file_size(f) == 0x10000 + 16;
    if (f)
        kh_file_close(f);
    STEP("remove, read back %s", ok ? "ok" : "FAILED");
    kh_file_remove(name);
    STEP("done");
#undef STEP
out:
    kh_free(buf);
}
#endif

/* Boot trace (real-hardware bring-up): through early title init every file the game opens is
 * logged with its result, and the log is committed line by line (ps2_log.c), so an early hang shows
 * the last file touched. */
extern int __real_FS_OpenFile(void *file, const char *path);
int __wrap_FS_OpenFile(void *file, const char *path)
{
    int r;
#if KH_PS2_DEBUG
    extern volatile const char *kh_watchdog_mark;
    static char mark[96];
    snprintf(mark, sizeof mark, "FS_OpenFile %s", path ? path : "(null)");
    kh_watchdog_mark = mark;
#endif
    r = __real_FS_OpenFile(file, path);
#if KH_PS2_DEBUG
    if (kh_vblank_count() < KH_BOOT_TRACE_VBLANKS)
        KH_INFO("fs", "open %s -> %d", path ? path : "(null)", r);
#endif
    return r;
}

static void open_pak(void)
{
    const char *path = "ps2data/khdays.pak";
    g_pak = kh_file_open(path, 0);
    if (!g_pak)
        kh_panic("Game data not found: %s%s\n\n  Create it from your own Kingdom Hearts 358/2 Days ROM:\n"
                 "    python ps2/tools/make_ps2data.py your_rom.nds ps2data\n"
                 "  and copy the ps2data folder next to the ELF.", kh_vfs_boot_dir(), path);
    g_stream_pak = kh_file_open_stream(path);
    if (!g_stream_pak)
        kh_panic("Cannot open the independent audio stream handle for %s", path);
    if (kh_file_read(g_pak, &g_hdr, sizeof g_hdr) != (int32_t)sizeof g_hdr || g_hdr.magic != PAK_MAGIC)
        kh_panic("%s is not a KH Days PS2 data pack (re-run make_ps2data.py)", path);
    if (g_hdr.version != 1)
        kh_panic("%s has pack version %u, this build needs 1 (re-run make_ps2data.py)", path, (unsigned)g_hdr.version);
    if (memcmp(g_hdr.gamecode, "YKGP", 4) != 0)
        kh_panic("%s contains %.4s data; this build targets European YKGP data", path, g_hdr.gamecode);
#if KH_PS2_DEBUG
    /* Keep the rewritten pack FAT in EE RAM so a stuck physical offset can be reported as the
     * original NitroFS file id.  At 1595 entries this costs less than 13 KiB. */
    g_profile_fat = kh_alloc(g_hdr.fat_size, 16, KH_LIFE_GLOBAL, KH_MEM_FILE_CACHE);
    if (g_profile_fat &&
        kh_file_read_at(g_pak, g_hdr.fat_offset, g_profile_fat, g_hdr.fat_size) == (int32_t)g_hdr.fat_size)
        kh_loadprof_set_pack_fat((const uint32_t *)g_profile_fat, g_hdr.file_count);
    else
        KH_WARN("fs", "load profiler could not retain the pack FAT; traces will omit file ids");
#endif
    KH_INFO("fs", "code target YKGP; data source %.4s: %u files, %u KiB", g_hdr.gamecode,
            (unsigned)g_hdr.file_count, (unsigned)(kh_file_size(g_pak) / 1024));
#if KH_PS2_DEBUG
    read_self_test();
    write_probe();
#endif
}

/* FSi_InitRom */
void func_0200afb8(u32 default_dma_no)
{
    FSArchive *arc = (FSArchive *)data_02046334;
    (void)default_dma_no;
    open_pak();
    FS_InitArchive(arc);
    FS_RegisterArchiveName(arc, "rom", 3);
    FS_SetArchiveProc(arc, (void *)rom_proc, 0x2 /* WRITEFILE */ | (1u << 9) | (1u << 10));
    if (!FS_LoadArchive(arc, 0, g_hdr.fat_offset, g_hdr.fat_size, g_hdr.fnt_offset, g_hdr.fnt_size,
                        rom_read, (FSReadFunc)rom_write))
        kh_panic("FS_LoadArchive(rom) failed");
}

/* The DS waited here for the card thread; reads are synchronous on the PS2. */
void FSi_WaitForCardThread(int unused) { (void)unused; }
void CARD_LockRom(u16 id) { (void)id; }
void CARD_UnlockRom(u16 id) { (void)id; }
void CARD_Init(void) { }

/* ------------------------------------------------------------------ overlays */

typedef struct KhOverlayInfo {
    void (*entry)(void);           /* native equivalent of the DS load/registration address */
    u32 ram_size, bss_size;        /* original DS image sizes used by overlay slot accounting */
    u8 *bss_start, *bss_end;      /* .bss laid out from the symbol map */
    u8 *data_start, *data_end;    /* .data of the overlay's C objects */
    u8 *cbss_start, *cbss_end;    /* .bss/COMMON of the overlay's C objects */
} KhOverlayInfo;
extern const KhOverlayInfo kh_overlay_info[];      /* ps2/gen/stubs/overlays.c */
extern const u32 kh_overlay_count;
extern const u32 __kh_overlay_retain_start[], __kh_overlay_retain_end[];

static u8 g_overlay_loaded[512];
static u8 *g_overlay_data_image[512];

/* Pristine copies of every overlay's .data, taken before the game runs. */
void kh_overlay_snapshot(void)
{
    u32 id, total = 0;
    for (id = 0; id < kh_overlay_count; id++) {
        const KhOverlayInfo *o = &kh_overlay_info[id];
        u32 n = (u32)(o->data_end - o->data_start);
        if (!o->data_start || !n)
            continue;
        g_overlay_data_image[id] = kh_alloc(n, 16, KH_LIFE_GLOBAL, KH_MEM_MISC);
        if (!g_overlay_data_image[id])
            kh_panic("no memory for overlay %u .data image", (unsigned)id);
        memcpy(g_overlay_data_image[id], o->data_start, n);
        total += n;
    }
    KH_INFO("fs", "overlay .data images: %u KiB; retained %u symbols",
            (unsigned)(total / 1024),
            (unsigned)(__kh_overlay_retain_end - __kh_overlay_retain_start));
}

/* As on the DS: loading an overlay gives it fresh .data and zeroed .bss. */
int FS_LoadOverlay(int target, u32 id)
{
    const KhOverlayInfo *o;
    (void)target;
    if (id >= kh_overlay_count) {
        KH_ERR("fs", "FS_LoadOverlay(%u): no such overlay", (unsigned)id);
        return 0;
    }
#if KH_PS2_DEBUG
    if (id == 12) {
        extern void kh_debug_stage(const char *stage, int a, int b);
        kh_debug_stage("FS ov012: begin load", (int)id, 0);
    }
#endif
    o = &kh_overlay_info[id];
#if KH_PS2_DEBUG
    if (id == 12) {
        extern void kh_debug_stage(const char *stage, int a, int b);
        kh_debug_stage("FS ov012: restore .data", (int)(o->data_end - o->data_start), 0);
    }
#endif
    if (g_overlay_data_image[id])
        memcpy(o->data_start, g_overlay_data_image[id], (size_t)(o->data_end - o->data_start));
#if KH_PS2_DEBUG
    if (id == 12) {
        extern void kh_debug_stage(const char *stage, int a, int b);
        kh_debug_stage("FS ov012: clear .bss", (int)(o->bss_end - o->bss_start), 0);
    }
#endif
    if (o->bss_start)
        memset(o->bss_start, 0, (size_t)(o->bss_end - o->bss_start));
#if KH_PS2_DEBUG
    if (id == 12) {
        extern void kh_debug_stage(const char *stage, int a, int b);
        kh_debug_stage("FS ov012: clear .cbss", (int)(o->cbss_end - o->cbss_start), 0);
    }
#endif
    if (o->cbss_start && o->cbss_end > o->cbss_start)
        memset(o->cbss_start, 0, (size_t)(o->cbss_end - o->cbss_start));
    g_overlay_loaded[id] = 1;
#if KH_PS2_DEBUG
    if (id == 12) {
        extern void kh_debug_stage(const char *stage, int a, int b);
        kh_debug_stage("FS ov012: load complete", (int)id, 0);
    }
#endif
    KH_INFO("fs", "overlay %u loaded", (unsigned)id);
    return 1;
}

int FS_UnloadOverlay(int target, u32 id)
{
    (void)target;
    if (id < kh_overlay_count)
        g_overlay_loaded[id] = 0;
    return 1;
}

int kh_overlay_is_loaded(u32 id) { return id < kh_overlay_count && g_overlay_loaded[id]; }

/* The two-step API (LoadOverlayInfo + LoadOverlayImage + StartOverlay) some game code uses. */
typedef struct {
    u32 id;
    void (*ram_address)(void);
    u32 ram_size;
    u32 bss_size;
    u32 sinit_init, sinit_init_end;
    u32 file_id, compressed_size, target, file_pos_top, file_pos_bottom;
} FSOverlayInfo;
typedef char KhOverlayInfoSizeMustBe2c[(sizeof(FSOverlayInfo) == 0x2c) ? 1 : -1];

int FS_LoadOverlayInfo(FSOverlayInfo *info, int target, u32 id)
{
    memset(info, 0, sizeof *info);
    info->id = id;
    info->target = (u32)target;
    if (id >= kh_overlay_count)
        return 0;
    info->ram_address = kh_overlay_info[id].entry;
    info->ram_size = kh_overlay_info[id].ram_size;
    info->bss_size = kh_overlay_info[id].bss_size;
    return 1;
}

int kh_overlay_call_entry(u32 id)
{
    if (id >= kh_overlay_count || !kh_overlay_info[id].entry) {
        KH_ERR("fs", "overlay %u has no native entry", (unsigned)id);
        return 0;
    }
    kh_overlay_info[id].entry();
    return 1;
}

int FS_LoadOverlayImage(FSOverlayInfo *info) { return FS_LoadOverlay(0, info->id); }
int FS_LoadOverlayImageAsync(FSOverlayInfo *info, void *file) { (void)file; return FS_LoadOverlay(0, info->id); }
void FS_StartOverlay(FSOverlayInfo *info) { (void)info; }
void FS_EndOverlay(FSOverlayInfo *info) { if (info->id < kh_overlay_count) g_overlay_loaded[info->id] = 0; }
int FS_UnloadOverlayImage(FSOverlayInfo *info) { FS_EndOverlay(info); return 1; }
void FS_ClearOverlayImage(FSOverlayInfo *info) { (void)info; }
