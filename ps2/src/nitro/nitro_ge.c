/* The DS 3D geometry engine, as a PS2 geometry front end.
 *
 * Everything the game and NitroSystem G3D send to the DS geometry engine -- packed command lists
 * written to GXFIFO (display lists, NNS_G3dGeSendDL, GX_SendFifoWords, the G3D command buffer),
 * single commands written to the command ports -- arrives in kh_ge_fifo() / kh_ge_cmd().  This is
 * a decoder, not a FIFO emulator: each command updates the geometry state (matrix modes and
 * stacks, material, lights, polygon/texture attributes) or produces vertices, which are assembled
 * into primitives, transformed to clip space with the DS fixed-point matrices, lit, clipped
 * against the view volume and written to the frame's triangle list in GS-ready form (screen x/y,
 * 24-bit Z, colour, alpha, texture coordinates and texture state).  kh_ge_render() turns that list
 * into GS primitives when the frame is presented (SWAP_BUFFERS).
 *
 * Fixed point is kept where the DS computes in fixed point (matrices, vertex transform, light
 * vectors), so geometry matches the DS; conversion to float happens only at the GS boundary.
 *
 * VU1: the triangle list layout (struct GeVtx) is what a later VU1 path consumes instead.
 */
#include "platform/kh_platform.h"
#include "platform/ps2/ps2_gs.h"
#include "nitro_internal.h"

#include <string.h>
#include <gs_gp.h>
#include <gs_psm.h>
#include <gif_tags.h>

typedef int32_t fx32;
typedef struct { fx32 m[4][4]; } __attribute__((aligned(16))) Mtx44;   /* VU0 loads rows with LQC2 */
typedef struct { fx32 m[3][3]; } Mtx33;

/* ------------------------------------------------------------------ state */

enum { MODE_PROJ = 0, MODE_POS = 1, MODE_POSVEC = 2, MODE_TEX = 3 };
enum { CLIP_FX = 1, CLIP_F = 2 };

/* A clip-space vertex, in the layout the VU1 microprogram reads (ps2/src/vu/vu1_tri.vsm):
 * x..w written by VU0 with one SQC2; s, t in texels and q1 = 1; colour 0..255, alpha 0..0x80. */
typedef struct GeVtx {
    float x, y, z, w;
    float s, t, q1, pad;
    s32 r, g, b, a;
} __attribute__((aligned(16))) GeVtx;

/* The frame's triangles, one list per pass (0 opaque, 1 translucent) in submission order, so a run
 * of triangles with the same GS state is contiguous and goes to VU1 by reference. */
#define MAX_TRIS_OPAQUE 6144
#define MAX_TRIS_TRANS  2048
typedef struct GeTriAttr { u32 polyattr, teximage, pltt; } GeTriAttr;

static struct {
    int mode;
    Mtx44 proj, pos, tex;
    Mtx33 vec;
    Mtx44 proj_stack[1];
    Mtx44 pos_stack[31];
    Mtx33 vec_stack[31];
    Mtx44 tex_stack[1];
    int proj_sp, pos_sp;
    Mtx44 clip;           /* pos * proj, DS fixed point (CLIPMTX_RESULT reads) */
    int clip_dirty;       /* CLIP_FX | CLIP_F: which form of the clip matrix is stale */
    float clipf[16] __attribute__((aligned(16)));   /* pos * proj in float: the vertex transform */

    /* vertex state */
    fx32 vx, vy, vz;      /* last vertex (for VTX_XY/XZ/YZ/DIFF), model space */
    u8 cr, cg, cb;        /* current vertex colour, 0..31 */
    s32 ts, tt;           /* texcoord, 1/16 texel */
    s32 ns, nt;           /* texcoord after texture-matrix processing */
    u32 polyattr, polyattr_pending;
    u32 teximage, pltt;

    /* material + lights */
    u16 dif, amb, spe, emi;
    int use_shine;
    u8 shine[128];
    fx32 lvec[4][3];      /* light vectors, transformed by the vector matrix */
    u16 lcol[4];

    /* primitive assembly */
    int prim;             /* 0 tri, 1 quad, 2 tri strip, 3 quad strip, -1 none */
    GeVtx strip[4];       /* the last vertices, slot = vertex number % (3 or 4) */
    int nstrip;           /* vertices since BEGIN_VTXS */

    u32 viewport;         /* x1 | y1<<8 | x2<<16 | y2<<24 */
    int swap_mode;
} g;

static GeVtx g_vtx_opaque[MAX_TRIS_OPAQUE * 3] __attribute__((aligned(64)));
static GeVtx g_vtx_trans[MAX_TRIS_TRANS * 3] __attribute__((aligned(64)));
static GeTriAttr g_attr_opaque[MAX_TRIS_OPAQUE], g_attr_trans[MAX_TRIS_TRANS];
static GeVtx *const g_vtx[2] = { g_vtx_opaque, g_vtx_trans };
static GeTriAttr *const g_attr[2] = { g_attr_opaque, g_attr_trans };
static const int g_max_tris[2] = { MAX_TRIS_OPAQUE, MAX_TRIS_TRANS };
static int g_ntri[2];
static int g_overflow_logged;
static int g_got_cmds;                  /* geometry commands arrived since the last present */
static int g_kept;                      /* the lists hold a frame kept for being redrawn */

/* DS: a polygon goes to the translucent list - drawn after the opaque ones, blended - when its
 * alpha is neither 31 (opaque) nor 0 (wireframe), or when its texture has translucent texels
 * (A3I5, A5I3).  Those formats in the opaque pass were drawn without blending: every texel with
 * alpha > 0 passed the alpha test at full strength (the white quads of light patches and glows). */
static inline int poly_translucent(u32 polyattr, u32 teximage)
{
    u32 alpha = (polyattr >> 16) & 31, fmt = (teximage >> 26) & 7;
    return (alpha != 31 && alpha != 0) || fmt == 1 || fmt == 6;
}

/* --------------------------------------------------------------- matrices */

static inline fx32 fmul(fx32 a, fx32 b) { return (fx32)(((s64)a * b) >> 12); }

static void m44_identity(Mtx44 *m)
{
    memset(m, 0, sizeof *m);
    m->m[0][0] = m->m[1][1] = m->m[2][2] = m->m[3][3] = 0x1000;
}

static void m33_identity(Mtx33 *m)
{
    memset(m, 0, sizeof *m);
    m->m[0][0] = m->m[1][1] = m->m[2][2] = 0x1000;
}

/* Fixed-point dot product with the DS geometry engine's precision: 64-bit accumulation, then
 * >> 12.  The R5900 MADD instruction accumulates 32x32 products straight into HI:LO, so four
 * terms cost MULT + 3 MADD instead of four emulated 64-bit multiply-adds. */
static inline fx32 dot4_fx(fx32 a0, fx32 b0, fx32 a1, fx32 b1, fx32 a2, fx32 b2, fx32 a3, fx32 b3)
{
    u32 lo, hi;
    __asm__("mult $0, %2, %3\n\tmadd %4, %5\n\tmadd %6, %7\n\tmadd %8, %9\n\tmflo %0\n\tmfhi %1"
            : "=r"(lo), "=r"(hi)
            : "r"(a0), "r"(b0), "r"(a1), "r"(b1), "r"(a2), "r"(b2), "r"(a3), "r"(b3)
            : "hi", "lo");
    return (fx32)((lo >> 12) | (hi << 20));
}

/* d = a * b (row vectors, as the DS: new = a x current) */
static void m44_mul(Mtx44 *d, const Mtx44 *a, const Mtx44 *b)
{
    Mtx44 r;
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            r.m[i][j] = dot4_fx(a->m[i][0], b->m[0][j], a->m[i][1], b->m[1][j],
                                a->m[i][2], b->m[2][j], a->m[i][3], b->m[3][j]);
    *d = r;
}

static void m33_mul(Mtx33 *d, const Mtx33 *a, const Mtx33 *b)
{
    Mtx33 r;
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            r.m[i][j] = dot4_fx(a->m[i][0], b->m[0][j], a->m[i][1], b->m[1][j], a->m[i][2], b->m[2][j], 0, 0);
    *d = r;
}

static void m44_to_33(Mtx33 *d, const Mtx44 *s)
{
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            d->m[i][j] = s->m[i][j];
}

static void update_clip(void)
{
    if (g.clip_dirty & CLIP_FX) {
        m44_mul(&g.clip, &g.pos, &g.proj);
        g.clip_dirty &= ~CLIP_FX;
    }
}

