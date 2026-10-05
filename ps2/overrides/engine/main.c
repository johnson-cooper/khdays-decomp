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
extern void  KhNitro_DiscardFrame(void);   /* abandon an unsubmitted dead-scene frame */

/* ---- globals ---- */
extern unsigned char data_027e0060;   /* current scene id (0 = none) */
extern void         *data_027e0350;   /* boot resource list head */
extern int           data_0204c024;   /* default arena ref */
extern unsigned char data_0204c215;   /* "present pending" flag */
extern unsigned char gPauseMode;      /* display mode byte (0/1/2) */
extern unsigned char gObjSystem;      /* frame-rate/skip mode byte */
extern char          gSceneCtl[];     /* obj, entry, curId, pendId, pendArg */
extern void         *gBootTaskClass;

/* Valid only while SCENE_CALENDAR/ov004 is resident. */
extern void         *data_ov004_02051384;

struct SceneState { unsigned char phase; unsigned char _p[3]; int handle; };
extern struct SceneState data_020442a0;

static int ps2_calendar_active(void)
{
    return ((int *)gSceneCtl)[2] == SCENE_CALENDAR;
}

/*
 * ov004 is unusual: its large child object marks transitionComplete at +0x5550,
 * but the protected parent object does not observe that result until its NEXT
 * Obj_UpdateAll pass.  On DS that means one more ordinary frame is presented.
 *
 * Real PS2 hardware wedges while presenting precisely that in-between frame.
 * Keep BootTask as the sole scene dispatcher and preserve the original frame
 * separation: the next VBlank still drains queued VRAM transfers, the parent
 * then requests SCENE_FIELD, and a following BootTask pass performs teardown.
 * We suppress only the GS submission while the completed child waits for those
 * authentic update passes.
 */
static int ps2_calendar_child_complete(void)
{
    int *scene = (int *)gSceneCtl;
    unsigned char *ctx;

    if (scene[2] != SCENE_CALENDAR)
        return 0;

    ctx = (unsigned char *)data_ov004_02051384;
    if (ctx == 0)
        return 0;

    return *(int *)(ctx + 0x5550) != 0;
}

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
        OS_WaitVBlankIntr();

        frameTarget = VBlank_GetCount();
        kh_watchdog_fast_report = ps2_calendar_active();
        if (ps2_calendar_active())
            kh_debug_mark("calendar main: tasks", frameTarget, gPauseMode);
        kh_prof_begin(KH_PROF_GAME);
        FrameStep_UpdateTaskQueue();
        if (ps2_calendar_active())
            kh_debug_mark("calendar main: input", frameTarget, gPauseMode);
        Pad_Sample();
        if (ps2_calendar_active())
            kh_debug_mark("calendar main: matrix", frameTarget, gPauseMode);
        G3X_ResetMtxStack();

        if (ps2_calendar_active())
            kh_debug_mark("calendar main: objects", frameTarget, gPauseMode);
        switch (gPauseMode) {
        case 0: Obj_UpdateAll(0); break;
        case 1: Callbacks_Run(1); frameTarget = VBlank_GetCount(); break;
        case 2: Obj_UpdateAll(0); Callbacks_Run(1); break;
        }
        kh_prof_end(KH_PROF_GAME);
        if (ps2_calendar_active())
            kh_debug_mark("calendar main: sound", frameTarget, gPauseMode);
        kh_prof_begin(KH_PROF_SOUND);
        SoundMgr_Update();
        kh_prof_end(KH_PROF_SOUND);

        /* frame-rate pacing: advance whole frames until we reach the target */
        if (gObjSystem != 2) {
            frameTarget += (gObjSystem == 1) ? 2 : 1;
            while (VBlank_GetCount() < frameTarget) {
                OS_WaitVBlankIntr();
                kh_prof_begin(KH_PROF_GAME);
                FrameStep_UpdateTaskQueue();
                kh_prof_end(KH_PROF_GAME);
            }
        }

        /*
         * ov004 completion pump.
         *
         * The child can set done=1 late in Obj_UpdateAll, after the parent already
         * ran this frame.  The parent therefore needs another update pass to request
         * scene 2, and BootTask needs the following pass to perform the protected
         * teardown/load.  Hardware hangs if we submit the completed calendar frame
         * in between those passes, so abandon only this CPU-built frame and move on
         * to the next VBlank/update cycle.
         *
         * KhNitro_DiscardFrame does not free ov004 or recreate its heap, and it does
         * not consume the VRAM-transfer queue.  FrameStep_UpdateTaskQueue drains that
         * queue on the next authentic frame before any teardown can occur.
         */
        if (ps2_calendar_child_complete()) {
            int *scene = (int *)gSceneCtl;
            int *obj = (int *)scene[0];
            int state = obj ? obj[5] : 0;

            kh_debug_mark("calendar main: suppress final present",
                          scene[3], state);
            KhNitro_DiscardFrame();
            continue;
        }

        /* present: also in pause mode 1 (the cutscene pause menu), where the 3D scene is frozen -
         * the DS keeps displaying both screens, with the last 3D frame (nitro_ge.c keeps it while
         * no SWAP_BUFFERS arrives) and the pause menu's 2D */
        if (ps2_calendar_active())
            kh_debug_mark("calendar main: present", frameTarget, gPauseMode);
        KhNitro_PresentFrame();
        if (gPauseMode != 1)
            data_0204c215 = 1;

        /* scene poll: on the DS the rest of this block is lid-close / sleep handling */
        if (ps2_calendar_active())
            kh_debug_mark("calendar main: scene poll", frameTarget, gPauseMode);
        (void)Game_PollSceneAlive();
    }
}
