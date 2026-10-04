/* PS2 debug override: bracket the script command that starts the opening movie. */
#include "game/engine.h"
#include "platform/kh_platform.h"

extern void kh_debug_stage(const char *stage, int a, int b);
extern void Ov012_StartOpeningMovie(int x);

int Ov012_StartOpeningMovieFromScriptArgs(void *arg1, char *arg2)
{
    int v = 0;

    kh_debug_stage("ov012 cmd4: resolve movie args", 0, 0);
    if (*(short *)(arg2 + 0) == 2)
        v = ByteCode_ResolveOperand(arg1, arg2);
    if (*(short *)(arg2 + 8) == 2)
        ByteCode_ResolveOperand(arg1, arg2 + 8);
    if (*(short *)(arg2 + 0x10) == 2)
        ByteCode_ResolveOperand(arg1, arg2 + 0x10);

    kh_debug_stage("ov012 cmd4: StartOpeningMovie", v, 0);
    Ov012_StartOpeningMovie(v);
    kh_debug_stage("ov012 cmd4: movie start returned", v, 0);
    return 1;
}
