/* NitroSDK OS core on the PS2: init, VBlank, interrupt callbacks, frame presentation.
 *
 * DS model kept: the game registers IRQ handlers with OS_SetIrqFunction (VBlank above all) and
 * waits for VBlank with OS_WaitVBlankIntr; the VBlank handler runs once per VBlank before the
 * waiting thread resumes.  PS2 model: our VBlank semaphore wakes the game thread, which then runs
 * the registered VBlank handler itself (thread context, never inside the EE interrupt), so game
 * code never executes at interrupt level.
 */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

#include <string.h>

#define OS_IE_V_BLANK (1u << 0)

typedef void (*OSIrqFunction)(void);

static OSIrqFunction g_irq_table[25];
static uint32_t g_irq_mask;
static uint32_t g_ds_vblank_count;
static volatile uint32_t g_irq_check;   /* OS_IRQTable check flags (DTCM on the DS) */

void kh_nitro_init(void)
{
    extern void kh_overlay_snapshot(void);
    kh_overlay_snapshot();
    {
        extern void kh_snd_render(int16_t *out, int frames);   /* ps2/src/audio: the DS sound driver */
        kh_audio_start(kh_snd_render);
    }
    KH_INFO("nitro", "NitroSDK layer ready");
}

/* ---------------------------------------------------------------- IRQ */

void OS_SetIrqFunction(uint32_t mask, OSIrqFunction fn)
{
    int i;
    for (i = 0; i < 25; i++)
        if (mask & (1u << i))
            g_irq_table[i] = fn;
}

OSIrqFunction OS_GetIrqFunction(uint32_t mask)
{
    int i;
    for (i = 0; i < 25; i++)
        if (mask & (1u << i))
            return g_irq_table[i];
    return NULL;
}

uint32_t OS_EnableIrqMask(uint32_t mask)  { uint32_t o = g_irq_mask; g_irq_mask |= mask; return o; }
uint32_t OS_DisableIrqMask(uint32_t mask) { uint32_t o = g_irq_mask; g_irq_mask &= ~mask; return o; }
uint32_t OS_SetIrqMask(uint32_t mask)     { uint32_t o = g_irq_mask; g_irq_mask = mask; return o; }
uint32_t OS_GetIrqMask(void)              { return g_irq_mask; }
uint32_t OS_ResetRequestIrqMask(uint32_t mask) { (void)mask; return 0; }
void OS_SetIrqCheckFlag(uint32_t mask)    { g_irq_check |= mask; }

/* The whole game runs on EE threads of equal standing with no interrupt-level game code, so a
 * DS critical section only has to keep other game threads out: EE interrupts are disabled. */
int OS_DisableInterrupts(void) { return DIntr(); }
int OS_EnableInterrupts(void) { int o = DIntr(); EIntr(); return o; }
int OS_RestoreInterrupts(int prev) { if (prev) EIntr(); else DIntr(); return prev; }
int OS_DisableInterrupts_IrqAndFiq(void) { return DIntr(); }
int OS_RestoreInterrupts_IrqAndFiq(int prev) { return OS_RestoreInterrupts(prev); }

/* ------------------------------------------------------------- VBlank */

static void run_vblank(void)
{
    g_ds_vblank_count++;
    /* HW_VBLANK_COUNT_BUF (0x027ffc3c): the OS's VBlank counter in shared RAM, read directly by
     * some scenes */
    *(volatile u32 *)(kh_ds_hiram + 0x1fc3c) = g_ds_vblank_count;
    /* VCOUNT reads the first VBlank line while the handler runs (the movie player's presenter
     * only flips inside lines 192-259), line 0 the rest of the time */
    *(volatile u16 *)(kh_ds_io + 6) = 192;
    if ((g_irq_mask & OS_IE_V_BLANK) && g_irq_table[0])
        g_irq_table[0]();
    *(volatile u16 *)(kh_ds_io + 6) = 0;
    g_irq_check |= OS_IE_V_BLANK;
}

/* On the DS the VBlank IRQ runs at every VBlank, also while a frame is still being computed, so
 * the game's VBlank counter is current whenever it is read: the main loop's pacing
 * (`while (VBlank_GetCount() < target) OS_WaitVBlankIntr();`) does not wait when the frame's work
 * already overran the target.  Here the handler runs at thread level, so VBlanks that passed
 * since the last delivery are delivered at the points where the game can observe them: when it
 * waits for a VBlank and when it reads the counter.  Without this every frame that crossed a
 * VBlank paid one extra whole VBlank (31 ms of work -> 50 ms frames instead of 33). */
#define MAX_CATCHUP 8
static uint32_t g_delivered;          /* kh_vblank_count() up to which VBlanks were delivered */
static int g_delivering;

