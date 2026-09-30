/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_MissionSceneCtor.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov008_MissionSceneCtor -- scene constructor.
 *
 * Allocates the char-select manager context (0x97f8 bytes) and stores it in the scene-object
 * slot data_ov008_02090fa4, brings up the 2D display (POWCNT1, VRAM banks BG=3/subBG=4/
 * OBJ=0x70/subOBJ=8, graphics mode), programs the BG priority registers (main BG0..3 =
 * 1/3/2/0, sub BG1..3 = 2/1/0), allocates eight 0x600-byte cell buffers, loads the font into
 * both BG3 and BG7 (data_..540 and data_..55c are BOTH "/text/font_eu_10all.NFTR") and the
 * char-select UI pack (data_..578 = "UI/mlt/res.p2", data_..588 = "UI/mlt/res_&.p2", the
 * localised variant, via Msg_OpenContainerAndReadHeader), seeds the animation state, sets the display mode
 * bits, and boots the sub-state (id 0xd when arg!=0) before returning the next scene-state
 * function (Ov008_UpdateMissionModeFrame).
 *
 * Resource paths confirmed by reading RAM at runtime.
 *
 * PROVENANCE: byte-identical twin of Ov006_MissionSceneCtor -- same code, this overlay's own
 * globals, propagated mechanically and verified byte-exact.
 * The scene-identity phrasing that came with the twin source (Mission Mode / char select /
 * ov025 panel) is the REP's, NOT established for ov008, so it was removed rather than
 * carried over. What IS measured: ov008's own strings include UI/mlt/res.p2 (the same pack
 * ov006 loads) plus UI/cm/*.p2 and ba/ch/*, so resource detail naming those is sound; the
 * scene label is not. The offsets and logic below are this function's -- the code is
 * byte-identical to the rep.
 */

#include "nitro/types.h"

struct S5 { int w[5]; };

extern int *data_ov008_02090fa4;
extern int gOv008TextFontEu10AllPath, gOv008TextFontEu10AllPath_2;
extern int gOv008UiMltResPath_2, gOv008UiMltResPath_3;
extern struct S5 data_ov008_0208fd58;

extern void *NNSi_FndGetCurrentRootHeap(void);
extern void MI_CpuFill8(void *dst, int data, unsigned int size);
extern void Gfx_Reset2DEngines(void);
extern void SetGameMode(int a);
extern void GX_SetBankForBG(int a);
extern void GX_SetBankForSubBG(int a);
extern void GX_SetBankForOBJ(int a);
extern void GX_SetBankForSubOBJ(int a);
extern void GXS_SetGraphicsMode(int a);
extern void GX_SetGraphicsMode(int a, int b, int c);
extern int NNS_FndAllocFromDefaultExpHeapEx(int size, int align);
extern void MIi_CpuClear16(int val, int dst, int size);
extern void Font_LoadUTF16(int dst, int desc);
extern int Msg_OpenContainerAndReadHeader(int path, int mode);
extern void Ov008_Container_Init(int *dst, struct S5 *src);
extern void SetMasterBrightnessMain(int a);
extern void SetMasterBrightnessSub(int a);
extern void Tween_Configure(int *p, int a, int b, int c, int d);
extern void Tween_Clear(int *p);
extern long long OS_GetTick(void);
extern void Ov008_SetBgLayout(int state);
extern void Ov008_UploadTextCells(int state);
extern void Ov008_RebindBgLayers(int state);
extern void Ov008_UpdateMissionModeFrame(void);

#define OBJ ((char *)data_ov008_02090fa4)

void *Ov008_MissionSceneCtor(int arg) {
    unsigned int i;
    int *base;
    long long anim;
    struct S5 param;

    data_ov008_02090fa4 = (int *)NNSi_FndGetCurrentRootHeap();
    MI_CpuFill8(data_ov008_02090fa4, 0, 0x97f8);
    Gfx_Reset2DEngines();
    SetGameMode(0);

    *(vu16 *)((unsigned int)kh_ds_io + 0x304) |= 0x8000;
    *(vu32 *)((unsigned int)kh_ds_io + 0x0) &= ~0x38000000;
    *(vu32 *)((unsigned int)kh_ds_io + 0x0) &= ~0x07000000;
    GX_SetBankForBG(3);
    GX_SetBankForSubBG(4);
    GX_SetBankForOBJ(0x70);
    GX_SetBankForSubOBJ(8);
    GXS_SetGraphicsMode(0);
    GX_SetGraphicsMode(1, 0, 0);

    {
        vu16 *bg1 = (vu16 *)((unsigned int)kh_ds_io + 0xa);
        vu16 *bgs2 = (vu16 *)((unsigned int)kh_ds_io + 0x100c);
        vu16 *bg0 = (vu16 *)((unsigned int)kh_ds_io + 0x8);
        *bg1 = *bg1 & ~3 | 3;
        bg1[1] = bg1[1] & ~3 | 2;
        *bg0 = *bg0 & ~3 | 1;
        bg1[2] = bg1[2] & ~3;
        *(vu16 *)((unsigned int)kh_ds_io + 0x100a) = *(vu16 *)((unsigned int)kh_ds_io + 0x100a) & ~3 | 2;
        *bgs2 = *bgs2 & ~3 | 1;
        bgs2[1] = bgs2[1] & ~3;
    }

    i = 0;
    do {
        *(int *)(OBJ + i * 4 + 0x94cc) = NNS_FndAllocFromDefaultExpHeapEx(0x600, 2);
        MIi_CpuClear16(0, *(int *)(OBJ + i * 4 + 0x94cc), 0x600);
        i = i + 1 & 0xff;
    } while (i < 8);

    Font_LoadUTF16((int)(OBJ + 0x9760), (int)&gOv008TextFontEu10AllPath);
    Font_LoadUTF16((int)(OBJ + 0x97ac), (int)&gOv008TextFontEu10AllPath_2);
    *(int *)OBJ = Msg_OpenContainerAndReadHeader((int)&gOv008UiMltResPath_2, 0xe);
    *(int *)(OBJ + 4) = Msg_OpenContainerAndReadHeader((int)&gOv008UiMltResPath_3, 0xe);

    param = data_ov008_0208fd58;
    Ov008_Container_Init((int *)(OBJ + 8), &param);
    SetMasterBrightnessMain(-0x10);
    SetMasterBrightnessSub(-0x10);
    Tween_Configure((int *)(OBJ + 0x9544), 0, -0x10000, -0x10000, 0);
    Tween_Clear((int *)(OBJ + 0x9574));
    Tween_Clear((int *)(OBJ + 0x9590));

    *(vu32 *)((unsigned int)kh_ds_io + 0x1000) = *(vu32 *)((unsigned int)kh_ds_io + 0x1000) & ~0x1f00 | 0x1e00;
    *(vu32 *)((unsigned int)kh_ds_io + 0x0) = *(vu32 *)((unsigned int)kh_ds_io + 0x0) & ~0x1f00 | 0x1f00;

    base = data_ov008_02090fa4;
    anim = OS_GetTick();
    *(long long *)((char *)base + 0x94c4) = anim;
    *(signed char *)((char *)base + 0x950c) = -1;
    if (arg != 0) {
        *(int *)(OBJ + 0x94f4) = 0xd;
    }
    Ov008_SetBgLayout(*(int *)(OBJ + 0x94f4));
    Ov008_UploadTextCells(*(int *)(OBJ + 0x94f4));
    Ov008_RebindBgLayers(*(int *)(OBJ + 0x94f4));
    return (void *)Ov008_UpdateMissionModeFrame;
}
