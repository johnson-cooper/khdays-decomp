/* PS2 adaptation of main (src/engine/main.c, 0x02000bcc) -- the game's boot and frame loop.
 *
 * Kept step for step with the DS function; the differences are all platform:
 *   - it is entered as kh_game_main() from the PS2 entry point (ps2/src/main) after platform
 *     bring-up, instead of from crt0;
 *   - presenting a frame is KhNitro_PresentFrame() (flush the recorded 3D/2D state to the GS)
 *     instead of GXi_FlushCommandList() + a write to the geometry engine's SWAP_BUFFERS
 *     register (0x04000540);
 *   - the lid / sleep handling (the ARM7 hinge bit, PM_* power management) does not exist on a
 *     PS2 and is dropped.  The scene-ended path keeps only what is not sleep-related.
 */

#include "nitro/types.h"

#include "game/scene.h"
#include "platform/kh_prof.h"
typedef u32 FSOverlayID;

extern u32 OVERLAY_1_ID[1];
#define FS_OVERLAY_ID_ov001 ((FSOverlayID)(u32)&OVERLAY_1_ID)

/* ---- core services / overlays ---- */
extern void  OS_Init(void);
extern void  FS_Init(int mode);
extern int   FS_LoadOverlay(int proc, FSOverlayID overlay);
extern int   FS_UnloadOverlay(int proc, FSOverlayID overlay);
extern void  Ov001_BootInit(void);
extern void  PublishArchiveVTableAndSeedRng(void);
extern void  InputState_Init(void);
struct BootSettings {
    unsigned char mode;
    unsigned char flags;
    unsigned char byte2;
    unsigned char byte3;
    unsigned char data04[0x14];
    unsigned short reserved18;
    unsigned short field1a;
    unsigned char data1c[0x34];
    unsigned short reserved50;
    unsigned short field52;
};

extern void  Game_ReadLocalProfile(struct BootSettings *out);
extern void  Record_EnsureAllocatedAndSetId(int mode, int arg);
extern int   func_02016264(void *list);
extern int   FS_TryLoadTable(int a, int b);
extern int   AllocFromExpHeapWrapper(int handle, int arena);
extern void  Obj_InitSystem(void);
extern void  Callbacks_Init(void);
extern void  SoundCtx_Init(void);
extern void  InstantiateClass(void *classDesc, int ctorArg);

/* ---- per-frame ---- */
extern void  OS_WaitVBlankIntr(void);
extern unsigned int VBlank_GetCount(void);
extern void  FrameStep_UpdateTaskQueue(void);
extern void  Pad_Sample(void);
extern void  G3X_ResetMtxStack(void);
extern void  Obj_UpdateAll(int a);
extern void  Callbacks_Run(int a);
extern void  SoundMgr_Update(void);
extern int   Game_PollSceneAlive(void);
extern void  kh_debug_mark(const char *stage, int a, int b);
extern volatile int kh_watchdog_fast_report;
extern void  KhNitro_PresentFrame(void);   /* ps2/src/nitro: GX flush + swap */

/* ---- globals ---- */
extern unsigned char data_027e0060;   /* current scene id (0 = none) */
extern void         *data_027e0350;   /* boot resource list head */
extern int           data_0204c024;   /* default arena ref */
extern unsigned char data_0204c215;   /* "present pending" flag */
extern unsigned char gPauseMode;      /* display mode byte (0/1/2) */
extern unsigned char gObjSystem;      /* frame-rate/skip mode byte */
extern char          gSceneCtl[];     /* obj, entry, curId, pendId, pendArg */
extern void         *gBootTaskClass;

struct SceneState { unsigned char phase; unsigned char _p[3]; int handle; };
extern struct SceneState data_020442a0;

static int ps2_calendar_active(void)
{
    return ((int *)gSceneCtl)[2] == SCENE_CALENDAR;
}

