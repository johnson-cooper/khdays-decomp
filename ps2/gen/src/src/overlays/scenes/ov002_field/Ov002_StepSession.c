/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_StepSession.c (ps2/tools/prep_sources.py). Do not edit. */
typedef struct {
    int nFlags;                         /* +0x00, bit 0 restart, bit 2 busy */
    int nRate;                          /* +0x04 */
    int nPending;                       /* +0x08 */
} Ov002Session;

typedef int (*Ov002SessionProc)(void);

extern Ov002Session *NNSi_FndGetCurrentRootHeap(void);
extern unsigned long long OS_GetTick(void);
extern int kh_rt_ll_udiv_w_32(unsigned long long value, unsigned int nDiv, int nMode);
extern void Ov002_ReportElapsed(void);
extern int Ov002_StartSession(void);

/* Step the session. A restart request hands back the start routine; otherwise
 * a busy session just refreshes its rate and an idle one is nudged. */
Ov002SessionProc Ov002_StepSession(void)
{
    Ov002Session *pSession;
    int nFlags;

    pSession = NNSi_FndGetCurrentRootHeap();
    nFlags = pSession->nFlags;

    if ((nFlags & 1) > 0) {
        return Ov002_StartSession;
    }

    if ((nFlags & 4) > 0) {
        pSession->nRate = kh_rt_ll_udiv_w_32(OS_GetTick() << 6, 0x82ea, 0);
        return 0;
    }

    Ov002_ReportElapsed();
    return 0;
}
