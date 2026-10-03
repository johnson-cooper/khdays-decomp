/* Logging for the PS2 port.
 *
 * During boot every line goes to stdout and khdays.log so hardware bring-up leaves a trace.
 * Once boot is complete, slow devices (USB/FAT) retain lines in EE RAM and do no IOP I/O; opening,
 * writing and closing the log even once a second can starve real-hardware audio.  Fast host:
 * logging (PCSX2/ps2link) remains live.  EE SIO output is compiled in with -DKH_LOG_SIO; it is
 * off by default because without a cable the SIO still drains at the baud rate and would stall.
 * A small ring of recent lines is kept for the panic screen.
 */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <kernel.h>
#include <debug.h>
#include <fcntl.h>
#include <unistd.h>
#ifdef KH_LOG_SIO
#include <sio.h>
#endif

#define RING_LINES 24
#define LINE_MAX_LEN 160

/* Release builds keep warnings, errors and panic diagnostics, but routine tracing is opt-in.
 * Besides its formatting cost, live host: output can stall an emulator at each file flush. */
static int g_level = KH_PS2_DEBUG ? KH_LOG_INFO : KH_LOG_WARN;
static char g_ring[RING_LINES][LINE_MAX_LEN];
static int g_ring_head;
/* The log file is not kept open: lines collect in RAM and a flush opens, appends and closes it.
 * A larger buffer lets slow hardware retain several minutes after the synchronous boot trace
 * without touching the IOP; panic, restart and orderly shutdown explicitly flush it. */
#define LOGBUF_SIZE (64 * 1024)
static char g_logbuf[LOGBUF_SIZE];
static int g_logbuf_len;
static int g_log_ok;            /* the file could be created */
static int g_log_slow_device;   /* mass:/ and memory-card paths; host: is fast */
static int g_log_mutex = -1;    /* buffer and file are used by one thread at a time */
static char g_logpath[320];
static uint32_t g_log_synced;
static int g_in_log;

static const char *const k_level_tag[] = { "E", "W", "I", "D", "T" };

void kh_log_set_level(int level) { g_level = level; }

void ps2_log_open_file(void)
{
    int fd;
    if (g_log_mutex >= 0)
        return;
    kh_vfs_resolve("khdays.log", g_logpath, sizeof g_logpath);
    g_log_slow_device = strncmp(g_logpath, "host:", 5) != 0;
    fd = open(g_logpath, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd >= 0) {
        close(fd);
        g_log_ok = 1;
    }
    g_log_synced = kh_vblank_count();
    {
        ee_sema_t m;
        m.init_count = 1;
        m.max_count = 1;
        m.option = 0;
        g_log_mutex = CreateSema(&m);
    }
}

/* append the buffer to the file (open, write, close) */
static void flush_locked(void)
{
    int fd, w;
    g_log_synced = kh_vblank_count();
    if (!g_logbuf_len || !g_log_ok)
        return;
    w = kh_io_begin();
    fd = open(g_logpath, O_WRONLY | O_CREAT, 0666);
    if (fd >= 0) {
        lseek(fd, 0, SEEK_END);
        write(fd, g_logbuf, g_logbuf_len);
        close(fd);
    }
    kh_io_end(w);
    g_logbuf_len = 0;
}

void kh_log_flush(void)
{
    if (g_log_mutex < 0 || g_in_log == GetThreadId())
        return;
    WaitSema(g_log_mutex);
    g_in_log = GetThreadId();
    flush_locked();
    g_in_log = 0;
    SignalSema(g_log_mutex);
}

static void emit(const char *line)
{
    size_t n = strlen(line);
    int defer_io = g_log_slow_device && kh_vblank_count() >= KH_BOOT_TRACE_VBLANKS;
    /* the ring first: stdout and the file go through the IOP and may block if it hangs */
    strncpy(g_ring[g_ring_head], line, LINE_MAX_LEN - 1);
    g_ring[g_ring_head][LINE_MAX_LEN - 1] = 0;
    g_ring_head = (g_ring_head + 1) % RING_LINES;
    /* On slow hardware, post-boot output must not compete with audio for the IOP. */
    if (!defer_io) {
        int w = kh_io_begin();
        fputs(line, stdout);
        kh_io_end(w);
    }
#ifdef KH_LOG_SIO
    sio_puts(line);
#endif
    if (g_log_mutex >= 0 && g_in_log != GetThreadId()) {   /* (no recursion) */
        int w = kh_io_begin();
        WaitSema(g_log_mutex);
        kh_io_end(w);
        g_in_log = GetThreadId();
        if (g_logbuf_len + (int)n > LOGBUF_SIZE && !defer_io)
            flush_locked();
        if (g_logbuf_len + (int)n <= LOGBUF_SIZE) {
            memcpy(g_logbuf + g_logbuf_len, line, n);
            g_logbuf_len += (int)n;
        }
        /* Slow devices commit the boot trace, then retain later lines in RAM until an explicit
         * flush.  Fast host: paths keep the convenient once-per-second live log. */
        if (kh_vblank_count() < KH_BOOT_TRACE_VBLANKS
            || (!g_log_slow_device && kh_vblank_count() - g_log_synced >= 60))
            flush_locked();
        g_in_log = 0;
        SignalSema(g_log_mutex);
    }
}

/* a line into the recent-lines ring only (no stdout, no file: never touches the IOP) */
void ps2_log_ring(const char *line)
{
    strncpy(g_ring[g_ring_head], line, LINE_MAX_LEN - 1);
    g_ring[g_ring_head][LINE_MAX_LEN - 1] = 0;
    g_ring_head = (g_ring_head + 1) % RING_LINES;
}

/* the recent lines on the debug text screen (no IOP, no locks): for the hang watchdog */
void ps2_log_print_recent(int lines)
{
    int i;
    if (lines > RING_LINES)
        lines = RING_LINES;
    for (i = RING_LINES - lines; i < RING_LINES; i++) {
        const char *l = g_ring[(g_ring_head + i) % RING_LINES];
        if (l[0])
            scr_printf("  %s", l);
    }
}

void kh_log(int level, const char *sub, const char *fmt, ...)
{
    char line[LINE_MAX_LEN];
    int n;
    va_list ap;

    if (level > g_level)
        return;
    n = snprintf(line, sizeof line, "[%s %6u %s] ", k_level_tag[level & 3], (unsigned)kh_vblank_count(), sub);
    if (n < 0 || n >= (int)sizeof line)
        n = 0;
    va_start(ap, fmt);
    vsnprintf(line + n, sizeof line - n - 1, fmt, ap);
    va_end(ap);
    n = (int)strlen(line);
    if (n == 0 || line[n - 1] != '\n') {
        line[n] = '\n';
        line[n + 1] = 0;
    }
    emit(line);
}

void kh_panic(const char *fmt, ...)
{
    char msg[256];
    va_list ap;
    int i;

    va_start(ap, fmt);
    vsnprintf(msg, sizeof msg, fmt, ap);
    va_end(ap);
    kh_log(KH_LOG_ERROR, "PANIC", "%s", msg);
    kh_log_flush();

    /* Fall back to the SDK's debug console: it reprograms the GS itself, so it works whatever
     * state the renderer was left in. */
    init_scr();
    scr_clear();
    scr_printf("\n  Kingdom Hearts 358/2 Days (PS2) - fatal error\n\n  %s\n\n  Recent log:\n", msg);
    for (i = 0; i < RING_LINES; i++) {
        const char *l = g_ring[(g_ring_head + i) % RING_LINES];
        if (l[0])
            scr_printf("  %s", l);
    }
    for (;;)
        SleepThread();
}
