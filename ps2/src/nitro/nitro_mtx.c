/* NitroSDK MTX functions whose SDK versions are ARM assembly or drive the divider, in C with
 * the SDK's exact fixed-point behaviour.  (The C ones -- concat, multiply, look-at, ... -- are
 * the decomp's own libs/nitro/mtx sources.) */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

#include <string.h>

typedef int32_t fx32;
typedef struct { fx32 m[2][2]; } MtxFx22;
typedef struct { fx32 m[3][3]; } MtxFx33;
typedef struct { fx32 m[4][3]; } MtxFx43;
typedef struct { fx32 m[4][4]; } MtxFx44;
#define ONE 0x1000

void MTX_Identity22_(MtxFx22 *m) { memset(m, 0, sizeof *m); m->m[0][0] = m->m[1][1] = ONE; }
void MTX_Identity33_(MtxFx33 *m) { memset(m, 0, sizeof *m); m->m[0][0] = m->m[1][1] = m->m[2][2] = ONE; }
void MTX_Identity43_(MtxFx43 *m) { memset(m, 0, sizeof *m); m->m[0][0] = m->m[1][1] = m->m[2][2] = ONE; }
void MTX_Identity44_(MtxFx44 *m) { memset(m, 0, sizeof *m); m->m[0][0] = m->m[1][1] = m->m[2][2] = m->m[3][3] = ONE; }

void MTX_Copy43To44_(const MtxFx43 *s, MtxFx44 *d)
{
    int r;
    for (r = 0; r < 4; r++) {
        d->m[r][0] = s->m[r][0];
        d->m[r][1] = s->m[r][1];
        d->m[r][2] = s->m[r][2];
        d->m[r][3] = r == 3 ? ONE : 0;
    }
}

void MTX_Copy44To43_(const MtxFx44 *s, MtxFx43 *d)
{
    int r;
    for (r = 0; r < 4; r++) {
        d->m[r][0] = s->m[r][0];
        d->m[r][1] = s->m[r][1];
        d->m[r][2] = s->m[r][2];
    }
}

/* Rotations (SDK layout: rows are basis vectors; RotX: [1 0 0; 0 c s; 0 -s c]) */
void MTX_RotX33_(MtxFx33 *m, fx32 s, fx32 c)
{
    MTX_Identity33_(m);
    m->m[1][1] = c; m->m[1][2] = s;
    m->m[2][1] = -s; m->m[2][2] = c;
}

void MTX_RotY33_(MtxFx33 *m, fx32 s, fx32 c)
{
    MTX_Identity33_(m);
    m->m[0][0] = c; m->m[0][2] = -s;
    m->m[2][0] = s; m->m[2][2] = c;
}

void MTX_RotZ33_(MtxFx33 *m, fx32 s, fx32 c)
{
    MTX_Identity33_(m);
    m->m[0][0] = c; m->m[0][1] = s;
    m->m[1][0] = -s; m->m[1][1] = c;
}

void MTX_RotX43_(MtxFx43 *m, fx32 s, fx32 c)
{
    MTX_Identity43_(m);
    m->m[1][1] = c; m->m[1][2] = s;
    m->m[2][1] = -s; m->m[2][2] = c;
}

void MTX_RotY43_(MtxFx43 *m, fx32 s, fx32 c)
{
    MTX_Identity43_(m);
    m->m[0][0] = c; m->m[0][2] = -s;
    m->m[2][0] = s; m->m[2][2] = c;
}

void MTX_RotZ43_(MtxFx43 *m, fx32 s, fx32 c)
{
    MTX_Identity43_(m);
    m->m[0][0] = c; m->m[0][1] = s;
    m->m[1][0] = -s; m->m[1][1] = c;
}

void MTX_Rot22_(MtxFx22 *m, fx32 s, fx32 c)
{
    m->m[0][0] = c; m->m[0][1] = s;
    m->m[1][0] = -s; m->m[1][1] = c;
}

/* MTX_OrthoW and Camera_BuildProjectionMtx (MTX_PerspectiveW) are the decomp's own C
 * (libs/nitro/mtx/calls), kept through ps2/config/lib_keep.txt: they program the divider's operand
 * registers and read the result with FX_GetDivResult*, which the divider state block
 * (nitro_cp.c) serves exactly. */
