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
extern int   Scene_AdvanceToPending(void);
extern void  kh_debug_mark(const char *stage, int a, int b);
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

/*
 * A scene object with flag 1 is intentionally not destroyed by Obj_UpdateAll when its state
 * becomes -2; the root BootTask notices it on its next update and Scene_AdvanceToPending performs
 * the protected teardown/overlay switch.  On the PS2 path we have observed the calendar reach
 * exactly that state (cur=5, pend=2, state=-2, flags=1) and then remain there indefinitely: the
 * following BootTask update is not guaranteed to run.
 *
 * Run the same dispatcher explicitly after the completed/presented frame whenever that invariant
 * is visible.  This is deliberately narrow: a live scene, an unprotected ordinary object, or a
 * scene with no pending replacement is untouched.  Calling the dispatcher again is harmless when
 * BootTask already advanced normally because pendId is cleared by a successful handoff.
 */
static void ps2_finish_pending_dead_scene(void)
{
    int *scene = (int *)gSceneCtl;
    int *obj = (int *)scene[0];

    if (obj != 0 && scene[3] != 0 && obj[5] == -2 && (obj[0] & 1) != 0) {
        kh_debug_mark("main: advance dead scene", scene[2], scene[3]);
        Scene_AdvanceToPending();
    }
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
        kh_prof_begin(KH_PROF_GAME);
        FrameStep_UpdateTaskQueue();
        Pad_Sample();
        G3X_ResetMtxStack();

        switch (gPauseMode) {
        case 0: Obj_UpdateAll(0); break;
        case 1: Callbacks_Run(1); frameTarget = VBlank_GetCount(); break;
        case 2: Obj_UpdateAll(0); Callbacks_Run(1); break;
        }
        kh_prof_end(KH_PROF_GAME);
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

        /* present: also in pause mode 1 (the cutscene pause menu), where the 3D scene is frozen -
         * the DS keeps displaying both screens, with the last 3D frame (nitro_ge.c keeps it while
         * no SWAP_BUFFERS arrives) and the pause menu's 2D */
        KhNitro_PresentFrame();
        if (gPauseMode != 1)
            data_0204c215 = 1;

        /* scene poll: on the DS the rest of this block is lid-close / sleep handling */
        (void)Game_PollSceneAlive();

        /*
         * The frame is already on the GS, so it is safe to tear down the finished scene here.
         * Normally BootTask performs this at the start of the next object update.  The explicit
         * guard prevents a PS2-only pause/scheduling edge from stranding a protected dead scene.
         */
        ps2_finish_pending_dead_scene();
    }
}
