/* MobiClip: open up to three streams and start everything that drives them.
 *
 * Takes a slot for each stream the request names, opens each one's file,
 * gives the first stream that carries audio a ring of its own, works out how
 * many ticks a frame is worth on each slot, arms every slot's alarm and primes
 * its decoder, and finally installs the vertical-blank presenter.
 *
 * If no slot could be taken, or any file failed to open, everything taken so
 * far is handed back and the caller is told the movie will not play.
 */

#include "nitro/types.h"
#include "platform/kh_platform.h"
#include <stdio.h>

#define TICK_BASE   0x00ffb0ffu
#define FRAME_TRIES 10
#define RING_FRAMES 10

struct MobiClipOpenRequest {
    const char *pszStream0;
    const char *pszStream1;
    const char *pszStream2;
    void *pfnFrameReady;
};

struct MobiClipAudioStream {
    void *pStream;
    short *pLeft;
    short *pRight;
    int pad000c;
    u32 nSampleRate;
    u32 nFrameSamples;
    u32 nChannels;
    int nFilled;
    u32 nBlocks;
};

struct MobiClipFrameTimer {
    void *pStream;
    u8 alarm[0x2c];
    s64 nStartTick;
    u8 nState;
    u8 nFrontBuffer;
    u8 bPresented;
    u8 pad003b[0x40 - 0x3b];
    int nDecoded;
    u32 nConsumed;
    int nPresented;
    u64 nTimeBase;
    void *pfnBufferForIndex;
};

struct MobiClipGlobals {
    u8 bStopped;
    u8 pad0001[3];
    struct MobiClipAudioStream *pAudio;
    struct MobiClipFrameTimer *pMain;
    struct MobiClipFrameTimer *pSub;
};

typedef u8 MobiClipFile[0x48];

struct MobiClipFileBank {
    MobiClipFile aFiles[3];
    u8 pad00d8[0x80e0 - 3 * 0x48];
    void *pfnFrameReady;
};

extern struct MobiClipFileBank data_ov024_02093a48;
extern int data_ov024_0209ba48;
extern struct MobiClipGlobals data_ov024_02093a2c;
extern int gOv024MobiclipIntrName;
extern struct MobiClipFrameTimer *data_ov024_02093a3c[3];

extern void FS_InitFile(void *pFile);
extern void MI_CpuFill8(void *pDest, int nValue, u32 nSize);
extern void DC_StoreAll(void);
extern void *NNS_FndAllocFromDefaultExpHeapEx(u32 nSize, int nAlignment);
extern void NNSi_FndFreeFromDefaultHeap(void *pBlock);
extern void OS_CreateAlarm(void *pAlarm);
extern void OS_WaitVBlankIntr(void);
extern struct MobiClipFrameTimer *Ov024_MobiClip_OpenStreamSlot(int nSlot, const char *pszPath);
extern void Ov024_MobiClip_ReserveTcmArenas(void);
extern void *Ov024_MobiClip_MakeStream(void *pFile, int nTries);
extern u32 Ov024_GetBlockSizeIfSet(void *pStream);
extern u32 Ov024_ReleaseIfSet(void *pStream);
extern u32 Ov024_ReleaseIfSet_2(void *pStream);
extern void Ov024_MobiClip_DecodeAudioEntryChecked(void *pStream);
extern void Ov024_AdvanceNodeAnim(struct MobiClipFrameTimer *pTimer);
extern void Ov024_MobiClip_FlushAudioRing(struct MobiClipAudioStream *pAudio);
extern void Ov024_MobiClip_FlushStagedBlock(void);
extern void Ov024_GetFrameBuffer(void);
extern void Ov024_GetSubScreenBuffer(void);
extern void Ov024_MobiClip_PresentOnVBlank(void);
extern void OS_InitAlarm(void);
extern void RegisterNamedTask(int nSlot, void *pTable, void *pfn);

#if KH_PS2_DEBUG
extern int ps2_gs_crash_screen(const char *title, const char *const *lines, int count);
extern volatile const char *kh_watchdog_mark;

