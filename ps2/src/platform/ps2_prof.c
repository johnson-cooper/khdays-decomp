/* Lightweight frame profiler.
 *
 * Zone timing reads the EE cp0 Count register (CPU clock, 294.912 MHz): one mfc0 per zone change,
 * cheap enough for the geometry front end's per-call zones.  Zones of the game thread nest
 * exclusively (a stack: entering a zone pauses its parent), so per frame they add up to the frame
 * time.  Every KH_PERF_PERIOD frames the averages go to the log as one "[perf]" line.
 *
 * Disabled with KH_PS2_PROFILE=0: zones and counters compile to nothing, only the fps remains.
 */
#include "platform/kh_platform.h"

#include <string.h>

#define CPU_HZ 294912000u
#define KH_PERF_PERIOD 120

#if KH_PS2_PROFILE
uint32_t kh_prof_counters[KH_PC_COUNT];

static inline uint32_t count_reg(void)
{
    uint32_t c;
    __asm__ __volatile__("mfc0 %0, $9" : "=r"(c));
    return c;
}

static uint32_t g_acc[KH_PROF_COUNT];        /* cycles this frame */
static uint32_t g_async_start[KH_PROF_COUNT];
static uint8_t g_stack[32];
static int g_sp;
static uint32_t g_mark;                      /* Count at the last zone change */
static int g_frame_open;
#endif

static uint64_t g_last_frame;
static KhProfStats g_out;

#if KH_PS2_PROFILE
/* period sums */
static uint64_t g_sum_zone[KH_PROF_COUNT];
static uint64_t g_sum_cnt[KH_PC_COUNT];
static uint32_t g_max_frame;
static int g_nframes;
#endif

void kh_prof_begin(KhProfZone z)
{
#if KH_PS2_PROFILE
    uint32_t now = count_reg();
    if (z >= KH_PROF_ASYNC_FIRST) {
        g_async_start[z] = now;
        return;
    }
    if (g_sp < (int)sizeof g_stack) {
        g_acc[g_sp ? g_stack[g_sp - 1] : KH_PROF_FRAME] += now - g_mark;
        g_stack[g_sp++] = (uint8_t)z;
    }
    g_mark = now;
#else
    (void)z;
#endif
}

void kh_prof_end(KhProfZone z)
{
#if KH_PS2_PROFILE
    uint32_t now = count_reg();
    if (z >= KH_PROF_ASYNC_FIRST) {
        g_acc[z] += now - g_async_start[z];
        return;
    }
    if (g_sp && g_stack[g_sp - 1] == z) {
        g_acc[z] += now - g_mark;
        g_sp--;
    }
    g_mark = now;
#else
    (void)z;
#endif
}

void kh_prof_tex_upload(uint32_t bytes)
{
    KH_PROF_ADD(KH_PC_UPLOADS, 1);
    KH_PROF_ADD(KH_PC_UPLOAD_BYTES, bytes);
}

#if KH_PS2_PROFILE
static void report(void)
{
    extern volatile uint32_t kh_audio_underruns, kh_audio_queue_depth;
    double n = (double)g_nframes, us = 1e6 / CPU_HZ;
    uint64_t frame = 0;
    int i;
#define ZMS(z) ((double)g_sum_zone[z] * us / 1000.0 / n)
#define CNT(c) ((unsigned)(g_sum_cnt[c] / (uint64_t)g_nframes))
    for (i = 0; i < KH_PROF_ASYNC_FIRST; i++)
        frame += g_sum_zone[i];
    kh_log(KH_LOG_INFO, "perf",
           "%.1fms (max %.1f) %.1ffps | game %.1f ge %.1f r3d %.1f tex %.1f r2d %.1f gsw %.1f vbl %.1f "
           "vbt %.1f snd %.1f dbg %.1f misc %.1f | load %.1f audcpu %.1f",
           (double)frame * us / 1000.0 / n, (double)g_max_frame * us / 1000.0, g_out.fps,
           ZMS(KH_PROF_GAME), ZMS(KH_PROF_GE), ZMS(KH_PROF_R3D), ZMS(KH_PROF_TEX), ZMS(KH_PROF_R2D),
           ZMS(KH_PROF_GS_WAIT), ZMS(KH_PROF_VBLANK), ZMS(KH_PROF_VBTASK), ZMS(KH_PROF_SOUND),
           ZMS(KH_PROF_DEBUG), ZMS(KH_PROF_FRAME), ZMS(KH_PROF_LOAD), ZMS(KH_PROF_AUDIO));
    kh_log(KH_LOG_INFO, "perf", "audio cpu %.1fms queue %u underruns %u",
           ZMS(KH_PROF_AUDIO), (unsigned)kh_audio_queue_depth, (unsigned)kh_audio_underruns);
    kh_log(KH_LOG_INFO, "perf",
           "vtx %u poly %u cull %u clip %u off %u tri %u draw %u texbind %u miss %u stale %u | upload %u (%u KB) "
           "clut %u gif %u KB spr %u shadow %u",
           CNT(KH_PC_VERTS), CNT(KH_PC_POLYS), CNT(KH_PC_CULLED), CNT(KH_PC_CLIPPED), CNT(KH_PC_OFFSCREEN),
           CNT(KH_PC_TRIS), CNT(KH_PC_DRAWS), CNT(KH_PC_TEXBIND), CNT(KH_PC_TEXMISS), CNT(KH_PC_TEXSTALE), CNT(KH_PC_UPLOADS),
           CNT(KH_PC_UPLOAD_BYTES) / 1024, CNT(KH_PC_CLUTS), CNT(KH_PC_GIF_BYTES) / 1024, CNT(KH_PC_SPRITES), CNT(KH_PC_SHADOW));
#undef ZMS
#undef CNT
}
#endif

/* Called once per presented frame. */
void kh_prof_frame(void)
{
    uint64_t now = kh_time_ticks();
    if (g_last_frame) {
        uint64_t dt = now - g_last_frame;
        float fps = dt ? (float)KH_TICKS_PER_SEC / (float)dt : 0.0f;
        g_out.fps = g_out.fps ? g_out.fps * 0.9f + fps * 0.1f : fps;
    }
    g_last_frame = now;
#if KH_PS2_PROFILE
    {
        int i;
        uint32_t c = count_reg(), total = 0;
        /* close the open zones' share of this frame */
        g_acc[g_sp ? g_stack[g_sp - 1] : KH_PROF_FRAME] += c - g_mark;
        g_mark = c;
        for (i = 0; i < KH_PROF_COUNT; i++) {
            g_out.zone_us[i] = (uint32_t)((uint64_t)g_acc[i] * 1000000u / CPU_HZ);
            if (g_frame_open) {
                g_sum_zone[i] += g_acc[i];
                if (i < KH_PROF_ASYNC_FIRST)
                    total += g_acc[i];
            }
            g_acc[i] = 0;
        }
        for (i = 0; i < KH_PC_COUNT; i++) {
            g_out.count[i] = kh_prof_counters[i];
            if (g_frame_open)
                g_sum_cnt[i] += kh_prof_counters[i];
            kh_prof_counters[i] = 0;
        }
        if (g_frame_open) {
            if (total > g_max_frame)
                g_max_frame = total;
            if (++g_nframes >= KH_PERF_PERIOD) {
                report();
                memset(g_sum_zone, 0, sizeof g_sum_zone);
                memset(g_sum_cnt, 0, sizeof g_sum_cnt);
                g_max_frame = 0;
                g_nframes = 0;
            }
        }
        g_frame_open = 1;
    }
#endif
}

const KhProfStats *kh_prof_stats(void) { return &g_out; }
