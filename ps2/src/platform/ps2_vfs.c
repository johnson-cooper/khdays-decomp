/* Virtual filesystem: boot-path detection and buffered file access.
 *
 * The ELF never assumes where it lives.  argv[0] tells us the device and directory:
 *     mass:/khdays/khdays-ps2.elf        (USB, BDM; also mass0: .. mass9:)
 *     mmce0:/khdays/khdays-ps2.elf       (MMCE SD card adapters)
 *     hdd0:__common:pfs:/khdays/x.elf    (internal HDD partition, uLaunchELF style)
 *     pfs0:/khdays/x.elf                 (launcher-mounted partition; we remount it)
 *     host:C:/dev/khdays/x.elf           (PCSX2 host / ps2link)
 *     cdrom0:\KHDAYS.ELF;1
 *     mc0:/APPS/khdays-ps2.elf
 * Relative paths ("ps2data/khdays.pak") resolve against that directory.  I/O uses the POSIX
 * calls of the PS2SDK newlib port; once fileXioInit() has run they are routed through iomanX /
 * fileXio (mass, mmce, pfs, and the legacy ioman devices host:, cdrom0:, mc0:).  fileXio is only
 * called directly for what POSIX cannot express (mounting a HDD partition).
 *
 * The scene-data handle has sixteen 16 KiB read-ahead windows (the same 256 KiB budget as the old
 * two 128 KiB windows).  Real-hardware traces showed 175 KiB of logical requests causing 21.6 MiB
 * of reads: 174 tiny/random misses each pulled 128 KiB and the two slots thrashed.  More, smaller
 * slots retain scattered hot regions and cap a small miss at 16 KiB; requests at least that large
 * bypass the cache and read exactly into their destination.  Streamed music has its own descriptor
 * and keeps two 128 KiB windows because its planar stereo reads alternate between two long runs.
 *
 * Seek cursors.  USB/BDM storage goes through bdmfs_fatfs, whose FatFs is built with
 * FF_USE_FASTSEEK 0 and FF_FS_TINY 1: an lseek to the same or a later cluster walks forward from
 * the current cluster, but any seek BACKWARDS walks the FAT chain from the first cluster of the
 * file, one FAT sector read per 128 clusters, through a single shared sector buffer.  Deep into a
 * multi-hundred-MiB khdays.pak that is ~100 USB sector reads per backward seek; hardware traces
 * showed raw 16 KiB reads averaging 257 ms (722 seeks in 1087 reads) after loading a save, whose
 * data lies far into the pack, and two readers (game data and streamed music, which alternates
 * left/right channel regions) kept seeking each other backwards.  So a read-only file keeps up to
 * KH_MAX_CURSORS descriptors, each with its own device position: a read uses the descriptor at or
 * just before the target (a short forward walk, usually none), and another descriptor is opened
 * only when every existing one is already past the target.
 */
#define NEWLIB_PORT_AWARE   /* only for fileXioInit/Mount; all file I/O below is POSIX */
#include "platform/kh_platform.h"
#include "platform/kh_loadprof.h"
#include "ps2_internal.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <kernel.h>
#include <delaythread.h>
#include <io_common.h>
#include <fileXio_rpc.h>

#define SCENE_READAHEAD (16 * 1024)
#define SCENE_READ_WINDOWS 16
#define STREAM_READAHEAD (128 * 1024)
#define STREAM_READ_WINDOWS 2
#define MAX_READ_WINDOWS SCENE_READ_WINDOWS
#define KH_MAX_CURSORS 6
#define SCENE_CURSORS KH_MAX_CURSORS
#define STREAM_CURSORS 2         /* left and right channel runs */

typedef struct KhCursor {
    int fd;                /* -1: not opened yet */
    uint32_t raw_pos;      /* known device position of this descriptor */
    int raw_pos_valid;
    uint32_t age;
} KhCursor;

typedef struct KhReadWindow {
    uint32_t pos;
    uint32_t len;
    uint32_t age;
    uint8_t *data;
} KhReadWindow;

