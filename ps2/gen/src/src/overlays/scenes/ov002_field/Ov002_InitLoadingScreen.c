/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_InitLoadingScreen.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov002_InitLoadingScreen: configure loading graphics and register the blink tick. */

#include "nitro/types.h"

typedef struct Ov002BlinkState {u64 nLastTick;int nPhase,bHoldsPanel;u8 *pTileData;} Ov002BlinkState;
typedef struct Ov002PageChars {u8 pad0[0x10];int nCharSize;u8 *pCharData;} Ov002PageChars;
typedef struct BgPlttSrc {int nFormat,n_pad;u32 dwSize;void *pData;} BgPlttSrc;
typedef struct SpriteResSet {void *pScreen;Ov002PageChars *pChar;BgPlttSrc *pPalette;} SpriteResSet;
typedef int (*Ov002LoadingScreenNextState)(void);
extern Ov002BlinkState *data_ov002_0207fa18;
extern char gOv002UiNldgNldgPath[],gOv002UiSgBgPath[],gOv002UinowldtaskfuncName[];
extern u8 data_0204c240;
extern Ov002BlinkState *NNSi_FndGetCurrentRootHeap(void);
extern void MI_CpuFill8(void *,int,u32);
extern int Ov002_HasAssignedPeerId(void);
extern void Ov002_HoldPanelScreen(int,int);
extern void Ov002_World_SetField4(int);
extern void Ov002_SaveOrRestoreLcdSwap(int);
extern void *Archive_LoadFile(char *,int);
extern void Res_LoadSpriteSet(SpriteResSet *,void *,int,int,int);
extern void DC_FlushRange(void *,u32);
extern void GX_LoadBG3Char(void *,u32,u32);
extern void GX_LoadBGPltt(void *,u32,u32);
extern void NNSi_FndFreeFromDefaultHeap(void *);
extern u16 *G2_GetBG3ScrPtr(void);
extern void *G2_GetBG1ScrPtr(void);
extern void *G2_GetBG3CharPtr(void);
extern void MIi_CpuClearFast(u32,void *,u32);
extern void Ov002_DrawTileCursor2x2(int);
extern u8 *NNSi_FndAllocFromDefaultExpHeap(u32);
extern void MIi_CpuCopyFast(void *,void *,u32);
extern u64 OS_GetTick(void);
extern void RegisterNamedTask(int,char *,void (*)(void));
extern void Ov002_TickBlink(void);
extern void SetMasterBrightnessMain(int);
extern int Ov002_ConstReturn0(void);
Ov002LoadingScreenNextState Ov002_InitLoadingScreen(void)
{
    SpriteResSet resources;
    Ov002BlinkState *pState;
    void *pArchive;
    u16 *pMap;
    *(volatile u16 *)((unsigned int)kh_ds_pal + 0x0)=0;
    *(volatile u16 *)((unsigned int)kh_ds_pal + 0x400)=0;
    pState=NNSi_FndGetCurrentRootHeap();
    data_ov002_0207fa18=pState;
    MI_CpuFill8(pState,0,20);
    *(volatile u32 *)((unsigned int)kh_ds_io + 0x0)=(*(volatile u32 *)((unsigned int)kh_ds_io + 0x0)&~0x1f00)|0x800;
    *(volatile u32 *)((unsigned int)kh_ds_io + 0x1000)&=~0x1f00;
    if(Ov002_HasAssignedPeerId()){
        pState->bHoldsPanel=1;
        Ov002_HoldPanelScreen(1,1);
        Ov002_World_SetField4(1);
    }
    *(volatile u16 *)((unsigned int)kh_ds_io + 0xe)=(*(volatile u16 *)((unsigned int)kh_ds_io + 0xe)&0x43)|0x1f00;
    Ov002_SaveOrRestoreLcdSwap(1);
    pArchive=Archive_LoadFile(gOv002UiNldgNldgPath,14);
    Res_LoadSpriteSet(&resources,pArchive,0,0,0);
    DC_FlushRange(resources.pPalette->pData,0x200);
    GX_LoadBG3Char(resources.pChar->pCharData,0x13c0,0x40);
    GX_LoadBG3Char(resources.pChar->pCharData+0x40,0x17c0,0x40);
    GX_LoadBGPltt(resources.pPalette->pData,0,0x200);
    NNSi_FndFreeFromDefaultHeap(pArchive);
    pMap=G2_GetBG3ScrPtr();
    MIi_CpuClearFast(0,G2_GetBG1ScrPtr(),0x800);
    MIi_CpuClearFast(0,pMap,0x800);
    MIi_CpuClearFast(0,G2_GetBG3CharPtr(),0x20);
    Ov002_DrawTileCursor2x2(0);
    if((data_0204c240&12)==4){
        pMap[0]=0xe086;pMap[1]=0xe087;pMap[32]=0xe088;pMap[33]=0xe089;
        pArchive=Archive_LoadFile(gOv002UiSgBgPath,14);
        Res_LoadSpriteSet(&resources,pArchive,0,0,0);
        pState->pTileData=NNSi_FndAllocFromDefaultExpHeap(resources.pChar->nCharSize);
        MIi_CpuCopyFast(resources.pChar->pCharData,pState->pTileData,resources.pChar->nCharSize);
        NNSi_FndFreeFromDefaultHeap(pArchive);
    }
    *(volatile u32 *)((unsigned int)kh_ds_io + 0x1c)=0;
    pState->nLastTick=OS_GetTick();
    pState->nPhase=0;
    RegisterNamedTask(1,gOv002UinowldtaskfuncName,Ov002_TickBlink);
    SetMasterBrightnessMain(0);
    return Ov002_ConstReturn0;
}
