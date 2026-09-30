/* PS2: mechanically prepared copy of src/overlays/scenes/ov011/Ov011_CreateScene.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov011_CreateScene -- Ov011_CreateScene: bring up the movie scene and hand back
 * its first state.
 *
 * Allocates the 0x2cf84-byte scene from the root heap and publishes it in the overlay's
 * globals, records the lid/hinge bit from the ARM7 extended-key mirror at 0x027fffa8 into
 * bit 2 of the scene flags, pulls in ov024 (the MobiClip player), clears the scene, stores
 * the constructor argument, seeds both panes' scroll step to -16, arms handler 0x20e9 and
 * opens "UI/sf/m.p2".  Then it fills a stack frame with the resource, the "is there an
 * argument" flag and the movie name "843.z", lets Ov024_MobiClip_InstallStreamSourceVtbl populate the stream
 * interface at +0x2cf58, and drives it: initialize, open, start.  Screen brightness is set
 * to 0 on both engines before returning the next state.
 *
 * Levers: the title file defines its scene globals (gOv011Cursor, gpOv011Scene) so
 * the scalar pointer is .bss-relative; the movie name is copied element-wise through
 * a named destination pointer (unsigned source); the stream address is re-read
 * through a volatile view so the argument chain reloads; the enabled flag is stored
 * before that read.
 */

#include "nitro/types.h"
#include "game/engine.h"

typedef u32 FSOverlayID;
typedef void (*Ov011StateFn)(void);

extern u32 OVERLAY_24_ID[1];
#define FS_OVERLAY_ID_ov024 ((FSOverlayID)(u32) & (OVERLAY_24_ID))

typedef struct Ov011StreamOpenParams {
    void *resource;
    int   enabled;
} Ov011StreamOpenParams;

typedef struct Ov011StreamInterface {
    void (*initialize)(void);
    void (*open)(const Ov011StreamOpenParams *params);
    void (*unused08)(void);
    void (*start)(void);
} Ov011StreamInterface;

typedef struct Ov011MovieFrame {
    Ov011StreamOpenParams openParams;
    u8                    path[0x80];
} Ov011MovieFrame;

typedef struct Ov011Pane {
    u8 pad_00000[0x10930];
    s8 nScrollStep;
    u8 pad_10931[2];
    u8 nState;
    u8 pad_10934[0x10940 - 0x10934];
} Ov011Pane;

typedef struct Ov011Scene {
    int       nA;
    int       nMode;
    u8        pad_00008[0x14 - 8];
    Ov011Pane aPane[2];
    u8        pad_21294[0x23ac0 - 0x21294];
    u32       reserved0 : 2;
    u32       bLidClosed : 1;
    u32       reserved3 : 29;
    int       nArg;
    u8        pad_23ac8[0x2cf54 - 0x23ac8];
    void     *pResource;
    Ov011StreamInterface stream;
    u8        pad_2cf68[0x2cf84 - 0x2cf68];
} Ov011Scene;

/* khdays: shared-bss */
int data_ov011_0205e960;             /* gOv011Cursor */
Ov011Scene *data_ov011_0205e964;     /* gpOv011Scene */
extern const char gOv011UiSfMPath[];
extern const u8 gOv011843ZName[];

extern Ov011Scene *NNSi_FndGetCurrentRootHeap(void);
extern void MI_CpuFill8(void *destination, int value, u32 size);
extern void *Msg_OpenContainerAndReadHeader(const void *descriptor, int mode);
extern void Ov024_MobiClip_InstallStreamSourceVtbl(Ov011StreamInterface *stream);
extern void Ov011_TickMoviePlaybackState(void);

Ov011StateFn Ov011_CreateScene(int nArg)
{
    Ov011MovieFrame frame;
    u8 *pDst;
    Ov011StreamInterface *pStream;
    int bEnabled;

    data_ov011_0205e964 = NNSi_FndGetCurrentRootHeap();
    data_ov011_0205e964->bLidClosed =
        ((*(volatile u16 *)((unsigned int)kh_ds_hiram + 0x1ffa8) & 0x8000) >> 15) != 0;
    LoadOverlaySync(0, FS_OVERLAY_ID_ov024);
    MI_CpuFill8(data_ov011_0205e964, 0, sizeof(Ov011Scene));
    data_ov011_0205e964->nArg = nArg;
    data_ov011_0205e964->aPane[0].nState = 0;
    data_ov011_0205e964->aPane[1].nState = 0;
    data_ov011_0205e964->aPane[0].nScrollStep = -0x10;
    data_ov011_0205e964->aPane[1].nScrollStep = -0x10;
    data_ov011_0205e964->nMode = 0;
    GameState_SetFlag(0x20e9);
    data_ov011_0205e964->pResource = Msg_OpenContainerAndReadHeader(gOv011UiSfMPath, 0xf);
    frame.openParams.resource = data_ov011_0205e964->pResource;
    bEnabled = data_ov011_0205e964->nArg != 0;
    frame.openParams.enabled = bEnabled;
    pStream = &(*(Ov011Scene *volatile *)&data_ov011_0205e964)->stream;
    pDst = frame.path;
    pDst[0] = gOv011843ZName[0];
    pDst[1] = gOv011843ZName[1];
    pDst[2] = gOv011843ZName[2];
    pDst[3] = gOv011843ZName[3];
    pDst[4] = gOv011843ZName[4];
    pDst[5] = gOv011843ZName[5];
    Ov024_MobiClip_InstallStreamSourceVtbl(pStream);
    data_ov011_0205e964->stream.initialize();
    data_ov011_0205e964->stream.open(&frame.openParams);
    data_ov011_0205e964->stream.start();
    SetMasterBrightnessMain(0);
    SetMasterBrightnessSub(0);
    return (Ov011StateFn)Ov011_TickMoviePlaybackState;
}
