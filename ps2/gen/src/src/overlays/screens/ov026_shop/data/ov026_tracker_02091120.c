/* PS2: mechanically prepared copy of src/overlays/screens/ov026_shop/data/ov026_tracker_02091120.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov026 .rodata 0x02091120-0x02091134: surface/resource tracker config. */

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

extern void Ov026_ResourceEntryCallback(void);
extern void Ov026_ResourceNodeCallback(void);

/* Read by Ov026_LoadShopResources. */
const ResourceTrackerConfig data_ov026_02091120 __attribute__((aligned(__alignof__(ResourceTrackerConfig)))) = { 105, 1, 28, Ov026_ResourceEntryCallback, Ov026_ResourceNodeCallback };