/* VU0 macro mode: clipf = float(pos) x float(proj).  VITOF12 turns the 20.12 fixed-point entries
 * into floats exactly; the float product only places vertices on screen (matrix state the game
 * can read back stays in DS fixed point). */
static void update_clipf(void)
{
    if (!(g.clip_dirty & CLIP_F))
        return;
    __asm__ __volatile__(
        "lqc2 $vf5, 0(%1)\n\t"  "lqc2 $vf6, 16(%1)\n\t" "lqc2 $vf7, 32(%1)\n\t" "lqc2 $vf8, 48(%1)\n\t"
        "lqc2 $vf1, 0(%0)\n\t"  "lqc2 $vf2, 16(%0)\n\t" "lqc2 $vf3, 32(%0)\n\t" "lqc2 $vf4, 48(%0)\n\t"
        "vitof12.xyzw $vf5, $vf5\n\t" "vitof12.xyzw $vf6, $vf6\n\t" "vitof12.xyzw $vf7, $vf7\n\t" "vitof12.xyzw $vf8, $vf8\n\t"
        "vitof12.xyzw $vf1, $vf1\n\t" "vitof12.xyzw $vf2, $vf2\n\t" "vitof12.xyzw $vf3, $vf3\n\t" "vitof12.xyzw $vf4, $vf4\n\t"
        "vmulax.xyzw  $ACC, $vf5, $vf1x\n\t" "vmadday.xyzw $ACC, $vf6, $vf1y\n\t"
        "vmaddaz.xyzw $ACC, $vf7, $vf1z\n\t" "vmaddw.xyzw  $vf9, $vf8, $vf1w\n\t" "sqc2 $vf9, 0(%2)\n\t"
        "vmulax.xyzw  $ACC, $vf5, $vf2x\n\t" "vmadday.xyzw $ACC, $vf6, $vf2y\n\t"
        "vmaddaz.xyzw $ACC, $vf7, $vf2z\n\t" "vmaddw.xyzw  $vf9, $vf8, $vf2w\n\t" "sqc2 $vf9, 16(%2)\n\t"
        "vmulax.xyzw  $ACC, $vf5, $vf3x\n\t" "vmadday.xyzw $ACC, $vf6, $vf3y\n\t"
        "vmaddaz.xyzw $ACC, $vf7, $vf3z\n\t" "vmaddw.xyzw  $vf9, $vf8, $vf3w\n\t" "sqc2 $vf9, 32(%2)\n\t"
        "vmulax.xyzw  $ACC, $vf5, $vf4x\n\t" "vmadday.xyzw $ACC, $vf6, $vf4y\n\t"
        "vmaddaz.xyzw $ACC, $vf7, $vf4z\n\t" "vmaddw.xyzw  $vf9, $vf8, $vf4w\n\t" "sqc2 $vf9, 48(%2)\n\t"
        : : "r"(&g.pos), "r"(&g.proj), "r"(g.clipf) : "memory");
    g.clip_dirty &= ~CLIP_F;
}

/* VU0: clip-space position of a model-space vertex (20.12 fixed point) */
static inline void xform_vertex(GeVtx *v, fx32 x, fx32 y, fx32 z)
{
    s32 in[4] __attribute__((aligned(16))) = { x, y, z, 0x1000 };
    __asm__ __volatile__(
        "lqc2 $vf1, 0(%1)\n\t" "lqc2 $vf2, 16(%1)\n\t" "lqc2 $vf3, 32(%1)\n\t" "lqc2 $vf4, 48(%1)\n\t"
        "lqc2 $vf5, 0(%2)\n\t" "vitof12.xyzw $vf5, $vf5\n\t"
        "vmulax.xyzw  $ACC, $vf1, $vf5x\n\t" "vmadday.xyzw $ACC, $vf2, $vf5y\n\t"
        "vmaddaz.xyzw $ACC, $vf3, $vf5z\n\t" "vmaddw.xyzw  $vf6, $vf4, $vf5w\n\t"
        "sqc2 $vf6, 0(%0)\n\t"
        : : "r"(&v->x), "r"(g.clipf), "r"(in) : "memory");
}

/* apply "load" or "multiply" to the matrices selected by the mode */
static void apply_matrix(const Mtx44 *m, int multiply, int is_scale, int is_trans)
{
    switch (g.mode) {
    case MODE_PROJ:
        if (multiply) m44_mul(&g.proj, m, &g.proj); else g.proj = *m;
        break;
    case MODE_POS:
        if (multiply) m44_mul(&g.pos, m, &g.pos); else g.pos = *m;
        break;
    case MODE_POSVEC:
        if (multiply) m44_mul(&g.pos, m, &g.pos); else g.pos = *m;
        /* the vector matrix does not take scale; translation has no effect on 3x3 */
        if (!is_scale && !is_trans) {
            Mtx33 v;
            m44_to_33(&v, m);
            if (multiply) m33_mul(&g.vec, &v, &g.vec); else g.vec = v;
        }
        break;
    case MODE_TEX:
        if (multiply) m44_mul(&g.tex, m, &g.tex); else g.tex = *m;
        return;                     /* the clip matrix does not involve the texture matrix */
    }
    g.clip_dirty = CLIP_FX | CLIP_F;
}

/* ----------------------------------------------------------------- output */

static void emit_tri(const GeVtx *a, const GeVtx *b, const GeVtx *c)
{
    int pass = poly_translucent(g.polyattr, g.teximage);
    GeVtx *v;
    GeTriAttr *t;
    if (g_ntri[pass] >= g_max_tris[pass]) {
        if (!g_overflow_logged) {
            g_overflow_logged = 1;
            KH_WARN("ge", "triangle list %d full (%d)", pass, g_max_tris[pass]);
        }
        return;
    }
    v = &g_vtx[pass][g_ntri[pass] * 3];
    t = &g_attr[pass][g_ntri[pass]++];
    v[0] = *a;
    v[1] = *b;
    v[2] = *c;
    t->polyattr = g.polyattr;
    t->teximage = g.teximage;
    t->pltt = g.pltt;
}

/* clip a convex polygon against one clip-space plane: dot = k0*x + k1*y + k2*z + w >= 0 */
static int clip_plane(GeVtx *in, int n, GeVtx *out, int axis, float sign)
{
    int i, m = 0;
    for (i = 0; i < n; i++) {
        const GeVtx *a = &in[i], *b = &in[(i + 1) % n];
        float da = (axis == 0 ? a->x : axis == 1 ? a->y : a->z) * sign + a->w;
        float db = (axis == 0 ? b->x : axis == 1 ? b->y : b->z) * sign + b->w;
        if (da >= 0)
            out[m++] = *a;
        if ((da >= 0) != (db >= 0)) {
            float t = da / (da - db);
            GeVtx *o = &out[m++];
            o->x = a->x + (b->x - a->x) * t;
            o->y = a->y + (b->y - a->y) * t;
            o->z = a->z + (b->z - a->z) * t;
            o->w = a->w + (b->w - a->w) * t;
            o->s = a->s + (b->s - a->s) * t;
            o->t = a->t + (b->t - a->t) * t;
            o->q1 = 1.0f;
            o->r = (s32)((float)a->r + (float)(b->r - a->r) * t);
            o->g = (s32)((float)a->g + (float)(b->g - a->g) * t);
            o->b = (s32)((float)a->b + (float)(b->b - a->b) * t);
            o->a = (s32)((float)a->a + (float)(b->a - a->a) * t);
        }
    }
    return m;
}

/* Outcodes.  x and y are tested against the view volume (for trivial rejection) and against a
 * guard band GB times wider: the GS draws in a 4096x4096 space with the screen around its
 * centre and the scissor cuts what lies outside, so a polygon only has to be clipped in x/y when
 * it leaves the guard band.  Near and far (z) are always clipped, as on the DS. */
#define GB 4.0f
enum {
    OC_XP = 1, OC_XN = 2, OC_YP = 4, OC_YN = 8, OC_ZP = 16, OC_ZN = 32,   /* view volume */
    OC_GXP = 64, OC_GXN = 128, OC_GYP = 256, OC_GYN = 512                /* guard band */
};

static inline int outcode(const GeVtx *v)
{
    float w = v->w, gw = GB * w;
    int c = 0;
    if (v->x > w) c |= OC_XP;
    if (v->x < -w) c |= OC_XN;
    if (v->y > w) c |= OC_YP;
    if (v->y < -w) c |= OC_YN;
    if (v->z > w) c |= OC_ZP;
    if (v->z < -w) c |= OC_ZN;
    if (v->x > gw) c |= OC_GXP;
    if (v->x < -gw) c |= OC_GXN;
    if (v->y > gw) c |= OC_GYP;
    if (v->y < -gw) c |= OC_GYN;
    return c;
}

