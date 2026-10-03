/* Subsystems not available in the current PS2 build, cleanly stubbed so the rest of the game runs.
 *
 *  DS wireless (ov105)     UNAVAILABLE on PS2 for now: every WM call fails with WM_ERRCODE_WM_DISABLE,
 *                          which the game treats as "wireless is off"; single player is unaffected.
 *
 * (MobiClip movies play through ov024's own player: ps2/overrides/overlays/Ov024_*.c.)
 */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

#include <string.h>

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
