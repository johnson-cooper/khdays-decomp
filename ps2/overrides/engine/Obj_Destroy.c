/* PS2 debug override: breadcrumb the nested scene-object destruction path.
 *
 * Calendar teardown destroys the protected class-8/group-15 scene object, whose method then
 * destroys its class-8/group-14 child.  Keep the original Obj_Destroy semantics exactly, but mark
 * each irreversible boundary so a hardware watchdog screen identifies the step that stopped.
 */
#include "platform/kh_platform.h"

extern int  Heap_SetCurrent(int arena);
extern void Obj_UnlinkNode(int node);
extern void NNSi_FndFreeFromDefaultHeap(void *p);
extern void ExpHeap_Free(void *obj, void *heap);
extern int  gObjSystem[];
extern int  data_0204c024[];
extern void kh_debug_mark(const char *stage, int a, int b);

int Obj_Destroy(int *param_1)
{
    int token;
    void (*cb)(void);
    int saved_current;
    int class_id;
    int group_id;

    class_id = *(unsigned short *)((char *)param_1 + 0x10);
    group_id = *(unsigned short *)((char *)param_1 + 0x12);
    kh_debug_mark("destroy: enter", class_id, group_id);

    token = Heap_SetCurrent(param_1[7]);
    saved_current = gObjSystem[1];
    gObjSystem[1] = (int)param_1;

    cb = (void (*)(void))param_1[6];
    if (cb != 0) {
        kh_debug_mark("destroy: method", class_id, group_id);
        cb();
        kh_debug_mark("destroy: method done", class_id, group_id);
    }

    *param_1 = 0;
    gObjSystem[1] = saved_current;

    Obj_UnlinkNode((int)param_1);
    kh_debug_mark("destroy: unlinked", class_id, group_id);

    saved_current = param_1[3];
    if (param_1[8] != 0) {
        kh_debug_mark("destroy: free aux", class_id, group_id);
        NNSi_FndFreeFromDefaultHeap((void *)param_1[8]);
        kh_debug_mark("destroy: aux freed", class_id, group_id);
    }

    kh_debug_mark("destroy: free self", class_id, group_id);
    ExpHeap_Free(param_1, (void *)data_0204c024[0]);
    kh_debug_mark("destroy: self freed", class_id, group_id);

    Heap_SetCurrent(token);
    kh_debug_mark("destroy: complete", class_id, group_id);
    return saved_current;
}