static void ps2_mobiclip_probe(const char *stage, int slot, int frame)
{
    static char mark[128];
    static char detail[128];
    const char *lines[2];

    snprintf(mark, sizeof mark, "MobiClip: %s slot=%d frame=%d", stage, slot, frame);
    kh_watchdog_mark = mark;
    snprintf(detail, sizeof detail, "%s   slot %d   frame %d", stage, slot, frame);
    lines[0] = detail;
    lines[1] = "If this screen remains, the next MobiClip call did not return.";
    ps2_gs_crash_screen("KH Days PS2 - MobiClip probe", lines, 2);
}
#define PROBE(stage, slot, frame) ps2_mobiclip_probe((stage), (slot), (frame))
#else
#define PROBE(stage, slot, frame) ((void)0)
#endif

int Ov024_MobiClip_OpenStreams(struct MobiClipOpenRequest *pRequest)
{
    struct MobiClipFrameTimer *apSlots[3];
    struct MobiClipAudioStream *pAudio;
    struct MobiClipFileBank *pBank;
    u64 nTicks;
    u64 nUnit;
    int i;
    int j;

    PROBE("entered OpenStreams", -1, -1);
    pBank = &data_ov024_02093a48;
    ((int *)&data_ov024_0209ba48)[0x39] = 0;
    for (i = 0; i < 3; i++) {
        FS_InitFile(pBank->aFiles[i]);
        apSlots[i] = 0;
    }

    if (pRequest->pszStream0 != 0) {
        PROBE("open stream slot", 0, -1);
        apSlots[0] = Ov024_MobiClip_OpenStreamSlot(0, pRequest->pszStream0);
        data_ov024_02093a2c.pMain = apSlots[0];
    }
    if (pRequest->pszStream1 != 0) {
        PROBE("open stream slot", 1, -1);
        apSlots[1] = Ov024_MobiClip_OpenStreamSlot(1, pRequest->pszStream1);
        data_ov024_02093a2c.pSub = apSlots[1];
    }
    if (pRequest->pszStream2 != 0) {
        PROBE("open stream slot", 2, -1);
        apSlots[2] = Ov024_MobiClip_OpenStreamSlot(2, pRequest->pszStream2);
    }
    if (apSlots[0] == 0 && apSlots[1] == 0 && apSlots[2] == 0) {
        return 0;
    }

    PROBE("reserve TCM arenas", -1, -1);
    Ov024_MobiClip_ReserveTcmArenas();
    if (apSlots[0] != 0) {
        apSlots[0]->pfnBufferForIndex = (void *)&Ov024_GetFrameBuffer;
    }
    if (apSlots[1] != 0) {
        apSlots[1]->pfnBufferForIndex = (void *)&Ov024_GetSubScreenBuffer;
    }
    if (apSlots[2] != 0) {
        apSlots[2]->pfnBufferForIndex = (void *)&Ov024_GetSubScreenBuffer;
    }
    pBank->pfnFrameReady = pRequest->pfnFrameReady;

    for (i = 0; i < 3; i++) {
        if (apSlots[i] != 0) {
            PROBE("make stream / parse container", i, -1);
            apSlots[i]->pStream =
                Ov024_MobiClip_MakeStream(pBank->aFiles[i], FRAME_TRIES);
            if (apSlots[i]->pStream == 0) {
                goto failed;
            }
        }
    }

    data_ov024_02093a2c.pAudio = 0;
    for (i = 0; i < 2; i++) {
        u32 nChannels;

        if (apSlots[i] != 0) {
            PROBE("read audio metadata", i, -1);
        }
        if (apSlots[i] != 0
            && (nChannels = Ov024_GetBlockSizeIfSet(apSlots[i]->pStream)) != 0) {
            data_ov024_02093a2c.pAudio =
                (struct MobiClipAudioStream *)
                NNS_FndAllocFromDefaultExpHeapEx(0x24, 0x20);
            MI_CpuFill8(data_ov024_02093a2c.pAudio, 0, 0x24);
            data_ov024_02093a2c.pAudio->pStream = apSlots[i]->pStream;
            data_ov024_02093a2c.pAudio->nChannels = nChannels;
            break;
        }
    }

    pAudio = data_ov024_02093a2c.pAudio;
    if (pAudio != 0) {
        u64 nRate;
        PROBE("allocate audio ring", -1, -1);

        pAudio->nSampleRate = Ov024_ReleaseIfSet(pAudio->pStream);
        nRate = TICK_BASE / (u64)(TICK_BASE / (u64)pAudio->nSampleRate);
        pAudio->nFrameSamples =
            (u32)((nRate << 24)
                  / ((u64)pAudio->nChannels * Ov024_ReleaseIfSet_2(pAudio->pStream)))
            + 1;
        pAudio->nBlocks = pAudio->nFrameSamples * RING_FRAMES;
        pAudio->pLeft = (short *)NNS_FndAllocFromDefaultExpHeapEx(
            (pAudio->nBlocks * pAudio->nChannels) << 1, 0x20);
        pAudio->pRight = (short *)NNS_FndAllocFromDefaultExpHeapEx(
            (pAudio->nBlocks * pAudio->nChannels) << 1, 0x20);
        MI_CpuFill8(pAudio->pLeft, 0, (pAudio->nBlocks * pAudio->nChannels) << 1);
        MI_CpuFill8(pAudio->pRight, 0, (pAudio->nBlocks * pAudio->nChannels) << 1);
        DC_StoreAll();
    }

    for (i = 0; i < 3; i++) {
        if (apSlots[i] == 0) {
            continue;
        }
        PROBE("configure stream timing", i, -1);
        if (pAudio != 0 && pAudio->pStream == apSlots[i]->pStream) {
            nUnit = TICK_BASE / (u64)pAudio->nSampleRate;
            nTicks = (u64)Ov024_ReleaseIfSet_2(pAudio->pStream) * TICK_BASE
                     / (nUnit * pAudio->nSampleRate);
            apSlots[i]->nTimeBase = nTicks;
            pAudio->nFilled = 0;
        } else {
            apSlots[i]->nTimeBase = Ov024_ReleaseIfSet_2(apSlots[i]->pStream);
        }
        apSlots[i]->nFrontBuffer = 0;
        apSlots[i]->bPresented = 1;
        OS_InitAlarm();
        OS_CreateAlarm(apSlots[i]->alarm);
        apSlots[i]->nConsumed = 0;
        apSlots[i]->nDecoded = 0;
        apSlots[i]->nPresented = 0;
        apSlots[i]->nState = 0;
        for (j = 0; j < RING_FRAMES; j++) {
            PROBE("prime StepFrame/read", i, j);
            Ov024_MobiClip_DecodeAudioEntryChecked(apSlots[i]->pStream);
            PROBE("prime portable frame decode", i, j);
            Ov024_AdvanceNodeAnim(apSlots[i]);
            if (pAudio != 0 && pAudio->pStream == apSlots[i]->pStream) {
                PROBE("prime audio decode/flush", i, j);
                Ov024_MobiClip_FlushAudioRing(pAudio);
            }
        }
    }

    data_ov024_02093a2c.bStopped = 0;
    PROBE("register movie VBlank presenter", -1, -1);
    RegisterNamedTask(1, &gOv024MobiclipIntrName, (void *)&Ov024_MobiClip_PresentOnVBlank);
    PROBE("first movie VBlank", -1, -1);
    OS_WaitVBlankIntr();
    PROBE("OpenStreams complete", -1, -1);
    return 1;

failed:
    PROBE("OpenStreams failed cleanup", -1, -1);
    Ov024_MobiClip_FlushStagedBlock();
    for (i = 0; i < 3; i++) {
        if (apSlots[i] != 0) {
            NNSi_FndFreeFromDefaultHeap(apSlots[i]);
            data_ov024_02093a3c[i] = 0;
        }
    }
    return 0;
}
