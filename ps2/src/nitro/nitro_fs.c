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
#include "nitro_internal.h"

#include <string.h>

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
static KhPakHeader g_hdr;

static int rom_read(FSArchive *arc, void *dst, u32 pos, u32 size)
{
    (void)arc;
    if (!g_pak || kh_file_seek(g_pak, pos) < 0 || kh_file_read(g_pak, dst, size) != (int32_t)size) {
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

static void open_pak(void)
{
    const char *path = "ps2data/khdays.pak";
    g_pak = kh_file_open(path, 0);
    if (!g_pak)
        kh_panic("Game data not found: %s%s\n\n  Create it from your own Kingdom Hearts 358/2 Days ROM:\n"
                 "    python ps2/tools/make_ps2data.py your_rom.nds ps2data\n"
                 "  and copy the ps2data folder next to the ELF.", kh_vfs_boot_dir(), path);
    if (kh_file_read(g_pak, &g_hdr, sizeof g_hdr) != (int32_t)sizeof g_hdr || g_hdr.magic != PAK_MAGIC)
        kh_panic("%s is not a KH Days PS2 data pack (re-run make_ps2data.py)", path);
    if (g_hdr.version != 1)
        kh_panic("%s has pack version %u, this build needs 1 (re-run make_ps2data.py)", path, (unsigned)g_hdr.version);
    KH_INFO("fs", "data pack %.4s: %u files, %u KiB", g_hdr.gamecode, (unsigned)g_hdr.file_count,
            (unsigned)(kh_file_size(g_pak) / 1024));
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
    u8 *bss_start, *bss_end;      /* .bss laid out from the symbol map */
    u8 *data_start, *data_end;    /* .data of the overlay's C objects */
    u8 *cbss_start, *cbss_end;    /* .bss/COMMON of the overlay's C objects */
} KhOverlayInfo;
extern const KhOverlayInfo kh_overlay_info[];      /* ps2/gen/stubs/overlays.c */
extern const u32 kh_overlay_count;

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
    KH_INFO("fs", "overlay .data images: %u KiB", (unsigned)(total / 1024));
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
    o = &kh_overlay_info[id];
    if (g_overlay_data_image[id])
        memcpy(o->data_start, g_overlay_data_image[id], (size_t)(o->data_end - o->data_start));
    if (o->bss_start)
        memset(o->bss_start, 0, (size_t)(o->bss_end - o->bss_start));
    if (o->cbss_start && o->cbss_end > o->cbss_start)
        memset(o->cbss_start, 0, (size_t)(o->cbss_end - o->cbss_start));
    g_overlay_loaded[id] = 1;
    KH_DBG("fs", "overlay %u loaded", (unsigned)id);
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
typedef struct { u32 id; u32 pad[15]; } FSOverlayInfo;

int FS_LoadOverlayInfo(FSOverlayInfo *info, int target, u32 id)
{
    (void)target;
    memset(info, 0, sizeof *info);
    info->id = id;
    return id < kh_overlay_count;
}

int FS_LoadOverlayImage(FSOverlayInfo *info) { return FS_LoadOverlay(0, info->id); }
int FS_LoadOverlayImageAsync(FSOverlayInfo *info, void *file) { (void)file; return FS_LoadOverlay(0, info->id); }
void FS_StartOverlay(FSOverlayInfo *info) { (void)info; }
void FS_EndOverlay(FSOverlayInfo *info) { if (info->id < kh_overlay_count) g_overlay_loaded[info->id] = 0; }
int FS_UnloadOverlayImage(FSOverlayInfo *info) { FS_EndOverlay(info); return 1; }
void FS_ClearOverlayImage(FSOverlayInfo *info) { (void)info; }
