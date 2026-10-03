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
 * Each open file has a read-ahead buffer: the game's own loader reads 512-byte blocks, which
 * would otherwise become 512-byte device transactions.
 */
#define NEWLIB_PORT_AWARE   /* only for fileXioInit/Mount; all file I/O below is POSIX */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <kernel.h>
#include <io_common.h>
#include <fileXio_rpc.h>

#define READAHEAD (128 * 1024)

struct KhFile {
    int fd;
    int writable;
    uint32_t size;
    uint32_t pos;          /* logical position */
    uint32_t buf_pos;      /* file offset of buf[0] */
    uint32_t buf_len;      /* valid bytes in buf */
    uint8_t *buf;
};

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
static int io_open(const char *p, int fl, int mode) { int w = kh_io_begin(); int r = open(p, fl, mode); kh_io_end(w); return r; }
static int io_read(int fd, void *b, unsigned n) { int w = kh_io_begin(); int r = (int)read(fd, b, n); kh_io_end(w); return r; }
static int io_write(int fd, const void *b, unsigned n) { int w = kh_io_begin(); int r = (int)write(fd, b, n); kh_io_end(w); return r; }
static off_t io_lseek(int fd, off_t o, int wh) { int w = kh_io_begin(); off_t r = lseek(fd, o, wh); kh_io_end(w); return r; }
static int io_close(int fd) { int w = kh_io_begin(); int r = close(fd); kh_io_end(w); return r; }
static int io_stat(const char *p, struct stat *st) { int w = kh_io_begin(); int r = stat(p, st); kh_io_end(w); return r; }
static int io_rename(const char *a, const char *b) { int w = kh_io_begin(); int r = rename(a, b); kh_io_end(w); return r; }
static int io_unlink(const char *p) { int w = kh_io_begin(); int r = unlink(p); kh_io_end(w); return r; }
static int io_mkdir(const char *p, int m) { int w = kh_io_begin(); int r = mkdir(p, m); kh_io_end(w); return r; }

KhFile *kh_file_open(const char *path, int write)
{
    char full[320];
    KhFile *f;
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
    f->fd = fd;
    f->writable = write;
    if (!write) {
        off_t sz = io_lseek(fd, 0, SEEK_END);
        io_lseek(fd, 0, SEEK_SET);
        f->size = sz < 0 ? 0 : (uint32_t)sz;
        f->buf = kh_alloc(READAHEAD, 64, KH_LIFE_GLOBAL, KH_MEM_FILE_CACHE);
    }
    return f;
}

static int raw_read(KhFile *f, uint32_t off, void *dst, uint32_t n)
{
    if (io_lseek(f->fd, (off_t)off, SEEK_SET) < 0)
        return -1;
    return (int)io_read(f->fd, dst, n);
}

int32_t kh_file_read(KhFile *f, void *dst, uint32_t size)
{
    uint8_t *d = dst;
    uint32_t done = 0;

    if (f->pos >= f->size)
        return 0;
    if (size > f->size - f->pos)
        size = f->size - f->pos;
    kh_prof_begin(KH_PROF_LOAD);
    while (done < size) {
        uint32_t want = size - done;
        if (f->pos >= f->buf_pos && f->pos < f->buf_pos + f->buf_len) {
            uint32_t avail = f->buf_pos + f->buf_len - f->pos;
            uint32_t n = want < avail ? want : avail;
            memcpy(d + done, f->buf + (f->pos - f->buf_pos), n);
            done += n;
            f->pos += n;
        } else if (want >= READAHEAD || !f->buf) {
            /* big read: straight into the destination, no copy */
            int r = raw_read(f, f->pos, d + done, want);
            if (r <= 0)
                break;
            done += (uint32_t)r;
            f->pos += (uint32_t)r;
        } else {
            /* refill aligned to 2 KiB so device transfers stay sector-aligned */
            uint32_t start = f->pos & ~2047u;
            int r = raw_read(f, start, f->buf, READAHEAD);
            if (r <= 0 || start + (uint32_t)r <= f->pos)
                break;
            f->buf_pos = start;
            f->buf_len = (uint32_t)r;
        }
    }
    kh_prof_end(KH_PROF_LOAD);
    return (int32_t)done;
}

int32_t kh_file_write(KhFile *f, const void *src, uint32_t size)
{
    /* devices may accept less than asked (mc0: writes in cluster-sized pieces) */
    const uint8_t *s = src;
    uint32_t done = 0;
    while (done < size) {
        int r = (int)io_write(f->fd, s + done, size - done);
        if (r <= 0)
            return done ? (int32_t)done : r;
        done += (uint32_t)r;
        f->pos += (uint32_t)r;
    }
    return (int32_t)done;
}

int32_t kh_file_seek(KhFile *f, uint32_t pos)
{
    if (f->writable && io_lseek(f->fd, (off_t)pos, SEEK_SET) < 0)
        return -1;
    f->pos = pos;
    return (int32_t)pos;
}

uint32_t kh_file_tell(KhFile *f) { return f->pos; }
uint32_t kh_file_size(KhFile *f) { return f->size; }

int kh_file_close(KhFile *f)
{
    int r;
    if (!f)
        return 0;
    r = io_close(f->fd);
    kh_free(f->buf);
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