static void emit_poly(const GeVtx *const *vp, int n)
{
    GeVtx buf0[12], buf1[12];
    int i, axis, oc_and = ~0, oc_or = 0;
    u32 pa = g.polyattr;
    const GeVtx *v0 = vp[0], *v1 = vp[1], *v2 = vp[2];

    KH_PROF_ADD(KH_PC_POLYS, 1);
    /* face culling on the unclipped polygon: with every w > 0 the sign of the 3x3 determinant of
     * the (x, y, w) rows is the sign of the projected (x/w, y/w) area, no divides needed */
    if (v0->w > 0 && v1->w > 0 && v2->w > 0) {
        float det = v0->x * (v1->y * v2->w - v2->y * v1->w) -
                    v1->x * (v0->y * v2->w - v2->y * v0->w) +
                    v2->x * (v0->y * v1->w - v1->y * v0->w);
        if ((det > 0 && !(pa & 0x80)) || (det < 0 && !(pa & 0x40))) {
            KH_PROF_ADD(KH_PC_CULLED, 1);              /* face not rendered */
            return;
        }
    }
    for (i = 0; i < n; i++) {
        int c = outcode(vp[i]);
        oc_and &= c;
        oc_or |= c;
    }
    if (oc_and & (OC_XP | OC_XN | OC_YP | OC_YN | OC_ZP | OC_ZN)) {
        KH_PROF_ADD(KH_PC_OFFSCREEN, 1);               /* entirely outside one plane */
        return;
    }
    /* fast path: inside near/far and the guard band -> no clipping */
    if (!(oc_or & (OC_ZP | OC_ZN | OC_GXP | OC_GXN | OC_GYP | OC_GYN))) {
        for (i = 1; i + 1 < n; i++)
            emit_tri(vp[0], vp[i], vp[i + 1]);
        return;
    }
    KH_PROF_ADD(KH_PC_CLIPPED, 1);
    for (i = 0; i < n; i++)
        buf0[i] = *vp[i];
    for (axis = 0; axis < 3; axis++) {
        /* planes: z against the view volume, x/y against the guard band (sign scales the plane) */
        float k = axis == 2 ? 1.0f : 1.0f / GB;
        int pos = axis == 0 ? OC_GXP : axis == 1 ? OC_GYP : OC_ZP;
        int neg = axis == 0 ? OC_GXN : axis == 1 ? OC_GYN : OC_ZN;
        if (oc_or & pos) {
            n = clip_plane(buf0, n, buf1, axis, -k);
            if (n < 3) { KH_PROF_ADD(KH_PC_OFFSCREEN, 1); return; }
            memcpy(buf0, buf1, sizeof(GeVtx) * (size_t)n);
        }
        if (oc_or & neg) {
            n = clip_plane(buf0, n, buf1, axis, k);
            if (n < 3) { KH_PROF_ADD(KH_PC_OFFSCREEN, 1); return; }
            memcpy(buf0, buf1, sizeof(GeVtx) * (size_t)n);
        }
    }
    for (i = 1; i + 1 < n; i++)
        emit_tri(&buf0[0], &buf0[i], &buf0[i + 1]);
}

/* ----------------------------------------------------------------- vertices */

static u8 c5(u16 c, int sh) { return (u8)((c >> sh) & 31); }

static void light_normal(u32 param)
{
    fx32 n[3], tn[3];
    int i, r, gg, b;
    /* 10-bit signed 1.0.9 fixed components -> fx32 */
    n[0] = ((s32)(param << 22) >> 22) << 3;
    n[1] = ((s32)(param << 12) >> 22) << 3;
    n[2] = ((s32)(param << 2) >> 22) << 3;
    for (i = 0; i < 3; i++)
        tn[i] = fmul(n[0], g.vec.m[0][i]) + fmul(n[1], g.vec.m[1][i]) + fmul(n[2], g.vec.m[2][i]);
    r = c5(g.emi, 0); gg = c5(g.emi, 5); b = c5(g.emi, 10);
    for (i = 0; i < 4; i++) {
        fx32 dl, sl;
        if (!(g.polyattr & (1u << i)))
            continue;
        dl = -(fmul(g.lvec[i][0], tn[0]) + fmul(g.lvec[i][1], tn[1]) + fmul(g.lvec[i][2], tn[2]));
        if (dl < 0) dl = 0;
        /* specular: half vector between light and the (0,0,-1) view direction */
        sl = -(fmul((g.lvec[i][0]) >> 1, tn[0]) + fmul((g.lvec[i][1]) >> 1, tn[1]) +
               fmul((g.lvec[i][2] - 0x1000) >> 1, tn[2]));
        if (sl < 0) sl = 0;
        sl = fmul(sl, sl);
        if (sl > 0x1000) sl = 0x1000;
        if (g.use_shine)
            sl = g.shine[sl >> 5] << 5;
        r  += (c5(g.lcol[i], 0)  * ((c5(g.dif, 0)  * dl + c5(g.spe, 0)  * sl) >> 12) + c5(g.lcol[i], 0)  * c5(g.amb, 0))  >> 5;
        gg += (c5(g.lcol[i], 5)  * ((c5(g.dif, 5)  * dl + c5(g.spe, 5)  * sl) >> 12) + c5(g.lcol[i], 5)  * c5(g.amb, 5))  >> 5;
        b  += (c5(g.lcol[i], 10) * ((c5(g.dif, 10) * dl + c5(g.spe, 10) * sl) >> 12) + c5(g.lcol[i], 10) * c5(g.amb, 10)) >> 5;
    }
    g.cr = (u8)(r > 31 ? 31 : r);
    g.cg = (u8)(gg > 31 ? 31 : gg);
    g.cb = (u8)(b > 31 ? 31 : b);
}

static void texcoord_transform(int src_normal_or_vertex, fx32 x, fx32 y, fx32 z)
{
    int mode = (int)(g.teximage >> 30);
    if (mode == 0) {
        g.ns = g.ts;
        g.nt = g.tt;
    } else if (mode == 1) {
        /* (s, t, 1/16, 1/16) x texture matrix */
        g.ns = (s32)(((s64)g.ts * g.tex.m[0][0] + (s64)g.tt * g.tex.m[1][0] + (s64)g.tex.m[2][0] + (s64)g.tex.m[3][0]) >> 12);
        g.nt = (s32)(((s64)g.ts * g.tex.m[0][1] + (s64)g.tt * g.tex.m[1][1] + (s64)g.tex.m[2][1] + (s64)g.tex.m[3][1]) >> 12);
    } else if (mode == src_normal_or_vertex) {
        g.ns = (s32)((((s64)x * g.tex.m[0][0] + (s64)y * g.tex.m[1][0] + (s64)z * g.tex.m[2][0]) >> 24) + g.ts);
        g.nt = (s32)((((s64)x * g.tex.m[0][1] + (s64)y * g.tex.m[1][1] + (s64)z * g.tex.m[2][1]) >> 24) + g.tt);
    }
}

/* DS polygon alpha 0..31 -> GS alpha (0x80 = 1.0) */
static const u8 k_alpha_gs[32] = {
#define A(n) (u8)(((n) * 0x80 + 15) / 31)
    A(0), A(1), A(2), A(3), A(4), A(5), A(6), A(7), A(8), A(9), A(10), A(11), A(12), A(13), A(14), A(15),
    A(16), A(17), A(18), A(19), A(20), A(21), A(22), A(23), A(24), A(25), A(26), A(27), A(28), A(29), A(30), A(31)
#undef A
};

