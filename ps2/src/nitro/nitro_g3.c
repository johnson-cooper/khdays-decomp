/* NitroSDK G3/G3X and the NitroSystem G3D submission paths, on the geometry front end
 * (nitro_ge.c).  On the DS all of these end in writes to GXFIFO (0x04000400) or to the geometry
 * command ports; here they end in kh_ge_fifo / kh_ge_cmd.  Transfers are synchronous, so the
 * NitroSystem "DMA busy" flag (data_027e0078) is always clear and its small command buffer
 * (data_027e0074) is flushed before anything else is sent, preserving command order. */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

#include <string.h>

typedef int32_t fx32;

typedef struct { u32 idx; u32 data[192]; } NNSG3dGeBuffer;
extern NNSG3dGeBuffer *data_027e0074;     /* NitroSystem G3D command buffer (may be NULL) */
extern volatile int data_027e0078;         /* "display list DMA in flight" */

void kh_ge_fifo(const u32 *w, u32 n);
void kh_ge_cmd(u32 cmd, const u32 *p);
void kh_ge_reset(void);
void kh_ge_get_pos_matrix(fx32 out[16]);
void kh_ge_get_vec_matrix(fx32 out[9]);
int kh_ge_param_count(u32 cmd);

static void flush_buffer(void)
{
    NNSG3dGeBuffer *b = data_027e0074;
    data_027e0078 = 0;
    if (b && b->idx) {
        u32 n = b->idx;
        b->idx = 0;
        kh_ge_fifo(b->data, n);
    }
}

void GXi_FlushCommandList(void) { flush_buffer(); }

void GX_SendFifoWords(u32 op, const u32 *args, u32 n)
{
    flush_buffer();
    kh_ge_fifo(&op, 1);
    if (n)
        kh_ge_fifo(args, n);
}

void NNS_G3dGeSendDL(const void *src, u32 size)
{
    flush_buffer();
    kh_ge_fifo((const u32 *)src, size >> 2);
}

void NNSi_G3dGeBufferCommand1(u32 command, u32 argument)
{
    u32 w[2] = { command, argument };
    flush_buffer();
    kh_ge_fifo(w, 2);
}

void NNS_G3dGeFlushBuffer(void) { flush_buffer(); }
void NNS_G3dGeWaitSendDL(void) { }
int NNS_G3dGeIsSendDLBusy(void) { return 0; }

/* MI_SendGXCommand*: display lists sent by DMA on the DS */
void MI_SendGXCommand(u32 dma, const void *src, u32 size) { (void)dma; flush_buffer(); kh_ge_fifo(src, size >> 2); }
void MI_SendGXCommandAsync(u32 dma, const void *src, u32 size, void (*cb)(void *), void *arg)
{
    (void)dma;
    flush_buffer();
    kh_ge_fifo(src, size >> 2);
    if (cb)
        cb(arg);
}
void MI_SendGXCommandAsyncFast(u32 dma, const void *src, u32 size, void (*cb)(void *), void *arg)
{
    MI_SendGXCommandAsync(dma, src, size, cb, arg);
}

/* Words written to one register: GXFIFO decodes packed commands, a command port (0x440..0x5fc)
 * is one command.  Used by MIi_CpuSend32 for sends aimed at those registers. */
void kh_ge_port_write(u32 io_offset, const u32 *w, u32 nwords)
{
    if (io_offset == 0x400) {
        flush_buffer();
        kh_ge_fifo(w, nwords);
        return;
    }
    if (io_offset >= 0x440 && io_offset < 0x600) {
        u32 cmd = (io_offset - 0x400) >> 2;
        int need = kh_ge_param_count(cmd);
        flush_buffer();
        if (need <= 1) {
            u32 i;
            for (i = 0; i < nwords || (need == 0 && i == 0); i++)
                kh_ge_cmd(cmd, need ? &w[i] : w);
        } else if ((int)nwords >= need) {
            kh_ge_cmd(cmd, w);
        }
        return;
    }
    KH_WARN("ge", "send to unhandled register 0x%03x", (unsigned)io_offset);
}

/* One word stored to a geometry register (prep rule R8).  Command ports collect their parameters
 * word by word, as on the DS (VTX_16 takes two stores); GXFIFO takes packed words. */
static struct { u32 cmd; u32 p[32]; int n; } g_port;

void kh_ge_port_write1(u32 off, u32 value)
{
    if (off == 0x400) {
        flush_buffer();
        kh_ge_fifo(&value, 1);
        return;
    }
    if (off >= 0x440 && off < 0x600) {
        u32 cmd = (off - 0x400) >> 2;
        int need = kh_ge_param_count(cmd);
        flush_buffer();
        if (need == 0) {
            kh_ge_cmd(cmd, &value);
            g_port.n = 0;
            return;
        }
        if (g_port.n && g_port.cmd != cmd)
            g_port.n = 0;              /* a different port: the partial command is dropped, as on the DS */
        g_port.cmd = cmd;
        g_port.p[g_port.n++] = value;
        if (g_port.n == need) {
            kh_ge_cmd(cmd, g_port.p);
            g_port.n = 0;
        }
        return;
    }
    if (off == 0x600) {                 /* GXSTAT: writes acknowledge errors / set IRQ condition */
        return;
    }
    *(volatile u32 *)(kh_ds_io + off) = value;
}

