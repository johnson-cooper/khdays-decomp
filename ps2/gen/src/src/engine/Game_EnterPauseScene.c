/* PS2: mechanically prepared copy of src/engine/Game_EnterPauseScene.c (ps2/tools/prep_sources.py). Do not edit. */
#pragma thumb on
/* Game_EnterPauseScene -- enter a scene, MAIN. Unless forced (gPauseAllowed) or flag 0x20ef is set, the
 * call is refused (PauseMenu_SetMode(0)). Otherwise: graphics mode 1/0/1 when +0xe4 asks for it, the
 * scene state becomes 3 (+0xc8, sub-state +0xcc = 2, +0xd8 cleared), a +0xdc scene resets its
 * display (Ov002_HoldPanelScreen(1, 0), TP_RequestAutoSamplingStopAsync, layers 4). Unless a reset is pending
 * (data_0204c240 bit 2 while mode bit 1 is set) the state is saved for the way back: the 64-bit
 * tick (+0x0), the VBlank count (+0x8) and the playback position of stream 0 (+0xc4, the stream is then paused). In mode bit 1
 * the sound fades to 0x40; the main screen shows BG0 only; the fade block (+0xac, 0x18 bytes) is
 * reset with its step at 3 in mode bit 1 else 2; mode bit 3 resets the effect layer; and
 * PauseMenu_Open is queued as the next task. */

#include "nitro/types.h"

#define REG_DISPCNT (*(volatile u32 *)((unsigned int)kh_ds_io + 0x0))

typedef struct GameHeap {
    u64 save;                           /* +0x00 */
    unsigned int vblankCount;           /* +0x08 */
    char pad0c[0xac - 0xc];
    int fade[6];                        /* +0xac */
    int streamPos;                      /* +0xc4: stream 0 position saved on pausing, -1 = none */
    int state;                          /* +0xc8 */
    int subState;                       /* +0xcc */
    int fadeStep;                       /* +0xd0 */
    int fadeTimer;                      /* +0xd4 */
    int stateTimer;                     /* +0xd8 */
    int scene;                          /* +0xdc */
    int object;                         /* +0xe0 */
    int gfxMode;                        /* +0xe4 */
} GameHeap;

extern char *data_0204be08;
extern u8 gPauseAllowed;
extern u8 data_0204c240;
extern char gPauseRefreshName[16];
extern int GameState_IsFlagSet(int id);
extern void PauseMenu_SetMode(int a);
extern void GX_SetGraphicsMode(int a, int b, int c);
extern void Ov002_HoldPanelScreen(int nHold, int bLeaving);
extern void TP_RequestAutoSamplingStopAsync(void);
extern void TP_WaitBusy(int layer);
extern void TP_CheckError(int layer);
extern int LoadGlobalU16At0(void);
extern u64 OS_GetTick(void);
extern unsigned int VBlank_GetCount(void);
extern int SoundStrm_HasPlaybackPos(int slot);
extern int SoundMgr_GetStreamNextPos(int slot);
extern void Table_TailCallWithEntry(int slot, int a);
extern void InvokeSubStructAndStampByte(int volume, int frames);
extern void MI_CpuFill8(void *dest, int data, u32 size);
extern void SetGameMode(int mode);
extern void RegisterNamedTask(int nSlot, const char *pName, void (*pfnTask)(void));
extern void PauseMenu_Open(void);

void Game_EnterPauseScene(void)
{
    GameHeap *heap = (GameHeap *)(&data_0204be08)[1];

    if (gPauseAllowed == 0 && GameState_IsFlagSet(0x20ef) == 0) {
        PauseMenu_SetMode(0);
        return;
    }
    if (heap->gfxMode != 0) {
        GX_SetGraphicsMode(1, 0, 1);
    }
    heap->stateTimer = 0;
    heap->state = 3;
    heap->subState = 2;
    if (heap->scene != 0) {
        Ov002_HoldPanelScreen(1, 0);
        TP_RequestAutoSamplingStopAsync();
        TP_WaitBusy(4);
        TP_CheckError(4);
    }
    if ((data_0204c240 & 4) == 0 || (LoadGlobalU16At0() & 2) == 0) {
        heap->save = OS_GetTick();
        heap->vblankCount = VBlank_GetCount();
        if (SoundStrm_HasPlaybackPos(0) != 0) {
            heap->streamPos = SoundMgr_GetStreamNextPos(0);
            Table_TailCallWithEntry(0, 0);
        }
    }
    if ((LoadGlobalU16At0() & 2) != 0) {
        InvokeSubStructAndStampByte(0x40, 10);
    }
    REG_DISPCNT = (REG_DISPCNT & 0xffffe0ff) | 0x100;
    heap->fadeStep = (LoadGlobalU16At0() & 2) ? 3 : 2;
    heap->fadeTimer = 0;
    MI_CpuFill8(heap->fade, 0, sizeof(heap->fade));
    heap->fade[1] = 1;
    if ((LoadGlobalU16At0() & 8) != 0) {
        SetGameMode(0);
    }
    RegisterNamedTask(1, gPauseRefreshName, PauseMenu_Open);
}
