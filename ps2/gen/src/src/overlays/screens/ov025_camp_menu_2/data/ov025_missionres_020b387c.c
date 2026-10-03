/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_missionres_020b387c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 mission resource descriptor data_ov025_020b387c, 0x020b387c-0x020b3888 (.rodata).
 *
 * Opens a mission list (Ov025_InitMissionList 0208a13c) from the resource
 * path, the selector substituted for the ampersand, and the list kind: UI/cm/msl_&.msi.z kind 2, the day list of the records hub (0208b7d0, selector = the current day).
 */

typedef struct Ov008MissionResourceDescriptor {
    const char *pResourcePath; /* 0x00: the msl / list resource path, & = the selector */
    int nSelector;             /* 0x04 */
    int nListKind;             /* 0x08 */
} Ov008MissionResourceDescriptor;

extern char gOv025UiCmMslPath;

const Ov008MissionResourceDescriptor data_ov025_020b387c __attribute__((aligned(__alignof__(Ov008MissionResourceDescriptor)))) = {
    &gOv025UiCmMslPath,  /* pResourcePath */
    0,  /* nSelector */
    2,  /* nListKind */
};
