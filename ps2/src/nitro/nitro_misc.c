/* Smaller NitroSDK pieces on the PS2: OS heap, fills, cartridge/power/touch/RTC/PXI endpoints,
 * DS Protect, streaming LZ decompression, MATH sort. */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ OS */

/* MI_CpuFill32 (the ITCM copy of MIi_CpuClear32): (value, dst, size) */
void INITi_CpuClear32_0x01ff86fc(u32 value, void *dst, u32 size)
{
    u32 *d = dst;
    size /= 4;
    while (size--)
        *d++ = value;
}

/* The OS heap (OS_CreateHeap on an arena) is only used by the MobiClip player for scratch
 * memory; it is served from the EE heap with a size header. */
void *OS_AllocFromHeap(int arena, int heap, u32 size)
{
    (void)arena; (void)heap;
    return kh_alloc(size, 32, KH_LIFE_GLOBAL, KH_MEM_MISC);
}

void OS_FreeToHeap(int arena, int heap, void *p)
{
    (void)arena; (void)heap;
    kh_free(p);
}

void OSi_FreeStackAlloc(void *p) { kh_free(p); }

/* The SDK's VBlank IRQ handler (wakes threads waiting for VBlank): OS_WaitVBlankIntr does that. */
void OSi_VBlankInterruptHandler(void) { }

/* OS tick timer reload (timer 0): ticks come from the EE timer instead. */
void SetupTimer0Reload(u64 tick) { (void)tick; }

/* ------------------------------------------------------------------ GX odds */

/* the SDK's CP context restore entry point (same thing as CP_RestoreContext) */
extern void CP_RestoreContext(const void *ctx);
void CPi_RestoreContext(const void *ctx) { CP_RestoreContext(ctx); }

/* DispCnt_ApplyPendingMode: re-apply the display mode saved by GX_DispOff (else "normal") */
extern unsigned short data_020446d0;
extern short data_020422b4;
void DispCnt_ApplyPendingMode(void)
{
    volatile u32 *dispcnt = (volatile u32 *)kh_ds_io;
    u16 v;
    data_020422b4 = 1;
    v = data_020446d0;
    if (v != 0) {
        *dispcnt = (*dispcnt & 0xfffcffffu) | ((u32)v << 16);
        return;
    }
    *dispcnt |= 0x10000u;
}

/* G3X_SetHOffset: BG0 horizontal offset of the 3D layer (BG0HOFS) */
void G3X_SetHOffset(int value) { *(volatile u32 *)(kh_ds_io + 0x10) = (u32)value; }

/* ------------------------------------------------------ cartridge / power */

int CTRDGi_IsAgbCartridge(void *p) { (void)p; return 0; }
int CTRDG_IsExisting(void) { return 0; }
int CTRDG_IsOptionCartridge(void) { return 0; }
int PM_SetLCDPower(int on) { (void)on; return 1; }
int PM_GetLCDPower(void) { return 1; }
int PM_GoSleepMode(int a, int b, int c) { (void)a; (void)b; (void)c; return 0; }

/* ------------------------------------------------------------------ touch */

/* No touch screen: the panel reports "not touched".  Screens that need touch get controller
 * navigation (docs/PS2_PORT.md 3.10); the virtual cursor will feed TP_ here. */
typedef struct { u16 x, y, touch, validity; } TPData;

void TP_Init(void) { }
int TP_GetUserInfo(void *calib) { (void)calib; return 0; }
void TP_SetCalibrateParam(const void *calib) { (void)calib; }
void TP_RequestSetStabilityAsync(u32 a, u32 b) { (void)a; (void)b; }
void TP_RequestAutoSamplingStartAsync(int vcount, int freq, void *bufs, u32 n) { (void)vcount; (void)freq; (void)bufs; (void)n; }
void TP_RequestAutoSamplingStopAsync(void) { }
void TP_RequestSamplingAsync(void) { }
void TP_WaitBusy(u32 mask) { (void)mask; }
int TP_CheckError(u32 mask) { (void)mask; return 0; }
int TP_GetLatestIndexInAuto(void) { return 0; }
void TP_GetCalibratedPoint(TPData *out, const TPData *in)
{
    (void)in;
    memset(out, 0, sizeof *out);
}
int TP_GetBufferedData(void *d) { memset(d, 0, sizeof(TPData)); return 0; }

/* ----------------------------------------------------------------- RTC/PXI */

void RTC_Init(void) { }

typedef void (*PXIFifoCallback)(int tag, u32 data, int err);
static PXIFifoCallback g_pxi_cb[32];

/* The ARM9 side of the sound library registers its ARM7 reply handler here; the PS2 sound
 * driver (ps2/src/audio) delivers replies through kh_pxi_deliver. */
void PXI_SetFifoRecvCallback(int tag, PXIFifoCallback cb) { g_pxi_cb[tag & 31] = cb; }
int PXI_IsCallbackReady(int tag, int proc) { (void)tag; (void)proc; return 1; }
void kh_pxi_deliver(int tag, u32 data)
{
    if (g_pxi_cb[tag & 31])
        g_pxi_cb[tag & 31](tag, data, 0);
}

