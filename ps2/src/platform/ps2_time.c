/* Time and VBlank.
 *
 * kh_time_ticks() is the PS2SDK timer-alarm system time (EE bus clock, 147.456 MHz, 64-bit).
 * VBlank: a VBLANK_START interrupt handler bumps a counter and signals a semaphore; waiters
 * block on the semaphore, so a waiting thread costs no CPU.  The game never runs inside the
 * interrupt.
 */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <kernel.h>
#include <timer.h>

static volatile uint32_t g_vblank_count;
static int g_vblank_sema = -1;
static int g_vblank_handler = -1;
static int g_refresh_hz = 60;

static int vblank_handler(int cause)
{
    (void)cause;
    g_vblank_count++;
    iSignalSema(g_vblank_sema);
    {
        extern void kh_watchdog_vblank(uint32_t vblank);
        kh_watchdog_vblank(g_vblank_count);
    }
    ExitHandler();
    return 0;
}

void kh_time_init(void)
{
    /* crt0 starts the timer subsystem; nothing else to do for the tick source. */
}

void ps2_time_install_vblank(void)
{
    ee_sema_t s;
    if (g_vblank_sema >= 0)
        return;
    s.init_count = 0;
    s.max_count = 1;
    s.option = 0;
    s.attr = 0;
    g_vblank_sema = CreateSema(&s);
    g_vblank_handler = AddIntcHandler(INTC_VBLANK_S, vblank_handler, 0);
    EnableIntc(INTC_VBLANK_S);
}

void ps2_time_set_refresh(int hz) { g_refresh_hz = hz; }

uint64_t kh_time_ticks(void) { return GetTimerSystemTime(); }

uint64_t kh_time_us(void) { return GetTimerSystemTime() / (KH_TICKS_PER_SEC / 1000000ull); }

void kh_time_sleep_us(uint32_t us)
{
    uint64_t end = kh_time_ticks() + (uint64_t)us * (KH_TICKS_PER_SEC / 1000000ull);
    /* Coarse waits sleep on VBlanks; the remainder spins (only short remainders reach here). */
    while (end > kh_time_ticks() + KH_TICKS_PER_SEC / 50)
        kh_vblank_wait();
    while (kh_time_ticks() < end)
        ;
}

static void vblank_wait(void);

void kh_vblank_wait(void)
{
    int w = kh_io_begin();
    vblank_wait();
    kh_io_end(w);
}

static void vblank_wait(void)
{
    if (g_vblank_sema < 0) {
        /* Before the handler exists (very early boot) poll the GS CSR VSINT bit. */
        volatile uint64_t *csr = (volatile uint64_t *)0x12001000;
        *csr = *csr & 8;
        while (!(*csr & 8))
            ;
        g_vblank_count++;
        return;
    }
    /* Drop every stale signal so we wait for the *next* VBlank, not one that already passed.  The
     * EE kernel does not clamp a semaphore to max_count: while a frame takes several VBlanks the
     * count keeps growing, and a single PollSema left WaitSema returning at once - the game thread
     * then never blocked and starved the lower-priority DS threads (the file loader: field loads
     * queued forever and the player could never act). */
    while (PollSema(g_vblank_sema) >= 0)
        ;
    WaitSema(g_vblank_sema);
}

uint32_t kh_vblank_count(void) { return g_vblank_count; }

int kh_video_refresh_hz(void) { return g_refresh_hz; }
