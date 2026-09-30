/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_StepCaptionFade.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Ov002_StepCaptionFade - drive the sub-screen brightness from the caption's
 * fade tween.
 *
 * Nothing happens while the caller says the screen is busy, or once the tween
 * has finished. Otherwise the divider context is saved around the sample -
 * this runs from an interrupt-time path, so the caller's division must survive
 * it - and the sampled level goes straight into the sub-screen's master
 * brightness once the divider is idle again.
 *
 * ARM.
 */

typedef struct {
    int aWords[6];
    unsigned int pad0 : 2;
    unsigned int bDone : 1;
} Ov002Tween;

typedef struct {
    char pad000[0x58];
    Ov002Tween tweenFade;
} Ov002TextScene;

extern int data_ov002_0207f62c;

extern void CP_SaveContext(void *pContext);
extern void CPi_RestoreContext(const void *pContext);
extern void Tween_Sample(Ov002Tween *pTween, int *pOut);
extern void G2x_ChangeBlendBrightness_(int nReg, int nLevel);

extern int Ov002_GetPanelField01b0(void);

void Ov002_StepCaptionFade(void)
{
    char aCpContext[0x1c];
    int nLevel;
    Ov002TextScene *s;
    volatile unsigned short *reg_divcnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x280);

    s = *(Ov002TextScene **)((char *)&data_ov002_0207f62c + 4);
    if (Ov002_GetPanelField01b0() != 0) {
        return;
    }
    if (s->tweenFade.bDone != 0) {
        return;
    }

    CP_SaveContext(aCpContext);
    Tween_Sample(&s->tweenFade, &nLevel);
    CPi_RestoreContext(aCpContext);

    while (*reg_divcnt & 0x8000) ;
    G2x_ChangeBlendBrightness_(((unsigned int)kh_ds_io + 0x1050), nLevel >> 12);
}
