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
typedef struct { fx32 m[4][4]; } Mtx44;
typedef struct { fx32 m[3][3]; } Mtx33;

/* ------------------------------------------------------------------ state */

enum { MODE_PROJ = 0, MODE_POS = 1, MODE_POSVEC = 2, MODE_TEX = 3 };

typedef struct GeVtx { float x, y, z, w; float s, t; u8 r, g, b, a; } GeVtx;   /* clip space */

#define MAX_TRIS 8192
typedef struct GeTri {
    GeVtx v[3];
    u32 polyattr, teximage, pltt;
} GeTri;

static struct {
    int mode;
    Mtx44 proj, pos, tex;
    Mtx33 vec;
    Mtx44 proj_stack[1];
    Mtx44 pos_stack[31];
    Mtx33 vec_stack[31];
    Mtx44 tex_stack[1];
    int proj_sp, pos_sp;
    Mtx44 clip;           /* pos * proj */
    int clip_dirty;

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
    GeVtx strip[4];
    int nstrip, stripcount;

    u32 viewport;         /* x1 | y1<<8 | x2<<16 | y2<<24 */
    int swap_mode;
} g;

static GeTri g_tris[MAX_TRIS];
static int g_ntris;
static int g_overflow_logged;

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

/* d = a * b (row vectors, as the DS: new = a x current) */
static void m44_mul(Mtx44 *d, const Mtx44 *a, const Mtx44 *b)
{
    Mtx44 r;
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            r.m[i][j] = (fx32)(((s64)a->m[i][0] * b->m[0][j] + (s64)a->m[i][1] * b->m[1][j] +
                                (s64)a->m[i][2] * b->m[2][j] + (s64)a->m[i][3] * b->m[3][j]) >> 12);
    *d = r;
}

static void m33_mul(Mtx33 *d, const Mtx33 *a, const Mtx33 *b)
{
    Mtx33 r;
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            r.m[i][j] = (fx32)(((s64)a->m[i][0] * b->m[0][j] + (s64)a->m[i][1] * b->m[1][j] +
                                (s64)a->m[i][2] * b->m[2][j]) >> 12);
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
    if (g.clip_dirty) {
        m44_mul(&g.clip, &g.pos, &g.proj);
        g.clip_dirty = 0;
    }
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
        break;
    }
    g.clip_dirty = 1;
}

/* ----------------------------------------------------------------- output */

static void emit_tri(const GeVtx *a, const GeVtx *b, const GeVtx *c)
{
    GeTri *t;
    if (g_ntris >= MAX_TRIS) {
        if (!g_overflow_logged) {
            g_overflow_logged = 1;
            KH_WARN("ge", "triangle list full (%d)", MAX_TRIS);
        }
        return;
    }
    t = &g_tris[g_ntris++];
    t->v[0] = *a;
    t->v[1] = *b;
    t->v[2] = *c;
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
            o->r = (u8)(a->r + (b->r - a->r) * t);
            o->g = (u8)(a->g + (b->g - a->g) * t);
            o->b = (u8)(a->b + (b->b - a->b) * t);
            o->a = (u8)(a->a + (b->a - a->a) * t);
        }
    }
    return m;
}

