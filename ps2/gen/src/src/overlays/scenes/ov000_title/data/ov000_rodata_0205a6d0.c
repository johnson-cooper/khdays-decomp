/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/data/ov000_rodata_0205a6d0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov000 .rodata 0x0205a6d0-0x0205a6f4: the save-screen layout template and its resource tracker config. */

typedef void (*TrackerCallback)(void);

/* Allocation config of a resource tracker: three pool capacities and the two
 * callbacks the tracker runs on its entries and nodes. */
typedef struct ResourceTrackerConfig {
    unsigned int nEntryCapacity;     /* 0x00 */
    unsigned int nNodeCapacity;      /* 0x04 */
    unsigned int nAuxCapacity;       /* 0x08 */
    TrackerCallback pfnEntry;        /* 0x0c */
    TrackerCallback pfnNode;         /* 0x10 */
} ResourceTrackerConfig;

/* Layout template copied by Ov000_LayoutSelectionPages: the save-screen object layout
 * ("UI/cm/sav_o_000.pobj.z"), mode 2. */
typedef struct LayoutTemplate {
    const char *pszLayout;  /* 0x00 */
    int nMode;              /* 0x04 */
    int aParam[2];          /* 0x08 */
} LayoutTemplate;

extern char gOv000UiCmSavO000Path;  /* "UI/cm/sav_o_000.pobj.z" */

const LayoutTemplate data_ov000_0205a6d0 __attribute__((aligned(__alignof__(LayoutTemplate)))) = { &gOv000UiCmSavO000Path, 2, { 0, 0 } };

extern void Ov000_ResourceEntryCallback(void);
extern void Ov000_ResourceNodeCallback(void);

/* Read by Ov000_LoadPageSubScreenLayer. */
const ResourceTrackerConfig data_ov000_0205a6e0 __attribute__((aligned(__alignof__(ResourceTrackerConfig)))) = { 256, 8, 12, Ov000_ResourceEntryCallback, Ov000_ResourceNodeCallback };