/*
 * Main-loop breadcrumbs for the calendar -> field handoff.
 *
 * These go to the watchdog mark only (kh_debug_loop_mark), never to the overlay's "DBG" stage.
 * The overlay is drawn INSIDE KhNitro_PresentFrame, so a "present" stage written just before it
 * appeared on every successfully presented frame - which made a finished, flipped frame look like
 * the frame the GS hung on.  The DBG line now keeps the last game-side breadcrumb (ov004 frame,
 * calendar parent, scene dispatcher, field constructor).
 *
 * Tracing stays on for a while after ov004 is left so the field load and its first frames are
 * covered too.
 */
#define PS2_HANDOFF_TRACE_FRAMES 1200
static int g_handoff_trace;

static int ps2_handoff_trace_active(void)
{
    if (ps2_calendar_active()) {
        g_handoff_trace = PS2_HANDOFF_TRACE_FRAMES;
        return 1;
    }
    if (g_handoff_trace > 0) {
        g_handoff_trace--;
        return 1;
    }
    return 0;
}

/*
 * Frame pacing guard.
 *
 * The DS loop waits `while (VBlank_GetCount() < frameTarget)`.  The target is at most two VBlanks
 * ahead and on the DS the VBlank IRQ always advances the counter, so this ends within two waits.
 * On the PS2 the game's counter (data_027e0088) only advances when nitro_core delivers the DS
 * VBlank IRQ to the game's handler: if that handler is masked or replaced, the canonical loop
 * spins forever.  (The pause path rewinds the counter via Scene_Leave, but case 1 above re-reads
 * the target after Callbacks_Run, so that path never reaches this bound.)  Each spin still calls OS_WaitVBlankIntr, so the
 * hang watchdog sees a live loop and never reports, and nothing is presented: the TV keeps the
 * last good frame (exactly the "frozen DAY card with no watchdog screen" symptom).
 *
 * Bound the wait well above anything legitimate and report it instead of hanging silently.
 * The DBG line then reads "main: pacing counter stalled a=<game count> b=<irq mask << 16 |
 * target & 0xffff>".
 */
#define PS2_PACING_MAX_WAITS 8

/*
 * Live handoff trace arming (ps2_probe.c kh_debug_live_marks).  Armed at the top of the frame
 * after the calendar parent requested the field (cur=5, pend!=0): that frame's BootTask pass is
 * the protected ov004 teardown + ov002 load.  Disarmed once the new scene has presented
 * PS2_LIVE_TRACE_PRESENTS frames, so normal gameplay is not slowed by per-mark GS uploads.
 */
#define PS2_LIVE_TRACE_PRESENTS 90
static int g_live_armed;
static int g_live_presents;

extern void kh_debug_live_marks(int on);

static void ps2_live_trace_update(int after_present)
{
    int *scene = (int *)gSceneCtl;

    if (!after_present) {
        if (!g_live_armed && scene[2] == SCENE_CALENDAR && scene[3] != 0) {
            g_live_armed = 1;
            g_live_presents = 0;
            kh_debug_live_marks(1);
        }
        return;
    }
    if (g_live_armed && scene[2] != SCENE_CALENDAR && ++g_live_presents >= PS2_LIVE_TRACE_PRESENTS) {
        g_live_armed = 0;
        kh_debug_live_marks(0);
    }
}

extern unsigned int OS_GetIrqMask(void);
extern void kh_debug_loop_mark(const char *stage, int a, int b);

