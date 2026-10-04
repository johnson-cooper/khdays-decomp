/* PS2 debug override: bracket the pre-movie subtitle/record drain. */
#include "platform/kh_platform.h"

extern void kh_debug_stage(const char *stage, int a, int b);
extern int ByteCode_ResolveOperand(void *a, unsigned short *param_2);
extern void Ov012_ArmAndStart(int x);
extern void Ov012_MobiClip_DrainStream(int x, int y);
extern void Ov012_ReleaseField50IfSet(int x);

int Ov012_InitAndDispatchTriple(char *arg1, unsigned short *param_2)
{
    int v;
    int stream;

    kh_debug_stage("ov012 cmd2: resolve operand", 0, 0);
    v = ByteCode_ResolveOperand(arg1, param_2);
    stream = *(int *)(*(int *)(arg1 + 0x128) + 0xc);

    kh_debug_stage("ov012 cmd2: ArmAndStart", v, stream);
    Ov012_ArmAndStart(stream);

    kh_debug_stage("ov012 cmd2: DrainStream", v, stream);
    Ov012_MobiClip_DrainStream(stream, v);

    kh_debug_stage("ov012 cmd2: ReleaseField50", v, stream);
    Ov012_ReleaseField50IfSet(stream);
    return 1;
}