struct KhFile {
    int fd;                /* cursor 0's descriptor (the only one for writable files) */
    int lock_sema;        /* serializes fd position and read-ahead state across EE threads */
    int writable;
    uint32_t size;
    uint32_t pos;          /* logical position */
    uint32_t raw_pos;      /* known physical fd position, to avoid redundant lseek RPCs */
    uint32_t cache_age;
    uint32_t read_ahead;
    int read_windows;
    int raw_pos_valid;
    int profile_pack;      /* khdays.pak: include reads in the scene load profile */
    uint8_t *cache;
    KhReadWindow win[MAX_READ_WINDOWS];
    int ncursors;          /* descriptors this file may use (1 for writable files) */
    uint32_t cursor_age;
    KhCursor cur[KH_MAX_CURSORS];
    char full_path[320];
    int low_priority;      /* streamed music: yields the device to game-data reads */
    /* raw-read statistics (all files; shown for the music stream on the debug screens) */
    uint32_t st_raw, st_seeks, st_defers;
    uint64_t st_bytes, st_io_us;
};

/* Game-data reads waiting for or using the device right now.  fileXio serializes every EE file
 * RPC behind one lock, so a music refill issued while the game is loading makes the game wait
 * for the whole refill.  The music stream buffers far more than the deferral below. */
static volatile int g_hi_io_busy;
#define LOW_PRIORITY_MAX_DEFER_MS 100

static KhFile *g_stream_file;   /* the music stream descriptor, for the debug screens */

static char g_boot_dir[256] = "host:";
static char g_boot_dev[16] = "host";
static int g_xio_ready;

const char *kh_vfs_boot_dir(void) { return g_boot_dir; }
const char *kh_vfs_boot_device(void) { return g_boot_dev; }

static void parse_boot_path(const char *argv0)
{
    const char *colon, *slash;
    size_t n;

    if (!argv0 || !*argv0 || !strchr(argv0, ':')) {
        KH_WARN("vfs", "no usable boot path (argv[0]=%s); assuming host:", argv0 ? argv0 : "(null)");
        return;
    }
    colon = strchr(argv0, ':');
    n = (size_t)(colon - argv0);
    if (n >= sizeof g_boot_dev)
        n = sizeof g_boot_dev - 1;
    memcpy(g_boot_dev, argv0, n);
    g_boot_dev[n] = 0;
    while (n && isdigit((unsigned char)g_boot_dev[n - 1]))
        g_boot_dev[--n] = 0;

    /* directory part: everything up to the last separator (keeps "mass:" for root files) */
    slash = strrchr(argv0, '/');
    {
        const char *bs = strrchr(argv0, '\\');
        if (bs && (!slash || bs > slash))
            slash = bs;
    }
    if (!slash || slash < colon) {
        /* No directory separator.  PCSX2 passes "host:E:dirdirfile.elf" (separators stripped)
         * and roots host: at the ELF's folder, so the directory is the device root. */
        slash = colon;
    }
    n = (size_t)(slash - argv0) + 1;
    if (n >= sizeof g_boot_dir)
        n = sizeof g_boot_dir - 1;
    memcpy(g_boot_dir, argv0, n);
    g_boot_dir[n] = 0;
}

/* "hdd0:PART:pfs:/dir/" -> mount PART on pfs0: and use "pfs0:/dir/" */
static void mount_hdd_boot_dir(void)
{
    char part[64], rest[192];
    const char *p = g_boot_dir + 5, *q;   /* after "hdd0:" */
    if (strncmp(g_boot_dir, "hdd", 3) != 0)
        return;
    q = strstr(p, ":pfs:");
    if (!q) {
        KH_WARN("vfs", "unrecognised HDD boot path %s", g_boot_dir);
        return;
    }
    snprintf(part, sizeof part, "hdd0:%.*s", (int)(q - p), p);
    snprintf(rest, sizeof rest, "%s", q + 5);
    fileXioUmount("pfs0:");
    if (fileXioMount("pfs0:", part, FIO_MT_RDWR) < 0) {
        KH_ERR("vfs", "cannot mount %s", part);
        return;
    }
    snprintf(g_boot_dir, sizeof g_boot_dir, "pfs0:%s", rest);
    strcpy(g_boot_dev, "pfs");
}

