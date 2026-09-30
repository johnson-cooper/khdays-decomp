/* Subsystems not available in the current PS2 build, cleanly stubbed so the rest of the game runs.
 *
 *  MobiClip video (ov024)  TEMPORARY SKIP while the decoder is brought up (docs/PS2_PORT.md 3.20):
 *                          a movie "opens", is immediately finished and reports every frame as
 *                          buffered, so scenes that play movies move on.  Cutscenes are not removed:
 *                          the scene logic around them runs unchanged.
 *  DS wireless (ov105)     UNAVAILABLE on PS2 for now: every WM call fails with WM_ERRCODE_WM_DISABLE,
 *                          which the game treats as "wireless is off"; single player is unaffected.
 */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

/* ------------------------------------------------------------ MobiClip */

#define MOVIE_SKIP() KH_UNIMPLEMENTED_ONCE("MobiClip movie playback (movie skipped)")

int Ov024_MobiClip_OpenStreams(void *request) { (void)request; MOVIE_SKIP(); KH_INFO("movie", "movie skipped"); return 1; }
int Ov024_TickStreamSlots(void) { return 1; }
int Ov024_MobiClip_BufferedFrameCount(void) { return 0x7fffffff; }
void Ov024_MobiClip_KickPlayerSlot(int slot) { (void)slot; }
void Ov024_MobiClip_StopPlayback(void) { }
void Ov024_MobiClip_InstallStreamSourceVtbl(void *v) { (void)v; }
int Ov024_RunDisplayTeardownSteps(void) { return 1; }
int func_ov024_02083358(void) { return 0; }

/* -------------------------------------------------------------- wireless */

#define WM_ERRCODE_WM_DISABLE 4
#define WM_OFF() do { KH_UNIMPLEMENTED_ONCE("DS wireless (unavailable on PS2)"); return WM_ERRCODE_WM_DISABLE; } while (0)

int Ov105_WmInitCore(void) { WM_OFF(); }
int Ov105_WMi_InitializeEx(void) { WM_OFF(); }
int Ov105_WMi_CheckStateEx(void) { WM_OFF(); }
int Ov105_WMi_SendCommand(void) { WM_OFF(); }
int Ov105_WMi_StartParentEx(void) { WM_OFF(); }
int Ov105_WM_Disconnect(void) { WM_OFF(); }
int Ov105_WM_EndMP(void) { WM_OFF(); }
int Ov105_WM_MeasureChannel(void) { WM_OFF(); }
int Ov105_WM_SetEntry(void) { WM_OFF(); }
int Ov105_WM_SetMPDataToPortEx(void) { WM_OFF(); }
int Ov105_WM_SetParentParameter(void) { WM_OFF(); }
int Ov105_WM_SetWEPKey(void) { WM_OFF(); }
int Ov105_WM_StartConnectEx(void) { WM_OFF(); }
int Ov105_WM_StartMP(void) { WM_OFF(); }
int Ov105_WM_StartScan(void) { WM_OFF(); }
int Ov105_WM_GetDispersionBeaconPeriod(void) { return 200; }
int Ov105_WM_GetDispersionScanPeriod(void) { return 30; }
int Ov105_WM_GetLinkLevel(void) { return 0; }
int Ov105_WM_GetNextTgid(void) { return 1; }