void kh_game_main(void) {
    struct BootSettings boot;
    unsigned int  frameTarget;

    /* --- 1. core services + ov001 one-shot init --- */
    OS_Init();
    FS_Init(3);
    FS_LoadOverlay(0, FS_OVERLAY_ID_ov001);
    Ov001_BootInit();
    FS_UnloadOverlay(0, FS_OVERLAY_ID_ov001);
    PublishArchiveVTableAndSeedRng();
    InputState_Init();

    /* --- 2. boot mode -> initial game mode --- */
    Game_ReadLocalProfile(&boot);
    {
        int mode = boot.mode;
        switch (mode) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            break;
        default:
            mode = 1;
            break;
        }
        Record_EnsureAllocatedAndSetId(mode, 0);
    }

    /* --- 3. boot resource tables + root/boot task --- */
    func_02016264(&data_027e0350);
    data_020442a0.handle = AllocFromExpHeapWrapper(FS_TryLoadTable(0, 0), data_0204c024);
    FS_TryLoadTable(data_020442a0.handle, FS_TryLoadTable(0, 0));
    Obj_InitSystem();
    data_0204c215 = 0;
    Callbacks_Init();
    SoundCtx_Init();
    InstantiateClass(&gBootTaskClass, 1);   /* -> BootTask_Construct -> Scene 1 */

    /* --- 4. FRAME LOOP --- */
    for (;;) {
        int trace;

        OS_WaitVBlankIntr();

        ps2_live_trace_update(0);
        frameTarget = VBlank_GetCount();
        trace = ps2_handoff_trace_active();
        kh_watchdog_fast_report = ps2_calendar_active();   /* covers the ov004 -> ov002 load too */
        if (trace)
            kh_debug_loop_mark("main: tasks", frameTarget, gPauseMode);
        kh_prof_begin(KH_PROF_GAME);
        FrameStep_UpdateTaskQueue();
        if (trace)
            kh_debug_loop_mark("main: input", frameTarget, gPauseMode);
        Pad_Sample();
        if (trace)
            kh_debug_loop_mark("main: matrix", frameTarget, gPauseMode);
        G3X_ResetMtxStack();

        if (trace)
            kh_debug_loop_mark("main: objects", frameTarget, gPauseMode);
        switch (gPauseMode) {
        case 0: Obj_UpdateAll(0); break;
        case 1: Callbacks_Run(1); frameTarget = VBlank_GetCount(); break;
        case 2: Obj_UpdateAll(0); Callbacks_Run(1); break;
        }
        kh_prof_end(KH_PROF_GAME);
        if (trace)
            kh_debug_loop_mark("main: sound", frameTarget, gPauseMode);
        kh_prof_begin(KH_PROF_SOUND);
        SoundMgr_Update();
        kh_prof_end(KH_PROF_SOUND);

        /* frame-rate pacing: advance whole frames until we reach the target */
        if (gObjSystem != 2) {
            int waits = 0;

            frameTarget += (gObjSystem == 1) ? 2 : 1;
            if (trace)
                kh_debug_loop_mark("main: pacing", frameTarget, VBlank_GetCount());
            while (VBlank_GetCount() < frameTarget) {
                if (++waits > PS2_PACING_MAX_WAITS) {
                    /* PS2-only: see PS2_PACING_MAX_WAITS.  Visible on the overlay's DBG line. */
                    kh_debug_mark("main: pacing counter stalled", (int)VBlank_GetCount(),
                                  (int)((frameTarget & 0xffffu) | (OS_GetIrqMask() << 16)));
                    break;
                }
                OS_WaitVBlankIntr();
                kh_prof_begin(KH_PROF_GAME);
                FrameStep_UpdateTaskQueue();
                kh_prof_end(KH_PROF_GAME);
            }
        }

        /* present: also in pause mode 1 (the cutscene pause menu), where the 3D scene is frozen -
         * the DS keeps displaying both screens, with the last 3D frame (nitro_ge.c keeps it while
         * no SWAP_BUFFERS arrives) and the pause menu's 2D.
         *
         * Every frame is presented, as on the DS, including ov004's phase-3 -> 4 frame and the
         * frames on which its child sets +0x5550 / its parent returns -2.  Earlier PS2 builds
         * presented those frames successfully on hardware (the overlay showed done=1 / pend=2 /
         * state=-2), so they are not GS-toxic; skipping them only hid where the CPU went next. */
        if (trace)
            kh_debug_loop_mark("main: present", frameTarget, gPauseMode);
        KhNitro_PresentFrame();
        ps2_live_trace_update(1);
        if (gPauseMode != 1)
            data_0204c215 = 1;

        /* scene poll: on the DS the rest of this block is lid-close / sleep handling */
        if (trace)
            kh_debug_loop_mark("main: scene poll", frameTarget, gPauseMode);
        (void)Game_PollSceneAlive();
    }
}