int kh_vfs_init(int argc, char **argv)
{
    parse_boot_path(argc > 0 ? argv[0] : NULL);
    KH_INFO("vfs", "boot device '%s', boot dir '%s'", g_boot_dev, g_boot_dir);

    if (ps2_iop_load_device_drivers(!strcmp(g_boot_dev, "pfs") ? "hdd" : g_boot_dev) < 0)
        KH_WARN("vfs", "some drivers for '%s' failed to load", g_boot_dev);
    if (ps2_iop_have_filexio()) {
        g_xio_ready = fileXioInit() >= 0;
        if (!g_xio_ready)
            KH_WARN("vfs", "fileXioInit failed, using fio only");
    }
    if (!strcmp(g_boot_dev, "hdd"))
        mount_hdd_boot_dir();
    if (!strcmp(g_boot_dev, "pfs") && strncmp(g_boot_dir, "pfs0:", 5) != 0)
        KH_WARN("vfs", "booted from a launcher-mounted partition; its mount was lost with the IOP reset");

    /* USB mass storage needs a moment to enumerate after bdm loads. */
    if (!strcmp(g_boot_dev, "mass")) {
        int tries;
        for (tries = 0; tries < 100; tries++) {
            char probe[300];
            int fd;
            snprintf(probe, sizeof probe, "%s", g_boot_dir);
            {
                struct stat st;
                fd = stat(probe, &st);
            }
            if (fd >= 0)
                break;
            kh_vblank_wait();
        }
    }
    return 0;
}

void kh_vfs_resolve(const char *rel, char *out, size_t outsz)
{
    if (strchr(rel, ':'))
        snprintf(out, outsz, "%s", rel);
    else
        snprintf(out, outsz, "%s%s", g_boot_dir, rel);
    /* cdrom0: wants backslashes and ;1 version suffixes; the pack name is chosen to fit 8.3 */
    if (!strncmp(out, "cdrom", 5)) {
        char *c;
        for (c = out; *c; c++) {
            if (*c == '/')
                *c = '\\';
            else
                *c = (char)toupper((unsigned char)*c);
        }
        if (!strchr(out, ';') && strlen(out) + 2 < outsz)
            strcat(out, ";1");
    }
}


/* device calls with EE interrupts on (kh_io_begin) */
static int io_open(const char *p, int fl, int mode) { int w = kh_io_begin_tag("VFS file open"); int r = open(p, fl, mode); kh_io_end(w); return r; }
static int io_write(int fd, const void *b, unsigned n) { int w = kh_io_begin_tag("VFS file write"); int r = (int)write(fd, b, n); kh_io_end(w); return r; }
static off_t io_lseek(int fd, off_t o, int wh) { int w = kh_io_begin_tag("VFS file seek"); off_t r = lseek(fd, o, wh); kh_io_end(w); return r; }
static int io_close(int fd) { int w = kh_io_begin_tag("VFS file close"); int r = close(fd); kh_io_end(w); return r; }
static int io_stat(const char *p, struct stat *st) { int w = kh_io_begin_tag("VFS file stat"); int r = stat(p, st); kh_io_end(w); return r; }
static int io_rename(const char *a, const char *b) { int w = kh_io_begin_tag("VFS file rename"); int r = rename(a, b); kh_io_end(w); return r; }
static int io_unlink(const char *p) { int w = kh_io_begin_tag("VFS file remove"); int r = unlink(p); kh_io_end(w); return r; }
static int io_mkdir(const char *p, int m) { int w = kh_io_begin_tag("VFS directory create"); int r = mkdir(p, m); kh_io_end(w); return r; }

static void file_lock(KhFile *f)
{
    int w = kh_io_begin_tag("VFS file-handle lock");
    WaitSema(f->lock_sema);
    kh_io_end(w);
}

static void file_unlock(KhFile *f)
{
    SignalSema(f->lock_sema);
}

