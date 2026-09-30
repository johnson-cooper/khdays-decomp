/* PS2: mechanically prepared copy of src/engine/Gfx_SetupSubEngine.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/engine.h"

extern int GX_ResetBankForSubOBJ();
extern int GX_ResetBankForSubBG();
extern int GX_SetBankForTex(int);
extern int GX_SetBankForSubBG(int);
extern int GX_SetBankForSubOBJ(int);
extern int GXS_SetGraphicsMode(int);
extern void GX_SetGraphicsMode(int a, int b, int c);
extern int NNS_GfdResetFrmTexVramState(void);
extern int NNS_GfdInitFrmTexVramManager(int a, int b);

extern char data_0204c214[];

/* Bring up the sub 2D engine: assign VRAM banks, set DISPCNT-B (mode + BG ext-pal),
 * enable POWCNT1 top/bottom swap (bit15), select graphics mode 3, then hand off. */
void Gfx_SetupSubEngine(void) {
    GX_ResetBankForSubOBJ();
    GX_ResetBankForSubBG();
    GX_SetBankForTex(7);
    GX_SetBankForSubBG(6 << 6);
    GX_SetBankForSubOBJ(8);
    {
        volatile unsigned int *reg_dispcnt_b = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x1000);
        *reg_dispcnt_b = (*reg_dispcnt_b & 0xffcfffef) | 0x00200010;
    }
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(3);
    {
        volatile unsigned short *reg_powcnt1 = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x304);
        *reg_powcnt1 = *reg_powcnt1 | 0x8000;
    }
    NNS_GfdResetFrmTexVramState();
    NNS_GfdInitFrmTexVramManager(3, 1);
    SetGameMode(0);
    *data_0204c214 = 1;
}
