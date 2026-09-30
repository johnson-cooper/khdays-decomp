/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_WaitDwellTimer.c (ps2/tools/prep_sources.py). Do not edit. */
/* Waits 800 ms after the scale tweens, then starts the header and footer tweens. */

typedef struct Ov022Tween {
    int values[7];
} Ov022Tween;

typedef struct Ov022RootContext {
    char padding000[0xc4];
    int timestamp;
    Ov022Tween tweenHeader;
    Ov022Tween tweenFooter;
} Ov022RootContext;

extern Ov022RootContext *NNSi_FndGetCurrentRootHeap(void);
extern long long OS_GetTick(void);
extern unsigned long long kh_rt_ll_udiv_w(long long value, int divisor, int flag);
extern void Tween_Configure(Ov022Tween *tween, int mode, int start, int target,
                          int duration);
extern void Tween_Start(Ov022Tween *tween);
extern void func_ov022_02086e80(int count);
extern int Ov022_StepHeaderFooterTweens(void);
extern int data_0204be04;

int Ov022_WaitDwellTimer(void)
{
    Ov022RootContext *context = NNSi_FndGetCurrentRootHeap();
    int result = 0;
    unsigned long long now;
    int limit;

    if (*(unsigned char *)&data_0204be04 != 0) {
        return 0;
    }

    now = kh_rt_ll_udiv_w(OS_GetTick() << 6, 0x82ea, 0);
    limit = context->timestamp + 800;
    if (now > (unsigned long long)(long long)limit) {
        Tween_Configure(&context->tweenHeader, 2, 0x1000, 0x3000, 200);
        Tween_Start(&context->tweenHeader);
        Tween_Configure(&context->tweenFooter, 2, 0x1000, 0, 200);
        Tween_Start(&context->tweenFooter);
        result = (int)Ov022_StepHeaderFooterTweens;
    }
    func_ov022_02086e80(4);
    return result;
}