static void submit_vertex(fx32 x, fx32 y, fx32 z)
{
    int alpha = (int)((g.polyattr >> 16) & 31);
    int vc = g.nstrip, prim = g.prim;
    GeVtx *v;
    g.vx = x; g.vy = y; g.vz = z;
    if (prim < 0)
        return;
    KH_PROF_ADD(KH_PC_VERTS, 1);
    if ((g.teximage >> 30) == 3)
        texcoord_transform(3, x, y, z);
    v = &g.strip[(prim & 1) ? (vc & 3) : (vc % 3)];
    update_clipf();
    xform_vertex(v, x, y, z);
    v->s = (float)g.ns * (1.0f / 16.0f);
    v->t = (float)g.nt * (1.0f / 16.0f);
    v->q1 = 1.0f;
    v->r = g.cr << 3 | g.cr >> 2;
    v->g = g.cg << 3 | g.cg >> 2;
    v->b = g.cb << 3 | g.cb >> 2;
    v->a = alpha == 0 ? 0x80 : k_alpha_gs[alpha];   /* alpha 0 = wireframe: drawn solid for now */
    g.nstrip = vc + 1;

    switch (prim) {
    case 0:     /* separate triangles */
        if (vc % 3 == 2) {
            const GeVtx *t[3] = { &g.strip[0], &g.strip[1], &g.strip[2] };
            emit_poly(t, 3);
        }
        break;
    case 1:     /* separate quads */
        if ((vc & 3) == 3) {
            const GeVtx *q[4] = { &g.strip[0], &g.strip[1], &g.strip[2], &g.strip[3] };
            emit_poly(q, 4);
        }
        break;
    case 2:     /* triangle strip: every other triangle has its first two vertices swapped */
        if (vc >= 2) {
            const GeVtx *t[3] = { &g.strip[(vc - 2) % 3], &g.strip[(vc - 1) % 3], &g.strip[vc % 3] };
            if (vc & 1) {
                const GeVtx *s0 = t[0];
                t[0] = t[1];
                t[1] = s0;
            }
            emit_poly(t, 3);
        }
        break;
    case 3:     /* quad strip: v0 v1 v3 v2, then each further pair */
        if (vc >= 3 && (vc & 1)) {
            const GeVtx *q[4] = { &g.strip[(vc - 3) & 3], &g.strip[(vc - 2) & 3], &g.strip[vc & 3], &g.strip[(vc - 1) & 3] };
            emit_poly(q, 4);
        }
        break;
    }
}

/* ----------------------------------------------------------------- commands */

static const u8 k_nparams[256] = {
    [0x10] = 1, [0x11] = 0, [0x12] = 1, [0x13] = 1, [0x14] = 1, [0x15] = 0, [0x16] = 16, [0x17] = 12,
    [0x18] = 16, [0x19] = 12, [0x1a] = 9, [0x1b] = 3, [0x1c] = 3,
    [0x20] = 1, [0x21] = 1, [0x22] = 1, [0x23] = 2, [0x24] = 1, [0x25] = 1, [0x26] = 1, [0x27] = 1,
    [0x28] = 1, [0x29] = 1, [0x2a] = 1, [0x2b] = 1,
    [0x30] = 1, [0x31] = 1, [0x32] = 1, [0x33] = 1, [0x34] = 32,
    [0x40] = 1, [0x41] = 0, [0x50] = 1, [0x60] = 1, [0x70] = 3, [0x71] = 2, [0x72] = 1,
};

int kh_ge_param_count(u32 cmd) { return k_nparams[cmd & 0xff]; }

static void load_m44(Mtx44 *m, const u32 *p)
{
    int i;
    for (i = 0; i < 16; i++)
        m->m[i / 4][i % 4] = (fx32)p[i];
}

static void load_m43(Mtx44 *m, const u32 *p)
{
    int i;
    for (i = 0; i < 12; i++)
        m->m[i / 3][i % 3] = (fx32)p[i];
    m->m[0][3] = m->m[1][3] = m->m[2][3] = 0;
    m->m[3][3] = 0x1000;
}

static s32 sext(u32 v, int bits) { return (s32)(v << (32 - bits)) >> (32 - bits); }

static void ge_cmd(u32 cmd, const u32 *p)
{
    if (g_kept) {                       /* a new frame starts: drop the one kept for redrawing */
        g_kept = 0;
        g_ntri[0] = g_ntri[1] = 0;
    }
    g_got_cmds = 1;
    Mtx44 m;
    switch (cmd) {
    case 0x10: g.mode = (int)(p[0] & 3); break;
    case 0x11:  /* MTX_PUSH */
        if (g.mode == MODE_PROJ) { g.proj_stack[0] = g.proj; g.proj_sp = 1; }
        else if (g.mode == MODE_TEX) { g.tex_stack[0] = g.tex; }
        else if (g.pos_sp < 31) { g.pos_stack[g.pos_sp] = g.pos; g.vec_stack[g.pos_sp] = g.vec; g.pos_sp++; }
        break;
    case 0x12:  /* MTX_POP n */
        if (g.mode == MODE_PROJ) { if (g.proj_sp) { g.proj = g.proj_stack[0]; g.proj_sp = 0; } }
        else if (g.mode == MODE_TEX) { g.tex = g.tex_stack[0]; }
        else {
            int n = sext(p[0], 6);
            g.pos_sp -= n;
            if (g.pos_sp < 0) g.pos_sp = 0;
            if (g.pos_sp > 30) g.pos_sp = 30;
            g.pos = g.pos_stack[g.pos_sp];
            g.vec = g.vec_stack[g.pos_sp];
        }
        g.clip_dirty = CLIP_FX | CLIP_F;
        break;
    case 0x13:  /* MTX_STORE i */
        if (g.mode == MODE_PROJ) g.proj_stack[0] = g.proj;
        else if (g.mode == MODE_TEX) g.tex_stack[0] = g.tex;
        else { int i = (int)(p[0] & 31); if (i < 31) { g.pos_stack[i] = g.pos; g.vec_stack[i] = g.vec; } }
        break;
    case 0x14:  /* MTX_RESTORE i */
        if (g.mode == MODE_PROJ) g.proj = g.proj_stack[0];
        else if (g.mode == MODE_TEX) g.tex = g.tex_stack[0];
        else { int i = (int)(p[0] & 31); if (i < 31) { g.pos = g.pos_stack[i]; g.vec = g.vec_stack[i]; } }
        g.clip_dirty = CLIP_FX | CLIP_F;
        break;
    case 0x15:
        m44_identity(&m);
        if (g.mode == MODE_PROJ) g.proj = m;
        else if (g.mode == MODE_POS) g.pos = m;
        else if (g.mode == MODE_POSVEC) { g.pos = m; m33_identity(&g.vec); }
        else g.tex = m;
        g.clip_dirty = CLIP_FX | CLIP_F;
        break;
    case 0x16: load_m44(&m, p); apply_matrix(&m, 0, 0, 0); break;
    case 0x17: load_m43(&m, p); apply_matrix(&m, 0, 0, 0); break;
    case 0x18: load_m44(&m, p); apply_matrix(&m, 1, 0, 0); break;
    case 0x19: load_m43(&m, p); apply_matrix(&m, 1, 0, 0); break;
    case 0x1a:
        m44_identity(&m);
        { int i; for (i = 0; i < 9; i++) m.m[i / 3][i % 3] = (fx32)p[i]; }
        apply_matrix(&m, 1, 0, 0);
        break;
    case 0x1b:
        m44_identity(&m);
        m.m[0][0] = (fx32)p[0]; m.m[1][1] = (fx32)p[1]; m.m[2][2] = (fx32)p[2];
        apply_matrix(&m, 1, 1, 0);
        break;
    case 0x1c:
        m44_identity(&m);
        m.m[3][0] = (fx32)p[0]; m.m[3][1] = (fx32)p[1]; m.m[3][2] = (fx32)p[2];
        apply_matrix(&m, 1, 0, 1);
        break;
    case 0x20:  /* COLOR */
        g.cr = c5((u16)p[0], 0); g.cg = c5((u16)p[0], 5); g.cb = c5((u16)p[0], 10);
        break;
    case 0x21:  /* NORMAL */
        if ((g.teximage >> 30) == 2) {
            s32 nx = sext(p[0], 10) << 3, ny = sext(p[0] >> 10, 10) << 3, nz = sext(p[0] >> 20, 10) << 3;
            texcoord_transform(2, nx, ny, nz);
        }
        light_normal(p[0]);
        break;
    case 0x22:  /* TEXCOORD */
        g.ts = (s16)(p[0] & 0xffff);
        g.tt = (s16)(p[0] >> 16);
        if ((g.teximage >> 30) <= 1)
            texcoord_transform(1, 0, 0, 0);
        break;
    case 0x23: submit_vertex((s16)(p[0] & 0xffff), (s16)(p[0] >> 16), (s16)(p[1] & 0xffff)); break;
    case 0x24: submit_vertex(sext(p[0], 10) << 6, sext(p[0] >> 10, 10) << 6, sext(p[0] >> 20, 10) << 6); break;
    case 0x25: submit_vertex((s16)(p[0] & 0xffff), (s16)(p[0] >> 16), g.vz); break;
    case 0x26: submit_vertex((s16)(p[0] & 0xffff), g.vy, (s16)(p[0] >> 16)); break;
    case 0x27: submit_vertex(g.vx, (s16)(p[0] & 0xffff), (s16)(p[0] >> 16)); break;
    case 0x28: submit_vertex(g.vx + sext(p[0], 10), g.vy + sext(p[0] >> 10, 10), g.vz + sext(p[0] >> 20, 10)); break;
    case 0x29: g.polyattr_pending = p[0]; break;
    case 0x2a: g.teximage = p[0]; break;
    case 0x2b: g.pltt = p[0] & 0x1fff; break;
    case 0x30:  /* DIF_AMB */
        g.dif = (u16)(p[0] & 0x7fff);
        g.amb = (u16)((p[0] >> 16) & 0x7fff);
        if (p[0] & 0x8000) { g.cr = c5(g.dif, 0); g.cg = c5(g.dif, 5); g.cb = c5(g.dif, 10); }
        break;
    case 0x31:  /* SPE_EMI */
        g.spe = (u16)(p[0] & 0x7fff);
        g.emi = (u16)((p[0] >> 16) & 0x7fff);
        g.use_shine = (p[0] >> 15) & 1;
        break;
    case 0x32: {  /* LIGHT_VECTOR: transformed by the vector matrix now */
        int l = (int)(p[0] >> 30), i;
        fx32 v[3] = { sext(p[0], 10) << 3, sext(p[0] >> 10, 10) << 3, sext(p[0] >> 20, 10) << 3 };
        for (i = 0; i < 3; i++)
            g.lvec[l][i] = fmul(v[0], g.vec.m[0][i]) + fmul(v[1], g.vec.m[1][i]) + fmul(v[2], g.vec.m[2][i]);
        break;
    }
    case 0x33: g.lcol[p[0] >> 30] = (u16)(p[0] & 0x7fff); break;
    case 0x34: { int i; for (i = 0; i < 32; i++) { g.shine[i * 4] = (u8)p[i]; g.shine[i * 4 + 1] = (u8)(p[i] >> 8);
                 g.shine[i * 4 + 2] = (u8)(p[i] >> 16); g.shine[i * 4 + 3] = (u8)(p[i] >> 24); } break; }
    case 0x40:  /* BEGIN_VTXS */
        g.prim = (int)(p[0] & 3);
        g.nstrip = 0;
        g.polyattr = g.polyattr_pending;
#if KH_PS2_DEBUG
        {   /* feature census: which polygon modes / DISP3DCNT settings the game uses */
            static u32 seen_modes, last_disp3d = 0xffffffffu;
            u32 mode = (g.polyattr >> 4) & 3, d3 = *(volatile u16 *)(kh_ds_io + 0x60);
            {
                static u32 seen_flags;
                u32 fl = g.polyattr & 0x0000f800u;      /* bits 11-15 */
                if (fl & ~seen_flags) {
                    seen_flags |= fl;
                    KH_INFO("ge", "POLYGON_ATTR flags first used: %08x (11 transl-depth-write, 12 far-clip, 13 1dot, 14 depth-EQUAL, 15 fog)",
                            (unsigned)g.polyattr);
                }
            }
            if (!(seen_modes & (1u << mode))) {
                static const char *const k_mode[4] = { "modulate", "decal", "toon/highlight", "shadow" };
                seen_modes |= 1u << mode;
                KH_INFO("ge", "polygon mode %s first used (POLYGON_ATTR %08x)", k_mode[mode], (unsigned)g.polyattr);
            }
            if (d3 != last_disp3d) {
                last_disp3d = d3;
                KH_INFO("ge", "DISP3DCNT %04x: tex %d shading %s atest %d blend %d aa %d edge %d fogalpha %d fog %d",
                        (unsigned)d3, d3 & 1, (d3 & 2) ? "highlight" : "toon", (d3 >> 2) & 1, (d3 >> 3) & 1,
                        (d3 >> 4) & 1, (d3 >> 5) & 1, (d3 >> 6) & 1, (d3 >> 7) & 1);
            }
        }
#endif
        break;
    case 0x41: g.prim = -1; g.nstrip = 0; break;      /* END_VTXS */
    case 0x50: g.swap_mode = (int)(p[0] & 3); break;  /* SWAP_BUFFERS (frame end handled at present) */
    case 0x60: g.viewport = p[0]; break;
    case 0x70: case 0x71: case 0x72:                   /* BOX/POS/VEC_TEST: results not modelled yet */
        KH_UNIMPLEMENTED_ONCE("ge: BOX/POS/VEC_TEST");
        break;
    default:
        break;
    }
}

