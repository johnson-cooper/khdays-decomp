/* PS2 replacement for Pad_Sample (src/engine/Pad_Sample.c).
 *
 * The DS version reads KEYINPUT (0x04000130) and the ARM7's X/Y/hinge word (0x027fffa8).  Here
 * the same DS key word (bit 0 A, 1 B, 2 Select, 3 Start, 4 Right, 5 Left, 6 Up, 7 Down, 8 R,
 * 9 L, 10 X, 11 Y; active high after the SDK's inversion) is built from the platform's
 * DualShock 2 state, and everything after that -- the held/previous/triggered words and the
 * per-button change timestamps the game's key-repeat logic uses -- is the DS code unchanged.
 * There is no lid, so the "lid closed -> no input" case is gone.
 *
 * Mapping (kh_pad_map below, western Kingdom Hearts II layout):
 *   Cross A (attack / confirm)   Circle B (jump / cancel)   Triangle X   Square Y
 *   L1/L2 L    R1/R2 R    Start Start    Select Select
 *   D-pad and left stick: directions (the DS game moves digitally; the stick is read as a
 *   D-pad until analog movement is added deliberately).
 */

#include "nitro/types.h"
#include "platform/kh_platform.h"

typedef struct {
    u16 cont;       /* 0x00 */
    u16 prev;       /* 0x02 */
    u16 trig;       /* 0x04 */
} PadState;

extern PadState gPadHeld;
extern unsigned int gPadPressTimes[];
extern unsigned int VBlank_GetCount(void);

enum {
    DS_A = 1 << 0, DS_B = 1 << 1, DS_SELECT = 1 << 2, DS_START = 1 << 3,
    DS_RIGHT = 1 << 4, DS_LEFT = 1 << 5, DS_UP = 1 << 6, DS_DOWN = 1 << 7,
    DS_R = 1 << 8, DS_L = 1 << 9, DS_X = 1 << 10, DS_Y = 1 << 11,
};

const struct { u32 ps2; u16 ds; } kh_pad_map[] = {
    { KH_BTN_CROSS, DS_A },     { KH_BTN_CIRCLE, DS_B },
    { KH_BTN_TRIANGLE, DS_X },  { KH_BTN_SQUARE, DS_Y },
    { KH_BTN_L1, DS_L },        { KH_BTN_L2, DS_L },
    { KH_BTN_R1, DS_R },        { KH_BTN_R2, DS_R },
    { KH_BTN_START, DS_START }, { KH_BTN_SELECT, DS_SELECT },
    { KH_BTN_UP, DS_UP },       { KH_BTN_DOWN, DS_DOWN },
    { KH_BTN_LEFT, DS_LEFT },   { KH_BTN_RIGHT, DS_RIGHT },
};

#define STICK_THRESHOLD 64

static u16 ds_keys(void)
{
    const KhPadState *pad = kh_input_pad(0);
    u16 keys = 0;
    unsigned i;
    if (!pad->connected)
        return 0;
    for (i = 0; i < sizeof kh_pad_map / sizeof kh_pad_map[0]; i++)
        if (pad->held & kh_pad_map[i].ps2)
            keys |= kh_pad_map[i].ds;
    if (pad->lx <= -STICK_THRESHOLD) keys |= DS_LEFT;
    if (pad->lx >= STICK_THRESHOLD)  keys |= DS_RIGHT;
    if (pad->ly <= -STICK_THRESHOLD) keys |= DS_UP;
    if (pad->ly >= STICK_THRESHOLD)  keys |= DS_DOWN;
    /* the DS pad cannot report opposite directions at once */
    if ((keys & (DS_LEFT | DS_RIGHT)) == (DS_LEFT | DS_RIGHT)) keys &= ~(DS_LEFT | DS_RIGHT);
    if ((keys & (DS_UP | DS_DOWN)) == (DS_UP | DS_DOWN)) keys &= ~(DS_UP | DS_DOWN);
    return keys & 0x2fff;
}

int Pad_Sample(void)
{
    u16 bit = 1;
    u16 prev;
    u16 cont;
    unsigned int now;
    int i;

    gPadHeld.prev = gPadHeld.cont;
    gPadHeld.cont = ds_keys();

    /* Some scenes (the opening movie loop, ...) poll the key registers themselves; keep the DS
     * views of them current: KEYINPUT (buttons 0-9, active low) and the ARM7's X/Y word at
     * 0x027fffa8 (X bit 10, Y bit 11, debug 13 active low; hinge bit 15 = 0, lid open). */
    *(volatile u16 *)(kh_ds_io + 0x130) = (u16)(~gPadHeld.cont & 0x3ff);
    *(volatile u16 *)(kh_ds_hiram + 0x1ffa8) = (u16)(0x2c00 & ~(gPadHeld.cont & 0x0c00));
    prev = gPadHeld.prev;
    cont = (u16)gPadHeld.cont;
    gPadHeld.trig = ~prev & cont;
    now = VBlank_GetCount();
    for (i = 0; i < 12; i++) {
        if ((u16)(prev ^ cont) & bit) {
            gPadPressTimes[i] = now;
        }
        bit = bit << 1;
    }
    return 1;
}
