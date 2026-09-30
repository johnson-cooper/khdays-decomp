/* PS2-private GS helpers shared by the platform layer and the renderer (ps2/src/gfx). */
#ifndef KH_PS2_GS_H
#define KH_PS2_GS_H

#include <stdint.h>

/* Primitive coordinates are offset so that the drawing area sits in the middle of the GS's
 * 4096x4096 coordinate space. */
#define KH_GS_OFS 2048
#define KH_GS_FRAME_QWORDS (1024 * 1024 / 16)  /* 1 MiB per frame packet (2D is one sprite per tile), double-buffered */

#include <gs_gp.h>
#ifndef GS_SET_XYZ
#define GS_SET_XYZ(X, Y, Z) ((uint64_t)((X) & 0xffff) | ((uint64_t)((Y) & 0xffff) << 16) | ((uint64_t)(uint32_t)(Z) << 32))
#endif

/* GIF PACKED-mode data layouts (these differ from the A+D register formats):
 *   RGBAQ: R[0:7] G[32:39] | B[64:71] A[96:103]
 *   ST:    S[0:31] T[32:63] | Q[64:95]
 *   UV:    U[0:13] V[32:45]
 *   XYZ2:  X[0:15] Y[32:47] | Z[64:95] ADC[111]
 *   XYZF2: X[0:15] Y[32:47] | Z[68:91] F[100:107] ADC[111]
 * Each macro gives the (lo, hi) pair for kh_gs_packet_q(). */
#define KH_PK_RGBAQ_LO(r, g, b, a)  ((uint64_t)((r) & 0xff) | ((uint64_t)((g) & 0xff) << 32))
#define KH_PK_RGBAQ_HI(r, g, b, a)  ((uint64_t)((b) & 0xff) | ((uint64_t)((a) & 0xff) << 32))
#define KH_PK_UV_LO(u, v)           ((uint64_t)((u) & 0x3fff) | ((uint64_t)((v) & 0x3fff) << 32))
#define KH_PK_XYZ2_LO(x, y)         ((uint64_t)((x) & 0xffff) | ((uint64_t)((y) & 0xffff) << 32))
#define KH_PK_XYZ2_HI(z)            ((uint64_t)(uint32_t)(z))

/* A GIF packet.  Normal mode: plain GIF data sent with one normal-mode DMA.  Chain mode
 * (frame packets): the data is wrapped in DMA tags so that large payloads -- tile, palette and
 * texture images -- are sent straight from where they live with REF tags, never copied. */
typedef struct KhGsPacket {
    void    *base;   /* 64-byte aligned, cached */
    uint32_t cap;    /* qwords */
    uint32_t len;    /* qwords used */
    int32_t  tag;    /* chain mode: qword index of the open CNT tag, -1 in normal mode */
} KhGsPacket;

void kh_gs_chain_begin(KhGsPacket *p);
/* chain mode: send qwc qwords from data (16-byte aligned, in main RAM) at this point */
void kh_gs_chain_ref(KhGsPacket *p, const void *data, uint32_t qwc);
void kh_gs_chain_send(KhGsPacket *p);
/* an IMAGE upload whose pixels are sent by reference (chain mode) */
void kh_gs_upload_ref(KhGsPacket *p, const void *src, int bp, int bw, int psm, int x, int y, int w, int h);

static inline void kh_gs_packet_q(KhGsPacket *p, uint64_t lo, uint64_t hi)
{
    uint64_t *q = (uint64_t *)p->base + p->len * 2;
    q[0] = lo;
    q[1] = hi;
    p->len++;
}

void kh_gs_packet_init(KhGsPacket *p, uint32_t qwords);
void kh_gs_packet_send(KhGsPacket *p);
void kh_gs_packet_ad_begin(KhGsPacket *p, uint32_t nregs);
void kh_gs_upload(KhGsPacket *p, const void *src, int bp, int bw, int psm, int w, int h);
KhGsPacket *kh_gs_frame_packet(void);

uint32_t kh_gs_texpool_base(void);   /* byte address of the first free VRAM byte */
uint32_t kh_gs_texpool_used(void);   /* provided by the VRAM residency cache */

void ps2_time_set_refresh(int hz);

#endif
