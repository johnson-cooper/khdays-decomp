/* PS2 debug override: trace object work-area allocation and constructor entry/return. */
#include "platform/kh_platform.h"

extern void kh_debug_mark(const char *stage, int a, int b);
extern int Heap_SetCurrent(int arena);
extern void *AllocFromExpHeapWrapper(int size, int arena);
extern void MI_CpuFill8(void *dst, int val, int n);
extern void Obj_LinkNode(int obj);
extern char gObjSystem;

int *RunClassConstructor(int *obj, unsigned short *desc, int ctorArg)
{
    int def, saved, token;
    int classId = desc ? (int)desc[0] : -1;
    int groupId = desc ? (int)desc[1] : -1;

    kh_debug_mark("class run: entered", classId, groupId);

    obj[0] = 0;
    obj[1] = ((int *)&gObjSystem)[1];
    *(unsigned short *)((char *)obj + 0x10) = desc[0];
    *(unsigned short *)((char *)obj + 0x12) = desc[1];

    def = 0;
    if (*(int **)((char *)desc + 0x10) != 0)
        def = **(int **)((char *)desc + 0x10);

    obj[7] = def;
    obj[9] = *(int *)((char *)desc + 0xc);
    obj[5] = 0;
    obj[6] = *(int *)((char *)desc + 8);
    obj[10] = 0;

    kh_debug_mark("class run: set heap", obj[7], obj[9]);
    token = Heap_SetCurrent(obj[7]);

    if (obj[9] == 0) {
        obj[8] = 0;
    } else {
        kh_debug_mark("class run: alloc work", obj[9], obj[7]);
        obj[8] = (int)AllocFromExpHeapWrapper(obj[9], obj[7]);
        kh_debug_mark("class run: work allocated", obj[8], obj[9]);
        MI_CpuFill8((void *)obj[8], 0, obj[9]);
        kh_debug_mark("class run: work cleared", obj[8], obj[9]);
    }

    Obj_LinkNode((int)obj);
    kh_debug_mark("class run: object linked", classId, groupId);

    saved = ((int *)&gObjSystem)[1];
    ((int *)&gObjSystem)[1] = (int)obj;

    kh_debug_mark("class run: call ctor", classId, groupId);
    *(int *)(((int *)&gObjSystem)[1] + 0x14) =
        (*(int (**)(int))((char *)desc + 4))(ctorArg);
    kh_debug_mark("class run: ctor returned", classId,
                   *(int *)(((int *)&gObjSystem)[1] + 0x14));

    ((int *)&gObjSystem)[1] = saved;
    Heap_SetCurrent(token);
    kh_debug_mark("class run: complete", classId, groupId);
    return obj;
}