/* ------------------------------------------------------------ packed FIFO */

static struct {
    u8 cmds[4];
    int ncmds, cur;
    u32 params[32];
    int nparams, need;
} f;

static void fifo_next_cmd(void)
{
    while (f.cur < f.ncmds) {
        u32 c = f.cmds[f.cur];
        f.need = k_nparams[c];
        f.nparams = 0;
        if (f.need)
            return;
        if (c)
            ge_cmd(c, f.params);        /* parameterless command */
        f.cur++;
    }
    f.ncmds = 0;
}

void kh_ge_cmd(u32 cmd, const u32 *p)
{
    kh_prof_begin(KH_PROF_GE);
    ge_cmd(cmd, p);
    kh_prof_end(KH_PROF_GE);
}

/* Packed command words: one word with up to four command bytes, then their parameters in order.
 * Commands whose parameters are all in the buffer run straight from it; a packet cut by the end of
 * the buffer (the G3D command buffer flushes word groups, ports send single words) continues in the
 * word-by-word state machine `f` with the next call. */
static void fifo_slow(u32 v)
{
    if (f.ncmds == 0) {
        f.cmds[0] = (u8)v; f.cmds[1] = (u8)(v >> 8); f.cmds[2] = (u8)(v >> 16); f.cmds[3] = (u8)(v >> 24);
        f.ncmds = 4;
        /* trailing NOP bytes end the packet */
        while (f.ncmds > 1 && f.cmds[f.ncmds - 1] == 0)
            f.ncmds--;
        f.cur = 0;
        fifo_next_cmd();
        return;
    }
    f.params[f.nparams++] = v;
    if (f.nparams == f.need) {
        ge_cmd(f.cmds[f.cur], f.params);
        f.cur++;
        fifo_next_cmd();
    }
}

void kh_ge_fifo(const u32 *w, u32 n)
{
    kh_prof_begin(KH_PROF_GE);
    while (n) {
        u32 cw;
        int k, last;
        if (f.ncmds) {                      /* finish a packet begun by an earlier call */
            fifo_slow(*w++);
            n--;
            continue;
        }
        cw = *w++;
        n--;
        for (last = 3; last > 0 && !((cw >> (last * 8)) & 0xff); last--)
            ;
        for (k = 0; k <= last; k++) {
            u32 c = (cw >> (k * 8)) & 0xff;
            u32 need = k_nparams[c];
            if (need > n) {
                /* parameters continue in a later call: hand the rest of the packet to the slow
                 * decoder, starting with this command */
                int j;
                f.ncmds = last + 1;
                for (j = 0; j < 4; j++)
                    f.cmds[j] = (u8)(cw >> (j * 8));
                f.cur = k;
                f.need = (int)need;
                f.nparams = 0;
                while (n) {
                    fifo_slow(*w++);
                    n--;
                }
                break;
            }
            if (c)
                ge_cmd(c, w);
            w += need;
            n -= need;
        }
    }
    kh_prof_end(KH_PROF_GE);
}

/* ------------------------------------------------------------- queries */

void kh_ge_get_clip_matrix(fx32 out[16])
{
    int i;
    update_clip();
    for (i = 0; i < 16; i++)
        out[i] = g.clip.m[i / 4][i % 4];
}

void kh_ge_get_vec_matrix(fx32 out[9])
{
    int i;
    for (i = 0; i < 9; i++)
        out[i] = g.vec.m[i / 3][i % 3];
}

void kh_ge_get_pos_matrix(fx32 out[16])
{
    int i;
    for (i = 0; i < 16; i++)
        out[i] = g.pos.m[i / 4][i % 4];
}

int kh_ge_pos_stack_level(void) { return g.pos_sp; }
int kh_ge_proj_stack_level(void) { return g.proj_sp; }

