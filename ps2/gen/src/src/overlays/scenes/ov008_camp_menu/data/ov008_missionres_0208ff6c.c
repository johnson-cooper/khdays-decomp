/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_missionres_0208ff6c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 mission resource descriptor data_ov008_0208ff6c, 0x0208ff6c-0x0208ff78 (.rodata).
 *
 * Opens a mission list (0205652c) from the msl resource path, the selector
 * substituted for the ampersand, and the list kind: mission list kind 5 for the reward scan (02089e94).
 */

typedef struct Ov008MissionResourceDescriptor {
    const char *pResourcePath; /* 0x00: "UI/cm/msl_&.msi.z", & = the selector */
    int nSelector;             /* 0x04 */
    int nListKind;             /* 0x08 */
} Ov008MissionResourceDescriptor;

extern char gOv008UiCmMslPath_4;

const Ov008MissionResourceDescriptor data_ov008_0208ff6c __attribute__((aligned(__alignof__(Ov008MissionResourceDescriptor)))) = {
    &gOv008UiCmMslPath_4,  /* pResourcePath */
    0,  /* nSelector */
    5,  /* nListKind */
};
