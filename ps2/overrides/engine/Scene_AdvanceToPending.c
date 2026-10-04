/* PS2 debug override: trace the exact release/load/instantiate boundary between scenes. */
#include "platform/kh_platform.h"

typedef struct SceneEntry {
    int overlayId;
    void *classDesc;
} SceneEntry;

typedef struct SceneCtl {
    void *obj;
    SceneEntry *entry;
    int curId;
    int pendId;
    int pendArg;
} SceneCtl;

extern void kh_debug_stage(const char *stage, int a, int b);
extern void kh_debug_mark(const char *stage, int a, int b);
extern int data_0204bda4;
extern char gSceneCtl[];
extern SceneEntry gSceneTable[];
extern void *data_0204c02c;

extern int Instance_ReleaseIfDead(void *obj);
extern void UnloadOverlaySync(int module, int overlayId);
extern void Callbacks_Init(void);
extern void HeapState_Recreate(void *);
extern void LoadOverlaySync(int module, int overlayId);
extern void *InstantiateClass(void *classDesc, int arg);
extern void Word_Set(void *obj, int);

int Scene_AdvanceToPending(void)
{
    SceneCtl *s = (SceneCtl *)gSceneCtl;
    static void *lastObj;
    static int lastPend;

    if (s->obj != 0) {
        if (s->pendId != 0) {
            int state = ((int *)s->obj)[5];
            if (lastObj != s->obj || lastPend != s->pendId) {
                lastObj = s->obj;
                lastPend = s->pendId;
                kh_debug_mark("scene advance: pending/current", s->pendId, state);
            }
        }

        if (((int *)s->obj)[5] == -2) {
            /* This mark is intentionally before Instance_ReleaseIfDead(): that helper destroys
             * protected scene objects synchronously.  If teardown itself stalls, the watchdog
             * must still tell us that the dispatcher reached the release boundary. */
            kh_debug_mark("scene advance: release dead", s->pendId, ((int *)s->obj)[0]);
        }

        if (Instance_ReleaseIfDead(s->obj) != 0) {
            kh_debug_stage("scene advance: current is dead", s->curId, s->pendId);
            if (s->entry->overlayId != -1) {
                kh_debug_stage("scene advance: unload current ov", s->entry->overlayId, s->curId);
                UnloadOverlaySync(0, s->entry->overlayId);
                kh_debug_stage("scene advance: current ov unloaded", s->entry->overlayId, s->curId);
            }

            kh_debug_stage("scene advance: callbacks init", s->curId, s->pendId);
            Callbacks_Init();
            kh_debug_stage("scene advance: recreate heap", s->curId, s->pendId);
            HeapState_Recreate(data_0204c02c);
            kh_debug_stage("scene advance: heap recreated", s->curId, s->pendId);

            s->obj = 0;
            s->curId = 0;
        }
    }

    if (s->obj == 0) {
        int id = s->pendId;
        if (id != 0) {
            SceneEntry *ent = &gSceneTable[id];
            int ov = ent->overlayId;

            kh_debug_stage("scene advance: begin pending", id, ov);
            if (ov != -1) {
                kh_debug_stage("scene advance: LoadOverlaySync", id, ov);
                LoadOverlaySync(0, ov);
                kh_debug_stage("scene advance: overlay loaded", id, ov);
            }

            /* Do not draw a full probe frame here: the opening-movie path may still own VIF/GIF.
             * A synchronous probe draw can itself become the apparent hang.  Record the boundary
             * only; the exception/watchdog screen will report it if the following call stalls. */
            kh_debug_mark("scene advance: InstantiateClass", id, s->pendArg);
            {
                void *obj = InstantiateClass(ent->classDesc, s->pendArg);
                kh_debug_stage("scene advance: class instantiated", id, (int)obj);
                s->obj = obj;
                s->entry = ent;
                kh_debug_stage("scene advance: Word_Set", id, 1);
                Word_Set(obj, 1);
                kh_debug_stage("scene advance: Word_Set returned", id, 1);
            }

            s->curId = s->pendId;
            s->pendId = 0;
            s->pendArg = 0;
            kh_debug_stage("scene advance: complete", s->curId, 0);
        }
    }
    return 1;
}