void kh_ge_reset(void)
{
    memset(&g, 0, sizeof g);
    m44_identity(&g.proj);
    m44_identity(&g.pos);
    m44_identity(&g.tex);
    m33_identity(&g.vec);
    g.clip_dirty = CLIP_FX | CLIP_F;
    g.prim = -1;
    g.viewport = 0xbfff0000u;
    g.polyattr = g.polyattr_pending = 0x001f00c0u;     /* opaque, both faces */
    f.ncmds = 0;
}

/* ----------------------------------------------------------- GS output */

/* Draw this frame's triangles into the rectangle that shows the 3D layer.
 * Pass 0: opaque polygons (alpha 31).  Pass 1: translucent polygons, depth written only when the
 * polygon asks for it (POLYGON_ATTR bit 11), as the DS draws translucent polygons after opaque ones.
 *
 * Each run of consecutive triangles that share their GS state (texture, PRIM, ZBUF) is drawn by
 * the VU1 microprogram ps2/src/vu/vu1_tri.vsm: the run's vertices go to VU1 memory by reference
 * (VIF1 UNPACK straight from the triangle list, no copy) behind a 6-qword header, VU1 projects
 * them and XGKICKs the GIF packet.  The EE only writes state changes and headers.  The EE path
 * (kh_ge_use_vu1 = 0) does the same work on the EE; it is kept for A/B comparison. */
extern int kh_tex3d_bind(KhGsPacket *p, u32 teximage, u32 pltt, u64 *tex0, int *w, int *h);
extern u64 kh_gs_zbuf_value(int mask_writes);
extern float kh_gs_z_max(void);

int kh_ge_use_vu1 = 1;
#if KH_PS2_DEBUG
/* debugging aid (poke with ps2/tools/win/pine_poke.py): triangles whose texture format bit
 * (1 << fmt) is set are not drawn; bit 8: the opaque pass, bit 9: the translucent pass, bit 10:
 * polygons with alpha 0; bit 11: log this frame's runs (one frame) */
volatile u32 kh_ge_dbg_skip;
#endif
extern uint64_t kh_gs_frame_value(uint32_t fbmsk);


#define PACKED3 ((u64)GIF_REG_ST | ((u64)GIF_REG_RGBAQ << 4) | ((u64)GIF_REG_XYZ2 << 8))
#define VU1_MAX_VERTS 84            /* per batch: 6 + 3 * 84 input + 1 + 3 * 84 output <= 512 qwords */

static inline u64 fbits(float f) { union { float f; u32 u; } c; c.f = f; return c.u; }

typedef struct Screen {
    float scale[4], offset[4];      /* clip-space xyz / w -> GS x, y (pixels) and 24-bit Z */
} Screen;

/* cs: colour scale (MODULATE: vertex colour 0x80 = 1.0, so textured runs scale by 128/255);
 * ca: alpha scale */
static void batch_vu1(KhGsPacket *p, const Screen *sc, const GeVtx *v, int nv, int tw, int th, float cs, float ca)
{
    kh_gs_chain_vif(p, KH_VIF_NOP, KH_VIF_UNPACK_V4_32(6, 0, 1));
    kh_gs_packet_q(p, GIF_SET_TAG(nv, 1, 0, 0, GIF_FLG_PACKED, 3), PACKED3);
    kh_gs_packet_q(p, (u64)(u32)nv, 0);
    kh_gs_packet_q(p, fbits(1.0f / (float)tw) | (fbits(1.0f / (float)th) << 32), fbits(1.0f));
    kh_gs_packet_q(p, fbits(cs) | (fbits(cs) << 32), fbits(cs) | ((u64)fbits(ca) << 32));
    kh_gs_packet_q(p, fbits(sc->scale[0]) | (fbits(sc->scale[1]) << 32), fbits(sc->scale[2]));
    kh_gs_packet_q(p, fbits(sc->offset[0]) | (fbits(sc->offset[1]) << 32),
                   fbits(sc->offset[2]) | ((u64)fbits(sc->offset[3]) << 32));
    kh_gs_chain_vif_ref(p, KH_VIF_NOP, KH_VIF_UNPACK_V4_32(nv * 3, 6, 1), v, (u32)nv * 3);
    kh_gs_chain_vif_ref(p, KH_VIF_NOP, KH_VIF_MSCAL(0), NULL, 0);
    p->vu_busy = 1;
}

static void batch_ee(KhGsPacket *p, const Screen *sc, const GeVtx *v, int nv, int tw, int th, float cs, float ca)
{
    int k;
    float itw = 1.0f / (float)tw, ith = 1.0f / (float)th;
    kh_gs_packet_q(p, GIF_SET_TAG(nv, 1, 0, 0, GIF_FLG_PACKED, 3), PACKED3);
    for (k = 0; k < nv; k++, v++) {
        float q = v->w > 0.00001f ? 1.0f / v->w : 100000.0f;
        float x = v->x * q * sc->scale[0] + sc->offset[0];
        float y = v->y * q * sc->scale[1] + sc->offset[1];
        float z = v->z * q * sc->scale[2] + sc->offset[2];
        u32 gz = (u32)(z < 0 ? 0 : z > sc->offset[3] ? sc->offset[3] : z);
        int r = (int)((float)v->r * cs), gg = (int)((float)v->g * cs), b = (int)((float)v->b * cs);
        int a = (int)((float)v->a * ca);
        kh_gs_packet_q(p, fbits(v->s * itw * q) | (fbits(v->t * ith * q) << 32), fbits(q));
        kh_gs_packet_q(p, KH_PK_RGBAQ_LO(r, gg, b, a), KH_PK_RGBAQ_HI(r, gg, b, a));
        kh_gs_packet_q(p, KH_PK_XYZ2_LO((int)(x * 16.0f), (int)(y * 16.0f)), KH_PK_XYZ2_HI(gz));
    }
}

/* DS shadow polygons (POLYGON_ATTR mode 3).  The DS draws a shadow as a volume in two steps:
 * mask polygons (polygon ID 0) set a stencil bit wherever they fail the depth test (they lie
 * behind what is already drawn - inside the volume, the floor), then shadow polygons (ID != 0)
 * are drawn only where that bit is set.  Drawn as plain translucent polygons, both steps showed
 * the volume itself as a dark translucent shape around the character.
 * The GS has no stencil; frame-buffer alpha bit 7 serves as one (alpha is not used for display or
 * for blending, which reads the source alpha):
 *   mask:   alpha bit := 1 over the polygons (FBA forces bit 7, RGB not written, Z always), then
 *           := 0 where they pass the depth test (alpha written as 0) - leaving 1 where they fail;
 *   shadow: drawn with the destination alpha test (DATE, pass where bit 7 = 1). */
static void shadow_mask_run(KhGsPacket *p, const Screen *sc, const GeVtx *v, int nv, int use_vu1)
{
    int k;
    for (k = 0; k < 2; k++) {
        kh_gs_packet_ad_begin(p, 5);
        kh_gs_packet_q(p, kh_gs_frame_value(0x00ffffffu), GS_REG_FRAME_1);      /* alpha only */
        kh_gs_packet_q(p, GS_SET_FBA(k == 0), GS_REG_FBA_1);
        kh_gs_packet_q(p, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, k == 0 ? 1 : 3), GS_REG_TEST_1);
        kh_gs_packet_q(p, kh_gs_zbuf_value(1), GS_REG_ZBUF_1);
        kh_gs_packet_q(p, GS_SET_PRIM(GS_PRIM_TRIANGLE, 0, 0, 0, 0, 0, 0, 0, 0), GS_REG_PRIM);
        if (use_vu1)
            batch_vu1(p, sc, v, nv, 8, 8, 0.0f, 0.0f);
        else
            batch_ee(p, sc, v, nv, 8, 8, 0.0f, 0.0f);
    }
    kh_gs_packet_ad_begin(p, 3);
    kh_gs_packet_q(p, kh_gs_frame_value(0), GS_REG_FRAME_1);
    kh_gs_packet_q(p, GS_SET_FBA(0), GS_REG_FBA_1);
    kh_gs_packet_q(p, GS_SET_TEST(1, 6, 0, 0, 0, 0, 1, 3), GS_REG_TEST_1);
}

