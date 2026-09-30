/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_StartSession.c (ps2/tools/prep_sources.py). Do not edit. */
typedef struct {
    int nFlags;                         /* +0x00 */
    int nRate;                          /* +0x04 */
    int nPending;                       /* +0x08 */
} Ov002Session;

typedef int (*Ov002SessionProc)(void);

extern Ov002Session *NNSi_FndGetCurrentRootHeap(void);
extern unsigned long long OS_GetTick(void);
extern int kh_rt_ll_udiv_w_32(unsigned long long value, unsigned int arg2, int arg3);
extern int Ov002_StepSession(void);

/* Start a session: when the armed bit is set, clear the busy bit and hand back
 * the step routine. The current rate is recomputed either way. */
Ov002SessionProc Ov002_StartSession(void)
{
    Ov002SessionProc pfnStep;
    Ov002Session *pSession;

    pfnStep = 0;
    pSession = NNSi_FndGetCurrentRootHeap();

    if ((pSession->nFlags & 2) > 0) {
        pSession->nFlags &= ~4;
        pfnStep = Ov002_StepSession;
    }

    pSession->nRate = kh_rt_ll_udiv_w_32(OS_GetTick() << 6, 0x82ea, 0);
    pSession->nPending = 0;

    return pfnStep;
}
