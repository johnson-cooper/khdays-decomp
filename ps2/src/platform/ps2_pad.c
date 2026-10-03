/* DualShock 2 input backend (libpad + padman).
 *
 * Produces the platform-neutral KhPadState; nothing outside this file sees libpad.  Both ports
 * are opened; port 0 is the player.  The controller is put in DualShock (analog) mode when it
 * supports it, but the mode is not locked so the player can still toggle it.
 */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <string.h>
#include <kernel.h>
#include <libpad.h>

#define NPORTS 2
#define DEADZONE 32

static char g_pad_area[NPORTS][256] __attribute__((aligned(64)));
static KhPadState g_state[NPORTS];
static int g_mode_set[NPORTS];

void kh_input_init(void)
{
    int p;
    padInit(0);
    for (p = 0; p < NPORTS; p++) {
        if (padPortOpen(p, 0, g_pad_area[p]) == 0)
            KH_WARN("pad", "padPortOpen(%d) failed", p);
    }
}

static int8_t axis(unsigned char raw)
{
    int v = (int)raw - 128;
    if (v > -DEADZONE && v < DEADZONE)
        return 0;
    return (int8_t)(v < -128 ? -128 : v > 127 ? 127 : v);
}

/* Development: buttons (KH_BTN_*) held on port 0 in addition to the pad's, written by test scripts
 * through PCSX2 PINE (ps2/tools/win/pine_press.py) - input that does not need the window focus. */
volatile uint32_t kh_dbg_pad_inject;

void kh_input_poll(void)
{
    int p;
    for (p = 0; p < NPORTS; p++) {
        KhPadState *s = &g_state[p];
        struct padButtonStatus b;
        int st = padGetState(p, 0);
        uint32_t prev = s->held;

        if (st == PAD_STATE_DISCONN) {
            if (s->connected)
                KH_INFO("pad", "port %d disconnected", p);
            memset(s, 0, sizeof *s);
            g_mode_set[p] = 0;
            continue;
        }
        if (st != PAD_STATE_STABLE && st != PAD_STATE_FINDCTP1)
            continue;   /* busy (e.g. executing a mode change): keep the last state */
        if (!s->connected)
            KH_INFO("pad", "port %d connected", p);
        s->connected = 1;
        if (!g_mode_set[p] && st == PAD_STATE_STABLE) {
            g_mode_set[p] = 1;
            if (padInfoMode(p, 0, PAD_MODECURID, 0) != 0)
                padSetMainMode(p, 0, PAD_MMODE_DUALSHOCK, 0);
        }
        if (padRead(p, 0, &b) == 0)
            continue;
        s->held = (uint32_t)(0xffff ^ b.btns);   /* libpad bits are active low, same layout as KH_BTN_* */
        if (p == 0)
            s->held |= kh_dbg_pad_inject;
        s->pressed = s->held & ~prev;
        s->released = prev & ~s->held;
        if ((b.mode >> 4) == 0x7) {             /* analog data present */
            s->lx = axis(b.ljoy_h);
            s->ly = axis(b.ljoy_v);
            s->rx = axis(b.rjoy_h);
            s->ry = axis(b.rjoy_v);
        } else {
            s->lx = s->ly = s->rx = s->ry = 0;
        }
    }
}

const KhPadState *kh_input_pad(int port)
{
    return &g_state[port & 1];
}
