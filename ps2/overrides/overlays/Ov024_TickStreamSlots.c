/* PS2 override: Ov024_TickStreamSlots, plus the VBlank the DS would have taken meanwhile.
 *
 * The original runs both screens' slot state machines (Ov024_MobiClip_TickSlot: read and decode
 * ahead up to ten frames) and reports 1 once both are done.  Its callers (the opening scene)
 * spin on it, relying on the VBlank interrupt and the frame alarm (OS alarms: blit the next
 * frame, flip at VBlank) to run in the background.  Here VBlanks and alarms are serviced at
 * thread level, so the spin has to give them their turn:
 *   - while a slot can still decode ahead, a VBlank that has passed is serviced without waiting
 *     (decoding keeps the rest of the time);
 *   - when there is nothing to decode (queue full, or draining at the end), the frame is
 *     presented and the next VBlank is waited for.
 */
typedef unsigned char u8;

#define QUEUE_DEPTH 10

struct MobiClipFrameTimer {
    void *pStream;
    u8 alarm[0x2c];
    long long nStartTick;
    u8 nState;
    u8 nFrontBuffer;
    u8 bPresented;
    u8 pad003b[0x40 - 0x3b];
    unsigned int nDecoded;
    unsigned int nConsumed;
};

extern unsigned int Ov024_MobiClip_TickSlot(int *slot);
extern int data_ov024_02093a2c[];
extern int data_ov024_0209ba48[];
extern void OS_WaitVBlankIntr(void);
extern void KhNitro_PresentFrame(void);
extern int kh_nitro_poll_vblank(void);

static int can_decode(const struct MobiClipFrameTimer *t)
{
    if (t == 0)
        return 0;
    return t->nState == 1 || t->nState == 2 || (t->nState == 3 && t->nDecoded - t->nConsumed < QUEUE_DEPTH);
}

unsigned int Ov024_TickStreamSlots(void)
{
    unsigned int lo;
    unsigned int hi;

    lo = Ov024_MobiClip_TickSlot((int *)data_ov024_02093a2c[2]);
    hi = Ov024_MobiClip_TickSlot((int *)data_ov024_02093a2c[3]);
    lo = lo & hi;
    if (lo != 0) {
        data_ov024_0209ba48[0x39] = 1;
        return lo;
    }
    if (can_decode((const struct MobiClipFrameTimer *)data_ov024_02093a2c[2]) ||
        can_decode((const struct MobiClipFrameTimer *)data_ov024_02093a2c[3])) {
        if (kh_nitro_poll_vblank())
            KhNitro_PresentFrame();
    } else {
        KhNitro_PresentFrame();
        OS_WaitVBlankIntr();
    }
    return lo;
}
