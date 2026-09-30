/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_SetupRenderState.c (ps2/tools/prep_sources.py). Do not edit. */
/* Sets up the battle's 3D rendering: VRAM banks, display control, texture and palette VRAM
 * managers, display swap and clear colour, then loads the shop table. */

#include "nitro/types.h"

extern void GX_SetBankForTex(int bank);
extern void GX_SetBankForTexPltt(int offset);
extern void NNS_GfdResetFrmTexVramState(void);
extern void NNS_GfdInitFrmTexVramManager(int mode, int enable);
extern void NNS_GfdSetFrmTexVramState(void *matrixState);
extern void NNS_GfdResetFrmPlttVramState(void);
extern void NNS_GfdInitFrmPlttVramManager(int value, int installCallbacks);
extern void Ov002_SetDisplaySwap(int top);
extern void G3X_SetClearColor(int red, int green, int blue, int alpha, int polygonId);
extern void Ov002_LoadShopTable(void);

static volatile u16 *const REG_DISP3DCNT = (volatile u16 *)((unsigned int)kh_ds_io + 0x60);

void Ov022_SetupRenderState(void)
{
    u32 mask;
    u32 matrixState[10];

    GX_SetBankForTex(0xf);
    GX_SetBankForTexPltt(0x60);

    mask = 0xffffcffd;
    *REG_DISP3DCNT = *REG_DISP3DCNT & mask;
    *REG_DISP3DCNT = *REG_DISP3DCNT & 0xcffb;
    *REG_DISP3DCNT = (*REG_DISP3DCNT & ~0x3000) | 8;

    NNS_GfdResetFrmTexVramState();
    NNS_GfdInitFrmTexVramManager(4, 1);

    matrixState[0] = 0;
    matrixState[1] = 0x20000;
    matrixState[2] = 0;
    matrixState[3] = 0x20000;
    matrixState[4] = 0;
    matrixState[5] = 0;
    matrixState[6] = 0;
    matrixState[7] = 0x20000;
    matrixState[8] = 0;
    matrixState[9] = 0x20000;
    NNS_GfdSetFrmTexVramState(matrixState);
    NNS_GfdResetFrmPlttVramState();
    NNS_GfdInitFrmPlttVramManager(0x8000, 1);
    Ov002_SetDisplaySwap(1);
    G3X_SetClearColor(0, 0x1f, 0x7fff, 0x3f, 0);
    Ov002_LoadShopTable();
}
