/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_EnterDimmedScene.c (ps2/tools/prep_sources.py). Do not edit. */
/* Scene entry: allocate nothing -- the object is the current root heap block --
 * zero its first 0x1e0 bytes, seed the two 0xffff cursor fields and the -1
 * selection, load the two animation resources, then bring up the sub display
 * darkened (G2x_SetBlendBrightness_ on 0x04001050 = the SUB engine's BLDY, plane
 * mask 0x2f, ev -16) and register the per-frame step at Ov002_StepCaptionFade.
 * Returns the next scene step.
 *
 * data_0204c240 bit 2 is the boot-mode gate the anti-tamper path also reads: only
 * when it is set does this scene build the dialog object at +0x04.
 *
 * `unsigned char` on data_0204c240 is load-bearing -- signed gives ldrsb and an
 * extra zero register, and the total size still comes out right because mwcc then
 * reuses that zero for the following argument. */

#include "game/engine.h"

typedef struct {
    char pad00[4];
    void *pDialog;          /* +0x04 */
    int nUnk08;             /* +0x08 */
    char pad0c[0x4c];
    int tween[7];           /* +0x58 */
    void *pStepObject;      /* +0x74 */
    void *pAnimA;           /* +0x78 */
    void *pAnimB;           /* +0x7c */
    char pad80[4];
    unsigned short wCursorA; /* +0x84 */
    unsigned short wCursorB; /* +0x86 */
    char pad88[0x24];
    int nSelected;          /* +0xac */
    char padb0[0x104];
    int aSubCtx[1];          /* +0x1b4 */
} Ov002SceneContext;

extern void *NNSi_FndGetCurrentRootHeap(void);
extern void MI_CpuFill8(void *dst, unsigned char value, unsigned int size);
extern void *Msg_OpenContainerAndReadHeader(const void *res, int a);
extern void Ov002_InitResourceRecord(void *dst, const void *src);
extern int Ov002_ForwardToSubDc(int id);
extern void Ov002_Ctx_InvokeTagTrackerCallback(int);
extern void Ov002_SetUpSubScreen(void);
extern void Ov002_UploadFileToSubBg1Char(void);
extern void Ov002_OpenCaptionSurfaces(void);
extern void Tween_Clear(void *tween);
extern void G2x_SetBlendBrightness_(unsigned int reg, int planeMask, int ev);
extern void *InstantiateClass(const void *res, int a);
extern void *Ov002_CreateStepNode(void *fn);
extern void Ov002_ForwardToSubDc_5(int a);
extern void Ov002_RequestCaption(int a, int b);
extern void Ov002_StepCaptionFade(void);
extern void Ov002_StepCaptionScreen(void);

extern char *data_ov002_0207f62c;
extern char gOv002UiBtlMapPath[];
extern char gOv002UiBtlMapchrPath[];
extern char gOv002UiBtlInfoTextPath[];
extern char data_ov002_0207ee70[];
extern unsigned char data_0204c240;

void *Ov002_EnterDimmedScene(void) {
    Ov002SceneContext *ctx = NNSi_FndGetCurrentRootHeap();

    (&data_ov002_0207f62c)[1] = (char *)ctx;
    MI_CpuFill8(ctx, 0, 0x1e0);
    ctx->wCursorA = 0xffff;
    ctx->wCursorB = 0xffff;
    ctx->nSelected = -1;
    ctx->nUnk08 = 0;
    ctx->pAnimA = Msg_OpenContainerAndReadHeader(gOv002UiBtlMapPath, 0xe);
    ctx->pAnimB = Msg_OpenContainerAndReadHeader(gOv002UiBtlMapchrPath, 0xe);
    Ov002_InitResourceRecord(ctx->aSubCtx, gOv002UiBtlInfoTextPath);
    Ov002_Ctx_InvokeTagTrackerCallback(Ov002_ForwardToSubDc(0x3e8));
    Ov002_SetUpSubScreen();
    Ov002_UploadFileToSubBg1Char();
    Ov002_OpenCaptionSurfaces();
    Tween_Clear(ctx->tween);
    G2x_SetBlendBrightness_(((unsigned int)kh_ds_io + 0x1050), 0x2f, -0x10);
    if ((data_0204c240 & 4) != 0) {
        ctx->pDialog = InstantiateClass(data_ov002_0207ee70, 0);
    }
    ctx->pStepObject = Ov002_CreateStepNode((void *)&Ov002_StepCaptionFade);
    Touch_StartAutoSampling();
    Ov002_ForwardToSubDc_5(0);
    Ov002_RequestCaption(0, 0);
    return (void *)&Ov002_StepCaptionScreen;
}