/* after a shadow: bit 7 over the volume's footprint back to "3D here" (it lies on 3D geometry) */
static void shadow_restore_coverage(KhGsPacket *p, const Screen *sc, const GeVtx *v, int nv, int use_vu1)
{
    kh_gs_packet_ad_begin(p, 5);
    kh_gs_packet_q(p, kh_gs_frame_value(0x00ffffffu), GS_REG_FRAME_1);
    kh_gs_packet_q(p, GS_SET_FBA(1), GS_REG_FBA_1);
    kh_gs_packet_q(p, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 1), GS_REG_TEST_1);
    kh_gs_packet_q(p, kh_gs_zbuf_value(1), GS_REG_ZBUF_1);
    kh_gs_packet_q(p, GS_SET_PRIM(GS_PRIM_TRIANGLE, 0, 0, 0, 0, 0, 0, 0, 0), GS_REG_PRIM);
    if (use_vu1)
        batch_vu1(p, sc, v, nv, 8, 8, 0.0f, 0.0f);
    else
        batch_ee(p, sc, v, nv, 8, 8, 0.0f, 0.0f);
    kh_gs_packet_ad_begin(p, 3);
    kh_gs_packet_q(p, kh_gs_frame_value(0), GS_REG_FRAME_1);
    kh_gs_packet_q(p, GS_SET_FBA(0), GS_REG_FBA_1);
    kh_gs_packet_q(p, GS_SET_TEST(1, 6, 0, 0, 0, 0, 1, 3), GS_REG_TEST_1);
}

