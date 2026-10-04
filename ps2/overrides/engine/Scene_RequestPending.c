/* PS2 debug override: make pending scene requests visible on hardware. */
#include "platform/kh_platform.h"

extern void kh_debug_stage(const char *stage, int a, int b);
extern int data_0204bda4;

void Scene_RequestPending(int arg0, int arg1)
{
    kh_debug_stage("scene request: pending", arg0, arg1);
    *(int *)((char *)&data_0204bda4 + 0x10) = arg0;
    *(int *)((char *)&data_0204bda4 + 0x14) = arg1;
}