static void deliver_pending_vblanks(int at_least_one)
{
    uint32_t now = kh_vblank_count(), n = now - g_delivered;
    if (g_delivering)
        return;
    if (n > MAX_CATCHUP)
        n = MAX_CATCHUP;
    if (!n && at_least_one)
        n = 1;
    g_delivering = 1;
    while (n--)
        run_vblank();
    g_delivering = 0;
    g_delivered = now;
}

/* Everything a VBlank brings: the flip, input, the game's VBlank handler, alarms. */
static void vblank_service(void)
{
    kh_prof_begin(KH_PROF_VBTASK);
    kh_video_flip();          /* show the frame submitted last iteration, now that we are in VBlank */
    kh_input_poll();
    {
        extern void kh_ds_key_registers_update(void);   /* ps2/overrides/engine/Pad_Sample.c */
        kh_ds_key_registers_update();
    }
    deliver_pending_vblanks(1);
    kh_nitro_run_alarms();
    {
        extern void kh_snd_run_alarms(void);
        kh_snd_run_alarms();      /* the ARM7 sound alarms (stream players) */
    }
    kh_prof_end(KH_PROF_VBTASK);
#if KH_PS2_DEBUG
    kh_prof_begin(KH_PROF_DEBUG);
    {
        extern void kh_nns_heap_check(const char *when);
        kh_nns_heap_check("between operations (found at a VBlank)");
    }
    if (g_ds_vblank_count % 300 == 0) {
        extern void kh_debug_log_object_states(void);
        kh_debug_log_object_states();
    }
    if (g_ds_vblank_count % 1200 == 0) {
        extern void kh_nns_heap_report(int detail);
        kh_nns_heap_report(0);
    }
    kh_prof_end(KH_PROF_DEBUG);
#endif
}

#if KH_PS2_DEBUG
/* Development: poke 1 (PINE) to make the main thread spin here - tests the hang watchdog */
volatile uint32_t kh_dbg_hang;
/* Development: poke 1 to make the main thread store to address 1 - tests the crash screen */
volatile uint32_t kh_dbg_crash;
#endif

void OS_WaitVBlankIntr(void)
{
#if KH_PS2_DEBUG
    extern volatile uint32_t kh_watchdog_progress;
    while (kh_dbg_hang)
        ;
    if (kh_dbg_crash)
        *(volatile uint32_t *)(uintptr_t)kh_dbg_crash = 0;
    kh_watchdog_progress++;
    if (kh_vblank_count() < KH_BOOT_TRACE_VBLANKS && kh_watchdog_progress % 30 == 1)
        KH_INFO("boot", "main loop: %u VBlank waits", (unsigned)kh_watchdog_progress);
#endif
    kh_prof_begin(KH_PROF_VBLANK);
    kh_vblank_wait();
    kh_prof_end(KH_PROF_VBLANK);
    vblank_service();
}

/* For loops that busy-wait on the DS while its VBlank interrupt works in the background (the movie
 * player's decode loop): if a VBlank has passed since the last one was serviced, service it now,
 * without waiting for the next.  1 if it did. */
int kh_nitro_poll_vblank(void)
{
    if (kh_vblank_count() == g_delivered)
        return 0;
    vblank_service();
    return 1;
}

void OS_WaitIrq(int clear, uint32_t mask)
{
    (void)clear;
    if (mask & OS_IE_V_BLANK)
        OS_WaitVBlankIntr();
}

void OS_WaitAnyIrq(void) { OS_WaitVBlankIntr(); }

/* The game's own VBlank counter (first word of its VBlank work, DTCM 0x027e0088 on the DS):
 * counted by the game's handler (OSi_VBlankInterruptHandler) and rewound by VBlank_SetCount when
 * a scene is left - not the SDK's count. */
uint32_t VBlank_GetCount(void)
{
    extern uint32_t data_027e0088;
    if (kh_vblank_count() != g_delivered) {
        kh_prof_begin(KH_PROF_VBTASK);
        deliver_pending_vblanks(0);
        kh_prof_end(KH_PROF_VBTASK);
    }
    return data_027e0088;
}
uint32_t OS_GetVBlankCount(void) { return g_ds_vblank_count; }

/* ------------------------------------------------------------ present */

extern void GXi_FlushCommandList(void);
extern void kh_ge_end_frame(void);
extern void kh_tex3d_frame_sent(void);
extern void kh_tex3d_new_frame(void);

void KhNitro_PresentFrame(void)
{
    GXi_FlushCommandList();   /* whatever G3D still buffers belongs to this frame */
    kh_nitro_render_frame();  /* 2D compositor + 3D triangles -> GS packet */
    kh_video_submit_frame();
    kh_prof_frame();
    kh_tex3d_frame_sent();    /* texture conversion scratch is free again */
    kh_tex3d_new_frame();
    kh_ge_end_frame();        /* SWAP_BUFFERS: the next frame's geometry starts empty */
}
