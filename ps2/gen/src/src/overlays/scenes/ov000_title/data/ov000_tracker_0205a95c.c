/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/data/ov000_tracker_0205a95c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov000 .rodata 0x0205a95c-0x0205a970: resource tracker config. */

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

extern void Ov000_BlitTileRequest(void);
extern void Ov000_TrackerReleaseNoOp(void);

/* Read by Ov000_SetupMenuObjects. */
const ResourceTrackerConfig data_ov000_0205a95c __attribute__((aligned(__alignof__(ResourceTrackerConfig)))) = { 4, 1, 4, Ov000_BlitTileRequest, Ov000_TrackerReleaseNoOp };
