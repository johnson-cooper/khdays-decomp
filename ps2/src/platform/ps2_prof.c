/* Lightweight frame profiler.  Compiled out of release builds with -DKH_NO_PROFILE. */
#include "platform/kh_platform.h"

#include <string.h>

static uint64_t g_start[KH_PROF_COUNT];
static uint64_t g_acc[KH_PROF_COUNT];
static uint64_t g_last_frame;
static KhProfStats g_cur, g_out;

#define TICKS_PER_US (KH_TICKS_PER_SEC / 1000000ull)

void kh_prof_begin(KhProfZone z)
{
#ifndef KH_NO_PROFILE
    g_start[z] = kh_time_ticks();
#endif
}

void kh_prof_end(KhProfZone z)
{
#ifndef KH_NO_PROFILE
    g_acc[z] += kh_time_ticks() - g_start[z];
#endif
}

void kh_prof_count(uint32_t draws, uint32_t tris, uint32_t verts)
{
    g_cur.draw_calls += draws;
    g_cur.triangles += tris;
    g_cur.vertices += verts;
}

void kh_prof_tex_upload(uint32_t bytes)
{
    g_cur.tex_uploads++;
    g_cur.tex_upload_bytes += bytes;
}

void kh_prof_frame(void)
{
    uint64_t now = kh_time_ticks();
    int i;
    if (g_last_frame) {
        uint64_t dt = now - g_last_frame;
        float fps = dt ? (float)KH_TICKS_PER_SEC / (float)dt : 0.0f;
        g_cur.fps = g_out.fps ? g_out.fps * 0.9f + fps * 0.1f : fps;
        g_acc[KH_PROF_FRAME] = dt;
    }
    g_last_frame = now;
    for (i = 0; i < KH_PROF_COUNT; i++) {
        g_cur.zone_us[i] = (uint32_t)(g_acc[i] / TICKS_PER_US);
        g_acc[i] = 0;
    }
    g_out = g_cur;
    memset(&g_cur, 0, sizeof g_cur);
    g_cur.fps = g_out.fps;
}

const KhProfStats *kh_prof_stats(void) { return &g_out; }