/* Reads of geometry result/status registers. */
u32 kh_ge_port_read(u32 off)
{
    flush_buffer();
    if (off == 0x600) {                 /* GXSTAT: idle, stack levels */
        extern int kh_ge_pos_stack_level(void);
        extern int kh_ge_proj_stack_level(void);
        return ((u32)kh_ge_pos_stack_level() << 8) | ((u32)kh_ge_proj_stack_level() << 13);
    }
    if (off >= 0x640 && off < 0x680) {  /* CLIPMTX_RESULT */
        extern void kh_ge_get_clip_matrix(fx32 out[16]);
        fx32 m[16];
        kh_ge_get_clip_matrix(m);
        return (u32)m[(off - 0x640) >> 2];
    }
    if (off >= 0x680 && off < 0x6a4) {  /* VECMTX_RESULT */
        fx32 m[9];
        kh_ge_get_vec_matrix(m);
        return (u32)m[(off - 0x680) >> 2];
    }
    if (off == 0x604)                   /* LISTRAM/VTXRAM counts */
        return 0;
    KH_UNIMPLEMENTED_ONCE("ge: read of an unmodelled geometry register");
    return 0;
}

/* G3X_GetClipMtx / G3X_GetVectorMtx: the SDK waits for the geometry engine and copies
 * CLIPMTX_RESULT / VECMTX_RESULT; here the queued commands are executed first, then the
 * engine's matrices are read directly (0: success, as when GXSTAT reports the engine idle). */
int G3X_GetClipMtx(void *dst)
{
    extern void kh_ge_get_clip_matrix(fx32 out[16]);
    flush_buffer();
    kh_ge_get_clip_matrix((fx32 *)dst);
    return 0;
}

int G3X_GetVectorMtx(void *dst)
{
    flush_buffer();
    kh_ge_get_vec_matrix((fx32 *)dst);
    return 0;
}

/* --------------------------------------------------------------- G3X */

void G3X_Init(void)
{
    kh_ge_reset();
    *(volatile u32 *)(kh_ds_io + 0x60) = 0;   /* DISP3DCNT */
}

void G3X_Reset(void) { flush_buffer(); }

void G3X_InitMtxStack(void)
{
    static const u32 mode_proj = 0, mode_posvec = 2;
    flush_buffer();
    kh_ge_cmd(0x10, &mode_proj);
    kh_ge_cmd(0x15, NULL);
    kh_ge_cmd(0x10, &mode_posvec);
    kh_ge_cmd(0x15, NULL);
}

void G3X_ResetMtxStack(void) { G3X_InitMtxStack(); }

int G3X_GetMtxStackLevelPV(int *level) { extern int kh_ge_pos_stack_level(void); *level = kh_ge_pos_stack_level(); return 0; }
int G3X_GetMtxStackLevelPJ(int *level) { extern int kh_ge_proj_stack_level(void); *level = kh_ge_proj_stack_level(); return 0; }

/* rear-plane clear colour/depth: DS registers CLEAR_COLOR (0x350) and CLEAR_DEPTH (0x354) */
void G3X_SetClearColor(u32 color, u32 alpha, u32 depth, u32 polyID, int fog)
{
    *(volatile u32 *)(kh_ds_io + 0x350) = (color & 0x7fff) | (fog ? 0x8000 : 0) | ((alpha & 31) << 16) | ((polyID & 63) << 24);
    *(volatile u16 *)(kh_ds_io + 0x354) = (u16)depth;
}

void G3_LoadMtx43(const void *m) { flush_buffer(); kh_ge_cmd(0x17, (const u32 *)m); }
void G3_LoadMtx44(const void *m) { flush_buffer(); kh_ge_cmd(0x16, (const u32 *)m); }
void G3_MultMtx43(const void *m) { flush_buffer(); kh_ge_cmd(0x19, (const u32 *)m); }
void G3_MultMtx44(const void *m) { flush_buffer(); kh_ge_cmd(0x18, (const u32 *)m); }
void G3_MultMtx33(const void *m) { flush_buffer(); kh_ge_cmd(0x1a, (const u32 *)m); }

/* ------------------------------------------------------------- NNS G3D */

extern void NNS_G3dGlbInit(void);

void NNS_G3dInit(void)
{
    G3X_Init();
    NNS_G3dGlbInit();
}

/* NNS_G3dGetCurrentMtx(MtxFx43 *m, MtxFx33 *n): the current position and vector matrices.  The
 * SDK reads them back from the geometry engine; the front end has them directly. */
void NNS_G3dGetCurrentMtx(fx32 *m, fx32 *n)
{
    flush_buffer();
    if (m) {
        fx32 p[16];
        int r;
        kh_ge_get_pos_matrix(p);
        for (r = 0; r < 4; r++) {
            m[r * 3 + 0] = p[r * 4 + 0];
            m[r * 3 + 1] = p[r * 4 + 1];
            m[r * 3 + 2] = p[r * 4 + 2];
        }
    }
    if (n)
        kh_ge_get_vec_matrix(n);
}
