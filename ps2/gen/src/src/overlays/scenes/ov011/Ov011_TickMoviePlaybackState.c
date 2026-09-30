/* PS2: mechanically prepared copy of src/overlays/scenes/ov011/Ov011_TickMoviePlaybackState.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov011_TickMoviePlaybackState -- Ov011 initial state: wait for the intro movie, then bring up
 * the title. Polls the MobiClip stream (stream.poll, interface +0x14) every frame and
 * returns 0 to stay until it reports finished. On finish it stops the stream (+0x8),
 * fades both engines out, frees the movie resource, dispatches handler 0x20e9 and stores
 * the ARM7 thread/lid bit into scene-flags bit0. If an argument was passed and that bit
 * is set it kicks Table_TailCallWithEntry(0,0x1e) and hands back state Ov011_InitGlobalStateAndGetHandler;
 * otherwise it opens the title container, builds two message handles, brings up both
 * panes, sets game mode 0, blends both BLDCNT registers down, fades back in and hands
 * back state Ov011_TickTitleMenu, clearing the busy bit first.
 *
 * Scene flags at +0x23ac0 are a bitfield: bit0 threadAvail, bit1 busy, bit2 lidClosed.
 * The signed 1-bit field is what makes bit0 read back with lsl#31/asr#31.
 */

#include "nitro/types.h"

typedef void (*Ov011StateFn)(void);

typedef struct Ov011StreamInterface {
    void (*initialize)(void);
    void (*open)(const void *params);
    void (*close)(void);
    void (*start)(void);
    void (*unk10)(void);
    int  (*poll)(void);
} Ov011StreamInterface;

typedef struct Ov011SceneFlags {
    int bThreadAvail : 1;
    int bBusy        : 1;
    int bLidClosed   : 1;
    int reserved     : 29;
} Ov011SceneFlags;

typedef struct Ov011Scene {
    u8    pad_0000[0xc];
    void *pMsgResource;
    u8    pad_0010[0x23a9c - 0x10];
    int   handle1;
    u8    pad_23aa0[0x23aa8 - 0x23aa0];
    int   handle2;
    u8    pad_23aac[0x23ac0 - 0x23aac];
    Ov011SceneFlags flags;
    int   nArg;
    u8    pad_23ac8[0x2cf54 - 0x23ac8];
    void *pResource;
    Ov011StreamInterface stream;
    u8    pad_2cf70[0x2cf84 - 0x2cf70];
} Ov011Scene;

typedef struct Ov011Globals {
    int         nCursor;
    Ov011Scene *pScene;
} Ov011Globals;

extern Ov011Globals data_ov011_0205e960;
extern const u8 gOv011UiSfSfPath[];
extern const u8 gOv011UiSfSffont10FontPath[];
extern const u8 gOv011UiSfSffont8FontPath[];

extern void SetMasterBrightnessMain(int brightness);
extern void SetMasterBrightnessSub(int brightness);
extern void ZeroHalfThenFree(void *resource);
extern void GameState_ClearFlag(int handlerId);
extern int  func_ov024_02083358(void);
extern void Table_TailCallWithEntry(int a, int b);
extern void *Msg_OpenContainerAndReadHeader(const void *descriptor, int mode);
extern int  Loader_RequestFile(const void *data, int id);
extern void Ov011_InitTitleDisplay(void);
extern long long Ov011_SetupTitleBackgrounds(void);
extern void Loader_SleepIfBusy(long long value);
extern void Ov011_CreateTitleObjects(void);
extern long long Ov011_SetupTitleTileSurfaces(void);
extern void Ov011_LoadEntryTable(long long value);
extern void SetGameMode(int mode);
extern void G2x_SetBlendBrightness_(u16 *reg, int a, int b);
extern void Ov011_SelectMenuTableA(void);
extern void Ov011_InitGlobalStateAndGetHandler(void);
extern void Ov011_TickTitleMenu(void);

Ov011StateFn Ov011_TickMoviePlaybackState(void)
{
    if (data_ov011_0205e960.pScene->stream.poll() == 0) {
        return 0;
    }
    data_ov011_0205e960.pScene->stream.close();
    SetMasterBrightnessMain(-0x10);
    SetMasterBrightnessSub(-0x10);
    ZeroHalfThenFree(data_ov011_0205e960.pScene->pResource);
    GameState_ClearFlag(0x20e9);
    data_ov011_0205e960.pScene->flags.bThreadAvail = func_ov024_02083358();
    if (data_ov011_0205e960.pScene->nArg != 0 &&
        data_ov011_0205e960.pScene->flags.bThreadAvail) {
        Table_TailCallWithEntry(0, 0x1e);
        return (Ov011StateFn)Ov011_InitGlobalStateAndGetHandler;
    }
    data_ov011_0205e960.pScene->pMsgResource = Msg_OpenContainerAndReadHeader(gOv011UiSfSfPath, 0xe);
    data_ov011_0205e960.pScene->handle1 = Loader_RequestFile(gOv011UiSfSffont10FontPath, 0xe);
    data_ov011_0205e960.pScene->handle2 = Loader_RequestFile(gOv011UiSfSffont8FontPath, 0xe);
    Ov011_InitTitleDisplay();
    Loader_SleepIfBusy(Ov011_SetupTitleBackgrounds());
    Ov011_CreateTitleObjects();
    Ov011_LoadEntryTable(Ov011_SetupTitleTileSurfaces());
    SetGameMode(0);
    G2x_SetBlendBrightness_((u16 *)((unsigned int)kh_ds_io + 0x50), 4, -0x10);
    G2x_SetBlendBrightness_((u16 *)((unsigned int)kh_ds_io + 0x1050), 4, -0x10);
    SetMasterBrightnessMain(0);
    SetMasterBrightnessSub(0);
    Ov011_SelectMenuTableA();
    data_ov011_0205e960.pScene->flags.bBusy = 0;
    return (Ov011StateFn)Ov011_TickTitleMenu;
}