static KhFile *file_open(const char *path, int write, uint32_t read_ahead,
                         int read_windows, int profile_pack, int cursors)
{
    char full[320];
    KhFile *f;
    ee_sema_t sema;
    int fd;

    kh_vfs_resolve(path, full, sizeof full);
    {
        extern volatile const char *kh_watchdog_mark;
        static char mark[96];
        snprintf(mark, sizeof mark, "file open %s", path);
        kh_watchdog_mark = mark;
    }
    fd = io_open(full, write ? (O_WRONLY | O_CREAT | O_TRUNC) : O_RDONLY, 0666);
    if (fd < 0)
        return NULL;

    f = kh_alloc(sizeof *f, 16, KH_LIFE_GLOBAL, KH_MEM_FILE_CACHE);
    if (!f) {
        io_close(fd);
        return NULL;
    }
    memset(f, 0, sizeof *f);
    memset(&sema, 0, sizeof sema);
    sema.init_count = 1;
    sema.max_count = 1;
    f->lock_sema = CreateSema(&sema);
    if (f->lock_sema < 0) {
        io_close(fd);
        kh_free(f);
        return NULL;
    }
    f->fd = fd;
    snprintf(f->full_path, sizeof f->full_path, "%s", full);
    {
        int c;
        for (c = 0; c < KH_MAX_CURSORS; c++)
            f->cur[c].fd = -1;
        f->cur[0].fd = fd;
        if (cursors < 1 || write)
            cursors = 1;
        if (cursors > KH_MAX_CURSORS)
            cursors = KH_MAX_CURSORS;
        f->ncursors = cursors;
    }
    f->writable = write;
    f->read_ahead = read_ahead;
    f->read_windows = read_windows;
    f->profile_pack = !write && profile_pack && strstr(path, "khdays.pak") != NULL;
    if (!write) {
        int i;
        off_t sz = io_lseek(fd, 0, SEEK_END);
        if (io_lseek(fd, 0, SEEK_SET) == 0) {
            f->raw_pos = 0;
            f->raw_pos_valid = 1;
            f->cur[0].raw_pos = 0;
            f->cur[0].raw_pos_valid = 1;
        }
        f->size = sz < 0 ? 0 : (uint32_t)sz;
        if (read_windows > MAX_READ_WINDOWS)
            read_windows = MAX_READ_WINDOWS;
        f->read_windows = read_windows;
        f->cache = kh_alloc(read_ahead * read_windows, 64, KH_LIFE_GLOBAL, KH_MEM_FILE_CACHE);
        if (f->cache) {
            for (i = 0; i < read_windows; i++)
                f->win[i].data = f->cache + i * read_ahead;
        }
        /* Seek cursors only pay off on big files (the pack); open them all now, on the opening
         * thread.  Opening them lazily from the music worker raced the game thread's own file
         * calls inside the C library's descriptor table. */
        if (f->size < (4u << 20))
            f->ncursors = 1;
        for (i = 1; i < f->ncursors; i++) {
            int extra = io_open(full, O_RDONLY, 0666);
            if (extra < 0) {
                f->ncursors = i;
                break;
            }
            f->cur[i].fd = extra;
            f->cur[i].raw_pos = 0;          /* a fresh descriptor starts at offset 0 */
            f->cur[i].raw_pos_valid = 1;
        }
    }
    return f;
}

KhFile *kh_file_open(const char *path, int write)
{
    return file_open(path, write, SCENE_READAHEAD, SCENE_READ_WINDOWS, 1, SCENE_CURSORS);
}

KhFile *kh_file_open_stream(const char *path)
{
    /* A separate descriptor is essential: lseek/read positions and read-ahead metadata are
     * mutable.  Stream data is planar, so the worker alternates between distant left/right
     * regions.  Preserve the original two 128 KiB windows for that access pattern; applying the
     * scene loader's 16 KiB random-read policy here causes constant physical refills and audible
     * starvation on a real console. */
    KhFile *f = file_open(path, 0, STREAM_READAHEAD, STREAM_READ_WINDOWS, 0, STREAM_CURSORS);
    if (f) {
        f->low_priority = 1;
        g_stream_file = f;
    }
    return f;
}

/* The cursor to read `off` with: the descriptor at or nearest before it (FatFs walks forward from
 * there), else the least recently used one. */
static KhCursor *pick_cursor(KhFile *f, uint32_t off)
{
    KhCursor *best = NULL, *lru = NULL;
    int c;
    for (c = 0; c < f->ncursors; c++) {
        KhCursor *k = &f->cur[c];
        if (k->fd < 0)
            continue;
        if (k->raw_pos_valid && k->raw_pos <= off && (!best || k->raw_pos > best->raw_pos))
            best = k;
        if (!lru || k->age < lru->age)
            lru = k;
    }
    return best ? best : lru;
}

/* The device's file position is shared by lseek/read, so the pair must live in one I/O
 * critical region.  Do not implement this in terms of io_lseek()+io_read(): that would
 * release ownership between the two operations. */
