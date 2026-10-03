/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_missionres_020b458c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 mission resource descriptor data_ov025_020b458c, 0x020b458c-0x020b4598 (.rodata).
 *
 * Opens a mission list (Ov025_InitMissionList 0208a13c) from the resource
 * path, the selector substituted for the ampersand, and the list kind: kind 5 list of the panel page.
 */

typedef struct Ov008MissionResourceDescriptor {
    const char *pResourcePath; /* 0x00: the msl / list resource path, & = the selector */
    int nSelector;             /* 0x04 */
    int nListKind;             /* 0x08 */
} Ov008MissionResourceDescriptor;

extern char gOv025UiCmMslPath_4;

const Ov008MissionResourceDescriptor data_ov025_020b458c __attribute__((aligned(__alignof__(Ov008MissionResourceDescriptor)))) = {
    &gOv025UiCmMslPath_4,  /* pResourcePath */
    0,  /* nSelector */
    5,  /* nListKind */
};