/* ------------------------------------------------------------- DS Protect */

/* ov028 (DS Protect, excluded) exposes anti-tamper predicates the game consults.  Two kinds, as
 * the call sites show: "no detection" predicates answer TRUE on a genuine cartridge, "detect"
 * predicates answer FALSE (and would call their callback on detection).  The PS2 always
 * answers "genuine". */
int func_ov028_0208b490(void *cb) { (void)cb; return 1; }   /* no-detect */
int func_ov028_0208b120(void *cb) { (void)cb; return 1; }   /* no-detect */
int func_ov028_0208b2e0(void *cb) { (void)cb; return 1; }   /* no-detect */
int func_ov028_0208b3c0(void *cb) { (void)cb; return 1; }   /* no-detect */
int func_ov028_0208b040(void *cb) { (void)cb; return 0; }   /* detect */
int func_ov028_0208b200(void *cb) { (void)cb; return 0; }   /* detect */

/* ------------------------------------------------------- streaming LZ decode */

/* MI_ReadUncompLZ8 (func_02004484): feed `len` bytes of an LZ10/LZ11 stream (header already
 * consumed by MI_InitUncompContextLZ) and write the output.  Returns the bytes still to be
 * produced (0 when finished).  The context fields are those MI_InitUncompContextLZ sets; this
 * implementation keeps a partial back-reference token in `length` (bytes collected counted in
 * destTmpCnt, lengthFlg == 0 while collecting, 3 when idle). */
typedef struct {
    u8 *destp;
    s32 destCount;
    u32 length;
    u16 destTmp;
    u8 destTmpCnt;
    u8 flags;
    u8 flagIndex;
    u8 lengthFlg;
    u8 exFormat;
    u8 _padding[1];
} MIUncompContextLZ;

static int token_size(const MIUncompContextLZ *c, u8 first)
{
    if (!c->exFormat)
        return 2;
    switch (first >> 4) {
    case 0: return 3;
    case 1: return 4;
    default: return 2;
    }
}

s32 func_02004484(MIUncompContextLZ *c, const u8 *data, u32 len)
{
    while (len && c->destCount > 0) {
        u8 b = *data++;
        len--;
        if (c->lengthFlg == 0) {                         /* collecting a reference token */
            u32 n, disp, need;
            c->length = (c->length << 8) | b;
            c->destTmpCnt++;
            need = (u32)token_size(c, (u8)(c->length >> ((c->destTmpCnt - 1) * 8)));
            if (c->destTmpCnt < need)
                continue;
            if (!c->exFormat) {
                n = ((c->length >> 12) & 0xf) + 3;
                disp = (c->length & 0xfff) + 1;
            } else if (need == 2) {
                n = ((c->length >> 12) & 0xf) + 1;
                disp = (c->length & 0xfff) + 1;
            } else if (need == 3) {
                n = ((c->length >> 12) & 0xff) + 0x11;
                disp = (c->length & 0xfff) + 1;
            } else {
                n = ((c->length >> 12) & 0xffff) + 0x111;
                disp = (c->length & 0xfff) + 1;
            }
            c->lengthFlg = 3;
            c->destTmpCnt = 0;
            c->length = 0;
            if ((s32)n > c->destCount)
                n = (u32)c->destCount;
            c->destCount -= (s32)n;
            while (n--) {
                *c->destp = *(c->destp - disp);
                c->destp++;
            }
            continue;
        }
        if (c->flagIndex == 0) {
            c->flags = b;
            c->flagIndex = 8;
            continue;
        }
        c->flagIndex--;
        if (c->flags & 0x80) {
            c->flags <<= 1;
            c->lengthFlg = 0;
            c->length = b;
            c->destTmpCnt = 1;
            if (token_size(c, b) == 1) { /* not possible; token sizes are >= 2 */ }
        } else {
            c->flags <<= 1;
            *c->destp++ = b;
            c->destCount--;
        }
    }
    return c->destCount > 0 ? c->destCount : 0;
}

/* overlays are native code on the PS2; nothing is compressed backwards in memory */
void MIi_UncompressBackward(void *bottom) { (void)bottom; }

/* ------------------------------------------------------------------ MATH */

/* MATH_QSort (Util_QuickSortWithWork): sorts num elements of `width` bytes with the SDK's
 * comparison convention; `work` is scratch the SDK version needed and is not required here. */
typedef s32 (*MATHCompareFunc)(void *a, void *b);
static MATHCompareFunc g_qs_cmp;
static int qs_adapter(const void *a, const void *b) { return (int)g_qs_cmp((void *)a, (void *)b); }

void Util_QuickSortWithWork(void *head, u32 num, u32 width, MATHCompareFunc cmp, void *work)
{
    (void)work;
    g_qs_cmp = cmp;
    qsort(head, num, width, qs_adapter);
}