static int raw_read_at(KhFile *f, uint32_t off, void *dst, uint32_t n,
                       uint32_t req_off, uint32_t req_size, int window)
{
    uint64_t start = 0;
    KhCursor *k = pick_cursor(f, off);
    uint64_t t0;
    int seek;
    int w;
    int r;
    if (!k)
        return -1;
    if (f->low_priority) {
        int waited = 0;
        if (g_hi_io_busy)
            f->st_defers++;
        while (g_hi_io_busy && waited < LOW_PRIORITY_MAX_DEFER_MS) {
            DelayThread(2000);
            waited += 2;
        }
    } else {
        int old = DIntr();
        g_hi_io_busy++;
        if (old)
            EIntr();
    }
    k->age = ++f->cursor_age;
    seek = !k->raw_pos_valid || k->raw_pos != off;
    t0 = kh_time_us();
    if (f->profile_pack) {
        kh_loadprof_raw_begin(req_off, req_size, off, n, window);
        if (seek)
            kh_loadprof_seek();
        start = kh_time_us();
    }
    w = kh_io_begin_tag(seek ? "VFS raw seek+read" : "VFS raw sequential read");
    if (seek && lseek(k->fd, (off_t)off, SEEK_SET) < 0) {
        k->raw_pos_valid = 0;
        r = -1;
    } else {
        r = (int)read(k->fd, dst, n);
        if (r > 0) {
            k->raw_pos = off + (uint32_t)r;
            k->raw_pos_valid = 1;
        } else {
            k->raw_pos_valid = 0;
        }
    }
    kh_io_end(w);
    f->st_raw++;
    if (seek)
        f->st_seeks++;
    if (r > 0)
        f->st_bytes += (uint32_t)r;
    f->st_io_us += kh_time_us() - t0;
    if (!f->low_priority) {
        int old = DIntr();
        g_hi_io_busy--;
        if (old)
            EIntr();
    }
    if (f->profile_pack)
        kh_loadprof_raw_end(r, (uint32_t)(kh_time_us() - start));
    return r;
}

/* Music-stream I/O so far, for the debug overlay and the stall screen. */
int kh_vfs_stream_stats(char *out, int n)
{
    const KhFile *f = g_stream_file;
    if (!f)
        return snprintf(out, (size_t)n, "stream: -");
    return snprintf(out, (size_t)n, "stream: raw %u %uK seek %u defer %u io %ums avg %uus",
                    (unsigned)f->st_raw, (unsigned)(f->st_bytes / 1024u), (unsigned)f->st_seeks,
                    (unsigned)f->st_defers, (unsigned)(f->st_io_us / 1000u),
                    (unsigned)(f->st_raw ? f->st_io_us / f->st_raw : 0));
}

static KhReadWindow *find_window(KhFile *f, uint32_t pos)
{
    int i;
    for (i = 0; i < f->read_windows; i++) {
        KhReadWindow *w = &f->win[i];
        if (w->data && w->len && pos >= w->pos && pos < w->pos + w->len) {
            w->age = ++f->cache_age;
            return w;
        }
    }
    return NULL;
}

static KhReadWindow *pick_window(KhFile *f)
{
    int i;
    KhReadWindow *oldest = NULL;
    for (i = 0; i < f->read_windows; i++) {
        KhReadWindow *w = &f->win[i];
        if (!w->data)
            continue;
        if (!w->len)
            return w;
        if (!oldest || w->age < oldest->age)
            oldest = w;
    }
    return oldest;
}

