/* PS2: mechanically prepared copy of libs/nns/snd/calls/SetupStreamFunction.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/pxi.h"
#include "nnsys/snd.h"

void * NNS_SndArcGetFileAddress(u32 fileId);
extern BOOL OpenFileStream(NNSSndStrmPlayer * player, u32 fileId);
extern void CloseFileStream(NNSSndStrmPlayer * player);
extern s32 ReadFileStream(NNSSndStrmPlayer * player, void * dest, u32 size, u32 offset);
extern void CancelFileStream(NNSSndStrmPlayer * player);
extern BOOL OpenMemoryStream(NNSSndStrmPlayer * player, u32 fileId);
extern void CloseMemoryStream(NNSSndStrmPlayer * player);
extern s32 ReadMemoryStream(NNSSndStrmPlayer * player, void * dest, u32 size, u32 offset);
extern void CancelMemoryStream(NNSSndStrmPlayer * player);
extern BOOL OpenFileStream (NNSSndStrmPlayer * player, u32 fileId);
extern void CloseFileStream (NNSSndStrmPlayer * player);
extern s32 ReadFileStream (NNSSndStrmPlayer * player, void * dest, u32 size, u32 offset);
extern void CancelFileStream (NNSSndStrmPlayer * player);
extern BOOL OpenMemoryStream (NNSSndStrmPlayer * player, u32 fileId);
extern void CloseMemoryStream (NNSSndStrmPlayer * player);
extern s32 ReadMemoryStream (NNSSndStrmPlayer * player, void * dest, u32 size, u32 offset);
extern void CancelMemoryStream (NNSSndStrmPlayer * player);

/* khdays: shared-bss */
NNSSndStrmThread * sPrepareThread;   /* sPrepareThread */
BOOL data_0204ad8c;   /* initialized$3434 */
u8 * sDecodeBuffer;   /* sDecodeBuffer */

/* SetupStreamFunction -- NitroSystem sndarc_stream.c: SetupStreamFunction. */
void SetupStreamFunction (NNSSndStrmPlayer * player, u32 fileId)
{
    if (NNS_SndArcGetFileAddress(fileId) == NULL) {
        player->openStreamFunc = OpenFileStream;
        player->closeStreamFunc = CloseFileStream;
        player->readStreamFunc = ReadFileStream;
        player->cancelStreamFunc = CancelFileStream;
    } else {
        player->openStreamFunc = OpenMemoryStream;
        player->closeStreamFunc = CloseMemoryStream;
        player->readStreamFunc = ReadMemoryStream;
        player->cancelStreamFunc = CancelMemoryStream;
    }

}
