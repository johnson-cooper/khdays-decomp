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
    if ((g_irq_mask & OS_IE_V_BLANK) && g_irq_table[0])
        g_irq_table[0]();
    g_irq_check |= OS_IE_V_BLANK;
}

void OS_WaitVBlankIntr(void)
{
    kh_prof_frame();
    kh_vblank_wait();
    kh_video_flip();          /* show the frame submitted last iteration, now that we are in VBlank */
    kh_input_poll();
    run_vblank();
    kh_nitro_run_alarms();
}

void OS_WaitIrq(int clear, uint32_t mask)
{
    (void)clear;
    if (mask & OS_IE_V_BLANK)
        OS_WaitVBlankIntr();
}

void OS_WaitAnyIrq(void) { OS_WaitVBlankIntr(); }

uint32_t VBlank_GetCount(void) { return g_ds_vblank_count; }
uint32_t OS_GetVBlankCount(void) { return g_ds_vblank_count; }

/* ------------------------------------------------------------ present */

extern void GXi_FlushCommandList(void);
extern void kh_ge_end_frame(void);
extern void kh_tex3d_frame_sent(void);
extern void kh_tex3d_new_frame(void);

void KhNitro_PresentFrame(void)
{
    GXi_FlushCommandList();   /* whatever G3D still buffers belongs to this frame */
    kh_prof_begin(KH_PROF_RENDER_SUBMIT);
    kh_nitro_render_frame();  /* 2D compositor + 3D triangles -> GS packet */
    kh_prof_end(KH_PROF_RENDER_SUBMIT);
    kh_video_submit_frame();
    kh_tex3d_frame_sent();    /* texture conversion scratch is free again */
    kh_tex3d_new_frame();
    kh_ge_end_frame();        /* SWAP_BUFFERS: the next frame's geometry starts empty */
}