static int32_t file_read_at(KhFile *f, uint32_t offset, void *dst, uint32_t size)
{
    uint8_t *d = dst;
    uint32_t pos = offset;
    uint32_t done = 0;

    if (!f || f->writable || pos >= f->size)
        return 0;
    if (size > f->size - pos)
        size = f->size - pos;
    if (f->profile_pack)
        kh_loadprof_logical(offset, size);

    kh_prof_begin(KH_PROF_LOAD);
    while (done < size) {
        uint32_t want = size - done;
        KhReadWindow *w = find_window(f, pos);
        if (w) {
            uint32_t avail = w->pos + w->len - pos;
            uint32_t n = want < avail ? want : avail;
            if (f->profile_pack)
                kh_loadprof_cache_hit();
            memcpy(d + done, w->data + (pos - w->pos), n);
            done += n;
            pos += n;
        } else if (want >= f->read_ahead || !f->cache) {
            /* big read: straight into the destination, no copy */
            int r;
            if (f->profile_pack) {
                kh_loadprof_cache_miss();
                kh_loadprof_direct_read();
            }
            r = raw_read_at(f, pos, d + done, want, offset, size, -1);
            if (r <= 0)
                break;
            done += (uint32_t)r;
            pos += (uint32_t)r;
        } else {
            /* Refill the unused/LRU window.  Keep the device-friendly 2 KiB alignment. */
            uint32_t start = pos & ~2047u;
            KhReadWindow *victim = pick_window(f);
            int r;
            int window;
            if (f->profile_pack) {
                kh_loadprof_cache_miss();
                kh_loadprof_window_refill();
            }
            if (!victim)
                break;
            window = (int)(victim - f->win);
            r = raw_read_at(f, start, victim->data, f->read_ahead, offset, size, window);
            if (r <= 0 || start + (uint32_t)r <= pos)
                break;
            victim->pos = start;
            victim->len = (uint32_t)r;
            victim->age = ++f->cache_age;
        }
    }
    kh_prof_end(KH_PROF_LOAD);
    return (int32_t)done;
}

int32_t kh_file_read_at(KhFile *f, uint32_t offset, void *dst, uint32_t size)
{
    int32_t r;
    if (!f)
        return 0;
    file_lock(f);
    r = file_read_at(f, offset, dst, size);
    file_unlock(f);
    return r;
}

int32_t kh_file_read(KhFile *f, void *dst, uint32_t size)
{
    int32_t r;
    if (!f)
        return 0;
    file_lock(f);
    r = file_read_at(f, f->pos, dst, size);
    if (r > 0)
        f->pos += (uint32_t)r;
    file_unlock(f);
    return r;
}

int32_t kh_file_write(KhFile *f, const void *src, uint32_t size)
{
    /* devices may accept less than asked (mc0: writes in cluster-sized pieces) */
    const uint8_t *s = src;
    uint32_t done = 0;
    if (!f)
        return -1;
    file_lock(f);
    while (done < size) {
        int r = (int)io_write(f->fd, s + done, size - done);
        if (r <= 0) {
            int32_t result = done ? (int32_t)done : r;
            file_unlock(f);
            return result;
        }
        done += (uint32_t)r;
        f->pos += (uint32_t)r;
    }
    file_unlock(f);
    return (int32_t)done;
}

int32_t kh_file_seek(KhFile *f, uint32_t pos)
{
    if (!f)
        return -1;
    file_lock(f);
    if (f->writable && io_lseek(f->fd, (off_t)pos, SEEK_SET) < 0) {
        file_unlock(f);
        return -1;
    }
    f->pos = pos;
    file_unlock(f);
    return (int32_t)pos;
}

uint32_t kh_file_tell(KhFile *f)
{
    uint32_t pos;
    if (!f)
        return 0;
    file_lock(f);
    pos = f->pos;
    file_unlock(f);
    return pos;
}
uint32_t kh_file_size(KhFile *f) { return f->size; }

int kh_file_close(KhFile *f)
{
    int r;
    if (!f)
        return 0;
    file_lock(f);
    r = io_close(f->fd);
    {
        int c;
        for (c = 1; c < KH_MAX_CURSORS; c++)
            if (f->cur[c].fd >= 0)
                io_close(f->cur[c].fd);
    }
    file_unlock(f);
    if (f == g_stream_file)
        g_stream_file = NULL;
    DeleteSema(f->lock_sema);
    kh_free(f->cache);
    kh_free(f);
    return r < 0 ? r : 0;
}

int kh_file_exists(const char *path)
{
    char full[320];
    struct stat st;
    kh_vfs_resolve(path, full, sizeof full);
    return io_stat(full, &st) == 0;
}

int kh_file_rename(const char *from, const char *to)
{
    char a[320], b[320];
    kh_vfs_resolve(from, a, sizeof a);
    kh_vfs_resolve(to, b, sizeof b);
    return io_rename(a, b);
}

int kh_file_remove(const char *path)
{
    char a[320];
    kh_vfs_resolve(path, a, sizeof a);
    return io_unlink(a);
}

int kh_file_mkdir(const char *path)
{
    char a[320];
    kh_vfs_resolve(path, a, sizeof a);
    return io_mkdir(a, 0777);
}
