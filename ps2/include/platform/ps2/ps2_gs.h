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
 * (frame packets): a VIF1 DMA chain.  GIF data is wrapped in DMA tags whose VIFcodes say DIRECT
 * (PATH2), so that large payloads -- tile, palette and texture images -- are sent straight from
 * where they live with REF tags, never copied; VU1 work (vertex UNPACKs and MSCALs, PATH1) is
 * placed in the same chain, so everything reaches the GS in packet order.  After VU1 work the
 * next DIRECT data is preceded by FLUSH (wait for the microprogram and its XGKICK). */
typedef struct KhGsPacket {
    void    *base;   /* 64-byte aligned, cached */
    uint32_t cap;    /* qwords */
    uint32_t len;    /* qwords used */
    int32_t  tag;    /* chain mode: qword index of the open CNT tag, -1 in normal mode */
    uint32_t tag_vif[2];  /* VIFcodes of the open tag; tag_vif[1] == 0: DIRECT for its data */
    int32_t  vu_busy;     /* VU1 work queued since the last FLUSH */
} KhGsPacket;

void kh_gs_chain_begin(KhGsPacket *p);
/* chain mode: send qwc qwords from data (16-byte aligned, in main RAM) at this point */
void kh_gs_chain_ref(KhGsPacket *p, const void *data, uint32_t qwc);
void kh_gs_chain_send(KhGsPacket *p);
/* chain mode, VIF1: a tag carrying VIFcodes vif0, vif1; its data is the next qwords put in the
 * packet (CNT) or, with ref, qwc qwords read from ref (REF).  vif1 must not be 0. */
void kh_gs_chain_vif(KhGsPacket *p, uint32_t vif0, uint32_t vif1);
void kh_gs_chain_vif_ref(KhGsPacket *p, uint32_t vif0, uint32_t vif1, const void *ref, uint32_t qwc);

/* VIFcodes */
#define KH_VIF_NOP          0x00000000u
#define KH_VIF_STCYCL(cl, wl) ((0x01u << 24) | ((uint32_t)(wl) << 8) | (uint32_t)(cl))
#define KH_VIF_OFFSET(o)    ((0x02u << 24) | (uint32_t)(o))
#define KH_VIF_BASE(b)      ((0x03u << 24) | (uint32_t)(b))
#define KH_VIF_FLUSHE       (0x10u << 24)
#define KH_VIF_FLUSH        (0x11u << 24)
#define KH_VIF_MSCAL(a)     ((0x14u << 24) | (uint32_t)(a))
#define KH_VIF_MPG(n, a)    ((0x4au << 24) | (((uint32_t)(n) & 0xff) << 16) | (uint32_t)(a))
#define KH_VIF_DIRECT(q)    ((0x50u << 24) | ((uint32_t)(q) & 0xffff))
/* UNPACK V4-32 of n qwords to VU address a; tops: relative to the double-buffer TOPS */
#define KH_VIF_UNPACK_V4_32(n, a, tops) ((0x6cu << 24) | (((uint32_t)(n) & 0xff) << 16) | \
                                         ((tops) ? 0x8000u : 0) | (uint32_t)(a))

/* VU1 microprograms (ps2/src/vu/) */
void kh_vu1_init(void);
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