static void emit_poly(const GeVtx *v, int n)
{
    GeVtx buf0[12], buf1[12];
    int i, axis;
    float cross;
    u32 pa = g.polyattr;

    /* culling on the unclipped polygon, in clip space projected (sign of the w-divided area) */
    {
        float x0 = v[0].x / v[0].w, y0 = v[0].y / v[0].w;
        float x1 = v[1].x / v[1].w, y1 = v[1].y / v[1].w;
        float x2 = v[2].x / v[2].w, y2 = v[2].y / v[2].w;
        if (v[0].w > 0 && v[1].w > 0 && v[2].w > 0) {
            cross = (x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0);
            if (cross > 0 && !(pa & 0x80)) return;         /* front face not rendered */
            if (cross < 0 && !(pa & 0x40)) return;         /* back face not rendered */
        }
    }
    memcpy(buf0, v, sizeof(GeVtx) * (size_t)n);
    for (axis = 0; axis < 3; axis++) {
        n = clip_plane(buf0, n, buf1, axis, 1.0f);
        if (n < 3) return;
        n = clip_plane(buf1, n, buf0, axis, -1.0f);
        if (n < 3) return;
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

static void submit_vertex(fx32 x, fx32 y, fx32 z)
{
    GeVtx v;
    int alpha = (int)((g.polyattr >> 16) & 31);
    g.vx = x; g.vy = y; g.vz = z;
    if ((g.teximage >> 30) == 3)
        texcoord_transform(3, x, y, z);
    update_clip();
    v.x = (float)(((s64)x * g.clip.m[0][0] + (s64)y * g.clip.m[1][0] + (s64)z * g.clip.m[2][0] + ((s64)g.clip.m[3][0] << 12)) >> 12) / 4096.0f;
    v.y = (float)(((s64)x * g.clip.m[0][1] + (s64)y * g.clip.m[1][1] + (s64)z * g.clip.m[2][1] + ((s64)g.clip.m[3][1] << 12)) >> 12) / 4096.0f;
    v.z = (float)(((s64)x * g.clip.m[0][2] + (s64)y * g.clip.m[1][2] + (s64)z * g.clip.m[2][2] + ((s64)g.clip.m[3][2] << 12)) >> 12) / 4096.0f;
    v.w = (float)(((s64)x * g.clip.m[0][3] + (s64)y * g.clip.m[1][3] + (s64)z * g.clip.m[2][3] + ((s64)g.clip.m[3][3] << 12)) >> 12) / 4096.0f;
    v.s = (float)g.ns / 16.0f;
    v.t = (float)g.nt / 16.0f;
    v.r = (u8)(g.cr << 3 | g.cr >> 2);
    v.g = (u8)(g.cg << 3 | g.cg >> 2);
    v.b = (u8)(g.cb << 3 | g.cb >> 2);
    v.a = (u8)(alpha == 0 ? 0x80 : (alpha * 0x80) / 31);   /* alpha 0 = wireframe: drawn solid for now */

    if (g.prim < 0)
        return;
    g.strip[g.nstrip++] = v;
    switch (g.prim) {
    case 0:     /* separate triangles */
        if (g.nstrip == 3) { emit_poly(g.strip, 3); g.nstrip = 0; }
        break;
    case 1:     /* separate quads */
        if (g.nstrip == 4) { emit_poly(g.strip, 4); g.nstrip = 0; }
        break;
    case 2:     /* triangle strip: alternate winding */
        if (g.nstrip == 3) {
            GeVtx t[3];
            if (g.stripcount & 1) { t[0] = g.strip[1]; t[1] = g.strip[0]; t[2] = g.strip[2]; }
            else { t[0] = g.strip[0]; t[1] = g.strip[1]; t[2] = g.strip[2]; }
            emit_poly(t, 3);
            g.strip[0] = g.strip[1];
            g.strip[1] = g.strip[2];
            g.nstrip = 2;
            g.stripcount++;
        }
        break;
    case 3:     /* quad strip: v0 v1 v3 v2 */
        if (g.nstrip == 4) {
            GeVtx q[4] = { g.strip[0], g.strip[1], g.strip[3], g.strip[2] };
            emit_poly(q, 4);
            g.strip[0] = g.strip[2];
            g.strip[1] = g.strip[3];
            g.nstrip = 2;
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

void kh_ge_cmd(u32 cmd, const u32 *p)
{
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
        g.clip_dirty = 1;
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
        g.clip_dirty = 1;
        break;
    case 0x15:
        m44_identity(&m);
        if (g.mode == MODE_PROJ) g.proj = m;
        else if (g.mode == MODE_POS) g.pos = m;
        else if (g.mode == MODE_POSVEC) { g.pos = m; m33_identity(&g.vec); }
        else g.tex = m;
        g.clip_dirty = 1;
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
        g.stripcount = 0;
        g.polyattr = g.polyattr_pending;
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
            kh_ge_cmd(c, f.params);     /* parameterless command */
        f.cur++;
    }
    f.ncmds = 0;
}

void kh_ge_fifo(const u32 *w, u32 n)
{
    while (n--) {
        u32 v = *w++;
        if (f.ncmds == 0) {
            f.cmds[0] = (u8)v; f.cmds[1] = (u8)(v >> 8); f.cmds[2] = (u8)(v >> 16); f.cmds[3] = (u8)(v >> 24);
            f.ncmds = 4;
            /* trailing NOP bytes end the packet */
            while (f.ncmds > 1 && f.cmds[f.ncmds - 1] == 0)
                f.ncmds--;
            f.cur = 0;
            fifo_next_cmd();
            continue;
        }
        f.params[f.nparams++] = v;
        if (f.nparams == f.need) {
            kh_ge_cmd(f.cmds[f.cur], f.params);
            f.cur++;
            fifo_next_cmd();
        }
    }
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
    g.clip_dirty = 1;
    g.prim = -1;
    g.viewport = 0xbfff0000u;
    g.polyattr = g.polyattr_pending = 0x001f00c0u;     /* opaque, both faces */
    f.ncmds = 0;
}

/* ----------------------------------------------------------- GS output */

/* Draw this frame's triangles into the rectangle that shows the 3D layer.
 * Pass 0: opaque polygons (alpha 31).  Pass 1: translucent polygons, depth written only when the
 * polygon asks for it (POLYGON_ATTR bit 11), as the DS draws translucent polygons after opaque ones. */
extern int kh_tex3d_bind(KhGsPacket *p, u32 teximage, u32 pltt, u64 *tex0, int *w, int *h);
extern u64 kh_gs_zbuf_value(int mask_writes);

#define PACKED3 ((u64)GIF_REG_ST | ((u64)GIF_REG_RGBAQ << 4) | ((u64)GIF_REG_XYZ2 << 8))

static inline u64 fbits(float f) { union { float f; u32 u; } c; c.f = f; return c.u; }

void kh_ge_render(int ox, int oy, int ow, int oh)
{
    KhGsPacket *p = kh_gs_frame_packet();
    float vx1 = (float)(g.viewport & 0xff), vy1 = (float)((g.viewport >> 8) & 0xff);
    float vx2 = (float)((g.viewport >> 16) & 0xff), vy2 = (float)((g.viewport >> 24) & 0xff);
    float sx = (float)ow / 256.0f, sy = (float)oh / 192.0f;
    int i, k, pass, drawn = 0;

    if (!g_ntris)
        return;
    kh_gs_packet_ad_begin(p, 4);
    kh_gs_packet_q(p, GS_SET_SCISSOR(ox, ox + ow - 1, oy, oy + oh - 1), GS_REG_SCISSOR_1);
    kh_gs_packet_q(p, GS_SET_TEST(1, 6, 0, 0, 0, 0, 1, 3), GS_REG_TEST_1);   /* alpha > 0, Z GREATER */
    kh_gs_packet_q(p, GS_SET_ALPHA(0, 1, 0, 1, 0), GS_REG_ALPHA_1);          /* (Cs-Cd)*As + Cd */
    kh_gs_packet_q(p, GS_SET_TEXA(0, 1, 0x80), GS_REG_TEXA);

    for (pass = 0; pass < 2; pass++) {
        u32 cur_tex = 0xffffffffu, cur_pltt = 0xffffffffu;
        int cur_textured = -1, cur_zmask = -1, tw = 8, th = 8;
        for (i = 0; i < g_ntris; i++) {
            GeTri *t = &g_tris[i];
            int alpha = (int)((t->polyattr >> 16) & 31);
            int translucent = alpha != 31 && alpha != 0;
            int textured = ((t->teximage >> 26) & 7) != 0;
            int zmask = translucent && !(t->polyattr & (1u << 11));
            if (translucent != pass)
                continue;
            if (p->len + 16 > p->cap)
                break;
            if (textured && (t->teximage != cur_tex || t->pltt != cur_pltt)) {
                u64 tex0;
                if (kh_tex3d_bind(p, t->teximage, t->pltt, &tex0, &tw, &th)) {
                    kh_gs_packet_ad_begin(p, 2);
                    kh_gs_packet_q(p, tex0, GS_REG_TEX0_1);
                    kh_gs_packet_q(p, GS_SET_CLAMP((t->teximage >> 16) & 1 ? 0 : 1, (t->teximage >> 17) & 1 ? 0 : 1,
                                                   0, 0, 0, 0), GS_REG_CLAMP_1);
                    cur_tex = t->teximage;
                    cur_pltt = t->pltt;
                } else {
                    textured = 0;
                }
            }
            if (textured != cur_textured || zmask != cur_zmask) {
                kh_gs_packet_ad_begin(p, 2);
                kh_gs_packet_q(p, GS_SET_PRIM(GS_PRIM_TRIANGLE, 1, textured, 0, translucent, 0, 0, 0, 0), GS_REG_PRIM);
                kh_gs_packet_q(p, kh_gs_zbuf_value(zmask), GS_REG_ZBUF_1);
                cur_textured = textured;
                cur_zmask = zmask;
            }
            kh_gs_packet_q(p, GIF_SET_TAG(3, 1, 0, 0, GIF_FLG_PACKED, 3), PACKED3);
            for (k = 0; k < 3; k++) {
                const GeVtx *v = &t->v[k];
                float q = v->w > 0.00001f ? 1.0f / v->w : 100000.0f;
                float x = (v->x * q + 1.0f) * 0.5f * (vx2 - vx1 + 1) + vx1;
                float y = 191.0f - ((v->y * q + 1.0f) * 0.5f * (vy2 - vy1 + 1) + vy1);
                float z = (v->z * q + 1.0f) * 0.5f;
                u32 gz = (u32)((1.0f - (z < 0 ? 0 : z > 1 ? 1 : z)) * 16777215.0f);
                /* textured (modulate): vertex colour 0x80 = 1.0 on the GS */
                int r = textured ? (v->r * 0x80) / 255 : v->r;
                int gg = textured ? (v->g * 0x80) / 255 : v->g;
                int b = textured ? (v->b * 0x80) / 255 : v->b;
                kh_gs_packet_q(p, fbits(v->s / (float)tw * q) | (fbits(v->t / (float)th * q) << 32), fbits(q));
                kh_gs_packet_q(p, KH_PK_RGBAQ_LO(r, gg, b, v->a), KH_PK_RGBAQ_HI(r, gg, b, v->a));
                kh_gs_packet_q(p, KH_PK_XYZ2_LO((int)((KH_GS_OFS + ox + x * sx) * 16.0f), (int)((KH_GS_OFS + oy + y * sy) * 16.0f)),
                               KH_PK_XYZ2_HI(gz));
            }
            drawn++;
        }
    }
    kh_prof_count(1, (u32)drawn, (u32)drawn * 3);
    kh_gs_packet_ad_begin(p, 3);
    kh_gs_packet_q(p, GS_SET_SCISSOR(0, kh_video_width() - 1, 0, kh_video_height() - 1), GS_REG_SCISSOR_1);
    kh_gs_packet_q(p, GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 2), GS_REG_TEST_1);
    kh_gs_packet_q(p, kh_gs_zbuf_value(0), GS_REG_ZBUF_1);
}

/* Called when the game presents a frame: the triangle list starts over. */
void kh_ge_end_frame(void)
{
    g_ntris = 0;
    g_overflow_logged = 0;
}
