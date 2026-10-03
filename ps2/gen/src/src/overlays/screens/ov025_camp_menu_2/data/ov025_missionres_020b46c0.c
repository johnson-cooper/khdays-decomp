/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_missionres_020b46c0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 mission resource descriptor data_ov025_020b46c0, 0x020b46c0-0x020b46cc (.rodata).
 *
 * Opens a mission list (Ov025_InitMissionList 0208a13c) from the resource
 * path, the selector substituted for the ampersand, and the list kind: kind 6 list of the mission menu (020ab7b0 family).
 */

typedef struct Ov008MissionResourceDescriptor {
    const char *pResourcePath; /* 0x00: the msl / list resource path, & = the selector */
    int nSelector;             /* 0x04 */
    int nListKind;             /* 0x08 */
} Ov008MissionResourceDescriptor;

extern char gOv025UiCmMslPath_5;

const Ov008MissionResourceDescriptor data_ov025_020b46c0 __attribute__((aligned(__alignof__(Ov008MissionResourceDescriptor)))) = {
    &gOv025UiCmMslPath_5,  /* pResourcePath */
    0,  /* nSelector */
    6,  /* nListKind */
};
