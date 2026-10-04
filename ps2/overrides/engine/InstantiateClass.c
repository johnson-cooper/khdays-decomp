/* PS2 debug override: trace class allocation and constructor dispatch. */
#include "platform/kh_platform.h"

extern void kh_debug_stage(const char *stage, int a, int b);
extern void *data_0204c024;
extern void *AllocFromExpHeapWrapper(int size, void *heap);
extern int *RunClassConstructor(void *ptr, void *desc, int ctorArg);

int *InstantiateClass(void *classDesc, int ctorArg)
{
    void *ptr;

    kh_debug_stage("class: alloc object", (int)classDesc, ctorArg);
    ptr = AllocFromExpHeapWrapper(0x2c, data_0204c024);
    kh_debug_stage("class: object allocated", (int)ptr, (int)classDesc);

    kh_debug_stage("class: RunClassConstructor", (int)classDesc, ctorArg);
    ptr = RunClassConstructor(ptr, classDesc, ctorArg);
    kh_debug_stage("class: constructor returned", (int)ptr, (int)classDesc);
    return (int *)ptr;
}
