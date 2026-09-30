/* Logging for the PS2 port.
 *
 * Every line goes to stdout (PCSX2's EE console, ps2link/host consoles) and, once the VFS is
 * up, to a log file next to the ELF (khdays.log), so real-hardware runs leave a trace.  EE SIO
 * output (for modded consoles with a serial cable) is compiled in with -DKH_LOG_SIO; it is off
 * by default because without a cable the SIO still drains at the baud rate and would stall.
 * A small ring of recent lines is kept for the panic screen.
 */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <kernel.h>
#include <debug.h>
#ifdef KH_LOG_SIO
#include <sio.h>
#endif

#define RING_LINES 24
#define LINE_MAX_LEN 160

static int g_level = KH_LOG_INFO;
static char g_ring[RING_LINES][LINE_MAX_LEN];
static int g_ring_head;
static KhFile *g_logfile;
static int g_in_log;

static const char *const k_level_tag[] = { "E", "W", "I", "D", "T" };

void kh_log_set_level(int level) { g_level = level; }

void ps2_log_open_file(void)
{
    if (g_logfile)
        return;
    g_logfile = kh_file_open("khdays.log", 1);
}

void kh_log_flush(void) { }

static void emit(const char *line)
{
    size_t n = strlen(line);
    fputs(line, stdout);
#ifdef KH_LOG_SIO
    sio_puts(line);
#endif
    if (g_logfile && !g_in_log) {
        g_in_log = 1;
        kh_file_write(g_logfile, line, (uint32_t)n);
        g_in_log = 0;
    }
    strncpy(g_ring[g_ring_head], line, LINE_MAX_LEN - 1);
    g_ring[g_ring_head][LINE_MAX_LEN - 1] = 0;
    g_ring_head = (g_ring_head + 1) % RING_LINES;
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
    if (g_logfile)
        kh_file_close(g_logfile);
    g_logfile = NULL;

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