void kh_ge_render(int ox, int oy, int ow, int oh)
{
    KhGsPacket *p = kh_gs_frame_packet();
    float vx1 = (float)(g.viewport & 0xff), vy1 = (float)((g.viewport >> 8) & 0xff);
    float vw = (float)((g.viewport >> 16) & 0xff) - vx1 + 1, vh = (float)((g.viewport >> 24) & 0xff) - vy1 + 1;
    float sx = (float)ow / 256.0f, sy = (float)oh / 192.0f;
    int use_vu1 = kh_ge_use_vu1;
    int i, pass, drawn = 0, cover;
    Screen sc;

    kh_prof_begin(KH_PROF_R3D);
    /* 3D coverage.  The DS blends translucent polygons only with what is already in the 3D
     * frame buffer: over the rear plane with alpha 0 (nothing 3D there) the polygon's colour is
     * written unblended, and the 3D layer reaches the 2D layers opaque unless BLDCNT makes BG0 a
     * first blend target (then its alpha blends it with the 2D layers).  Drawn straight onto the
     * 2D layers, translucent 3D (the title's A3I5 "358/2 Days") came out washed out.  Here frame
     * buffer alpha bit 7 marks 3D coverage: cleared over the 3D rectangle, set by opaque 3D and the
     * rear plane, and each translucent run is drawn unblended where it is clear (DATE, DATM 0) and
     * blended where it is set (DATM 1). */
    cover = !((*(volatile u16 *)(kh_ds_io + 0x50) & 1) && ((*(volatile u16 *)(kh_ds_io + 0x50) >> 6) & 3) == 1);
    if (cover) {
        kh_gs_packet_ad_begin(p, 10);
        kh_gs_packet_q(p, GS_SET_SCISSOR(ox, ox + ow - 1, oy, oy + oh - 1), GS_REG_SCISSOR_1);
        kh_gs_packet_q(p, kh_gs_frame_value(0x00ffffffu), GS_REG_FRAME_1);         /* alpha only */
        kh_gs_packet_q(p, GS_SET_FBA(0), GS_REG_FBA_1);
        kh_gs_packet_q(p, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 1), GS_REG_TEST_1);
        kh_gs_packet_q(p, kh_gs_zbuf_value(1), GS_REG_ZBUF_1);
        kh_gs_packet_q(p, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 0, 0), GS_REG_PRIM);
        kh_gs_packet_q(p, GS_SET_RGBAQ(0, 0, 0, 0, 0x3f800000), GS_REG_RGBAQ);
        kh_gs_packet_q(p, GS_SET_XYZ((KH_GS_OFS + ox) << 4, (KH_GS_OFS + oy) << 4, 0), GS_REG_XYZ2);
        kh_gs_packet_q(p, GS_SET_XYZ((KH_GS_OFS + ox + ow) << 4, (KH_GS_OFS + oy + oh) << 4, 0), GS_REG_XYZ2);
        kh_gs_packet_q(p, kh_gs_frame_value(0), GS_REG_FRAME_1);
    }
    /* the rear plane: where no polygon is drawn the 3D layer has the clear colour and alpha
     * (CLEAR_COLOR, G3X_SetClearColor); alpha 0 leaves the layers below visible */
    {
        u32 cc = *(volatile u32 *)(kh_ds_io + 0x350);
        u32 ca = (cc >> 16) & 31;
        if (ca) {
            int r = (int)((cc & 31) << 3), gg = (int)(((cc >> 5) & 31) << 3), b = (int)(((cc >> 10) & 31) << 3);
            kh_gs_packet_ad_begin(p, 8);
            kh_gs_packet_q(p, GS_SET_SCISSOR(ox, ox + ow - 1, oy, oy + oh - 1), GS_REG_SCISSOR_1);
            kh_gs_packet_q(p, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 1), GS_REG_TEST_1);   /* Z always */
            kh_gs_packet_q(p, kh_gs_zbuf_value(1), GS_REG_ZBUF_1);                    /* no Z write */
            kh_gs_packet_q(p, GS_SET_ALPHA(0, 1, 0, 1, 0), GS_REG_ALPHA_1);
            kh_gs_packet_q(p, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 0, 0, ca != 31, 0, 0, 0, 0), GS_REG_PRIM);
            kh_gs_packet_q(p, GS_SET_RGBAQ(r, gg, b, k_alpha_gs[ca], 0x3f800000), GS_REG_RGBAQ);
            kh_gs_packet_q(p, GS_SET_XYZ((KH_GS_OFS + ox) << 4, (KH_GS_OFS + oy) << 4, 0), GS_REG_XYZ2);
            kh_gs_packet_q(p, GS_SET_XYZ((KH_GS_OFS + ox + ow) << 4, (KH_GS_OFS + oy + oh) << 4, 0), GS_REG_XYZ2);
            if (cover) {                /* the rear plane is 3D content: mark it */
                kh_gs_packet_ad_begin(p, 5);
                kh_gs_packet_q(p, kh_gs_frame_value(0x00ffffffu), GS_REG_FRAME_1);
                kh_gs_packet_q(p, GS_SET_FBA(1), GS_REG_FBA_1);
                kh_gs_packet_q(p, GS_SET_XYZ((KH_GS_OFS + ox) << 4, (KH_GS_OFS + oy) << 4, 0), GS_REG_XYZ2);
                kh_gs_packet_q(p, GS_SET_XYZ((KH_GS_OFS + ox + ow) << 4, (KH_GS_OFS + oy + oh) << 4, 0), GS_REG_XYZ2);
                kh_gs_packet_q(p, kh_gs_frame_value(0), GS_REG_FRAME_1);
                kh_gs_packet_ad_begin(p, 1);
                kh_gs_packet_q(p, GS_SET_FBA(0), GS_REG_FBA_1);
            }
        }
    }
    if (!g_ntri[0] && !g_ntri[1]) {
        kh_gs_packet_ad_begin(p, 3);
        kh_gs_packet_q(p, GS_SET_SCISSOR(0, kh_video_width() - 1, 0, kh_video_height() - 1), GS_REG_SCISSOR_1);
        kh_gs_packet_q(p, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 2), GS_REG_TEST_1);
        kh_gs_packet_q(p, kh_gs_zbuf_value(0), GS_REG_ZBUF_1);
        kh_prof_end(KH_PROF_R3D);
        return;
    }
    /* x' = ((x/w + 1) / 2 * vw + vx1) * sx + OFS + ox; y is flipped (DS y grows upwards, viewport
     * from the bottom); Z: greater = nearer: (1 - (z/w + 1) / 2) * kh_gs_z_max() */
    sc.scale[0] = 0.5f * vw * sx;
    sc.offset[0] = (0.5f * vw + vx1) * sx + (float)(KH_GS_OFS + ox);
    sc.scale[1] = -0.5f * vh * sy;
    sc.offset[1] = (191.0f - 0.5f * vh - vy1) * sy + (float)(KH_GS_OFS + oy);
    sc.scale[2] = -0.5f * kh_gs_z_max();
    sc.offset[2] = 0.5f * kh_gs_z_max();
    sc.scale[3] = 0.0f;
    sc.offset[3] = kh_gs_z_max();

    kh_gs_packet_ad_begin(p, 4);
    kh_gs_packet_q(p, GS_SET_SCISSOR(ox, ox + ow - 1, oy, oy + oh - 1), GS_REG_SCISSOR_1);
    kh_gs_packet_q(p, GS_SET_TEST(1, 6, 0, 0, 0, 0, 1, 3), GS_REG_TEST_1);   /* alpha > 0, Z GREATER */
    kh_gs_packet_q(p, GS_SET_ALPHA(0, 1, 0, 1, 0), GS_REG_ALPHA_1);          /* (Cs-Cd)*As + Cd */
    kh_gs_packet_q(p, GS_SET_TEXA(0, 1, 0x80), GS_REG_TEXA);

    for (pass = 0; pass < 2; pass++) {
        const GeTriAttr *attr = g_attr[pass];
        const GeVtx *vtx = g_vtx[pass];
        int n = g_ntri[pass];
        u32 cur_tex = 0xffffffffu, cur_pltt = 0xffffffffu;
        int cur_textured = -1, cur_zmask = -1, tw = 8, th = 8;
        i = 0;
        while (i < n) {
            const GeTriAttr *t = &attr[i];
            int fmt = (int)((t->teximage >> 26) & 7);
            int textured = fmt != 0;
            int zmask = pass && !(t->polyattr & (1u << 11));
            int run;
            /* worst case per run: texture upload + state (~24 qwords) + VU1 batch (~12 qwords) or
             * EE batch (1 + 3 qwords per vertex) */
            if (p->len + 64 + (use_vu1 ? 0 : VU1_MAX_VERTS * 3) > p->cap)
                break;
            if (textured && (t->teximage != cur_tex || t->pltt != cur_pltt)) {
                u64 tex0;
                KH_PROF_ADD(KH_PC_TEXBIND, 1);
                if (kh_tex3d_bind(p, t->teximage, t->pltt, &tex0, &tw, &th)) {
                    kh_gs_packet_ad_begin(p, 2);
                    kh_gs_packet_q(p, tex0, GS_REG_TEX0_1);
                    kh_gs_packet_q(p, GS_SET_CLAMP((t->teximage >> 16) & 1 ? 0 : 1, (t->teximage >> 17) & 1 ? 0 : 1,
                                                   0, 0, 0, 0), GS_REG_CLAMP_1);
                    cur_tex = t->teximage;
                    cur_pltt = t->pltt;
                } else {
                    textured = 0;
                    cur_tex = 0xffffffffu;
                }
            }
            if (textured != cur_textured || zmask != cur_zmask) {
                KH_PROF_ADD(KH_PC_DRAWS, 1);
                kh_gs_packet_ad_begin(p, 2);
                kh_gs_packet_q(p, GS_SET_PRIM(GS_PRIM_TRIANGLE, 1, textured, 0, pass, 0, 0, 0, 0), GS_REG_PRIM);
                kh_gs_packet_q(p, kh_gs_zbuf_value(zmask), GS_REG_ZBUF_1);
                cur_textured = textured;
                cur_zmask = zmask;
            }
            /* the run: the following triangles with the same texture and Z mode (and, translucent,
             * the same polygon mode and ID: shadow polygons are drawn their own way) */
            for (run = 1; run < VU1_MAX_VERTS / 3 && i + run < n; run++) {
                const GeTriAttr *u = &attr[i + run];
                if (((u->teximage >> 26) & 7) != (u32)fmt ||
                    (fmt && (u->teximage != t->teximage || u->pltt != t->pltt)) ||
                    (pass && ((u->polyattr ^ t->polyattr) & ((1u << 11) | 0x3f000030u))))
                    break;
            }
            if (pass && ((t->polyattr >> 4) & 3) == 3) {
                KH_PROF_ADD(KH_PC_SHADOW, run);
                if (!((t->polyattr >> 24) & 63)) {
                    shadow_mask_run(p, &sc, &vtx[i * 3], run * 3, use_vu1);
                    cur_textured = cur_zmask = -1;        /* PRIM / ZBUF are sent again */
                    drawn += run;
                    i += run;
                    continue;
                }
                kh_gs_packet_ad_begin(p, 1);
                kh_gs_packet_q(p, GS_SET_TEST(1, 6, 0, 0, 1, 1, 1, 3), GS_REG_TEST_1);   /* + DATE */
            }
#if KH_PS2_DEBUG
            if (kh_ge_dbg_skip & 0x800) {       /* one-frame dump of the runs (cleared at frame end) */
                const GeVtx *v0 = &vtx[i * 3];
                KH_INFO("ge", "run pass %d n %d tex %08x pltt %04x attr %08x | v0 %.2f %.2f %.3f %.2f st %.1f %.1f rgba %d %d %d %d",
                        pass, run, (unsigned)t->teximage, (unsigned)t->pltt, (unsigned)t->polyattr,
                        (double)v0->x, (double)v0->y, (double)v0->z, (double)v0->w, (double)v0->s, (double)v0->t,
                        (int)v0->r, (int)v0->g, (int)v0->b, (int)v0->a);
            }
#endif
            {
                float cs = textured ? 128.0f / 255.0f : 1.0f;
#if KH_PS2_DEBUG
                if ((kh_ge_dbg_skip & ((1u << fmt) | (0x100u << pass))) ||
                    ((kh_ge_dbg_skip & 0x400) && !((t->polyattr >> 16) & 31)))
                    ;                           /* skipped (debugging) */
                else
#endif
                if (pass && cover && ((t->polyattr >> 4) & 3) != 3) {
                    int k;
                    for (k = 0; k < 2; k++) {   /* unblended over no 3D (FBA marks it), blended over 3D */
                        kh_gs_packet_ad_begin(p, 3);
                        kh_gs_packet_q(p, GS_SET_PRIM(GS_PRIM_TRIANGLE, 1, textured, 0, k, 0, 0, 0, 0), GS_REG_PRIM);
                        kh_gs_packet_q(p, GS_SET_TEST(1, 6, 0, 0, 1, k, 1, 3), GS_REG_TEST_1);
                        kh_gs_packet_q(p, GS_SET_FBA(k == 0), GS_REG_FBA_1);
                        if (use_vu1)
                            batch_vu1(p, &sc, &vtx[i * 3], run * 3, tw, th, cs, 1.0f);
                        else
                            batch_ee(p, &sc, &vtx[i * 3], run * 3, tw, th, cs, 1.0f);
                    }
                    kh_gs_packet_ad_begin(p, 2);
                    kh_gs_packet_q(p, GS_SET_TEST(1, 6, 0, 0, 0, 0, 1, 3), GS_REG_TEST_1);
                    kh_gs_packet_q(p, GS_SET_FBA(0), GS_REG_FBA_1);
                } else if (use_vu1)
                    batch_vu1(p, &sc, &vtx[i * 3], run * 3, tw, th, cs, 1.0f);
                else
                    batch_ee(p, &sc, &vtx[i * 3], run * 3, tw, th, cs, 1.0f);
            }
            if (pass && ((t->polyattr >> 4) & 3) == 3) {
                kh_gs_packet_ad_begin(p, 1);
                kh_gs_packet_q(p, GS_SET_TEST(1, 6, 0, 0, 0, 0, 1, 3), GS_REG_TEST_1);
                if (cover)
                    shadow_restore_coverage(p, &sc, &vtx[i * 3], run * 3, use_vu1);
                cur_textured = cur_zmask = -1;
            }
            drawn += run;
            i += run;
        }
    }
    KH_PROF_ADD(KH_PC_TRIS, drawn);
    kh_gs_packet_ad_begin(p, 3);
    kh_gs_packet_q(p, GS_SET_SCISSOR(0, kh_video_width() - 1, 0, kh_video_height() - 1), GS_REG_SCISSOR_1);
    kh_gs_packet_q(p, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 2), GS_REG_TEST_1);
    kh_gs_packet_q(p, kh_gs_zbuf_value(0), GS_REG_ZBUF_1);
    kh_prof_end(KH_PROF_R3D);
}

/* Called when the game presents a frame: the triangle lists start over. */
/* After a present the next frame's geometry starts empty - except when the game sent no geometry
 * at all for a frame: the DS then keeps showing the 3D frame it rendered last (the cutscene pause
 * menu stops the 3D scene while its 2D keeps being displayed), so the lists are kept and drawn
 * again until the next geometry command, which drops them first. */
void kh_ge_end_frame(void)
{
#if KH_PS2_DEBUG
    kh_ge_dbg_skip &= ~0x800u;
#endif
    if (!g_got_cmds) {
        g_kept = 1;
        return;
    }
    g_got_cmds = 0;
    g_kept = 1;
    g_overflow_logged = 0;
}
