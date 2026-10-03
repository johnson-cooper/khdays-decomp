/* PS2: mechanically prepared copy of src/overlays/scenes/ov009_camp_save/data/ov009_rodata_02055f54.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov009 .rodata 0x02055f54-0x02055f6c: menu header limits and the menu object tracker config (Ov009_Menu_Init). */

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

/* The two header limits of the menu (first 12, second 2). */
const struct {
    unsigned short first;
    unsigned short second;
} data_ov009_02055f54 __attribute__((aligned(4))) = { 12, 2 };

extern void Ov009_ResourceEntryCallback(void);
extern void Ov009_ResourceNodeCallback(void);

/* Read by Ov009_Menu_Init. */
const ResourceTrackerConfig data_ov009_02055f58 __attribute__((aligned(__alignof__(ResourceTrackerConfig)))) = { 256, 32, 32, Ov009_ResourceEntryCallback, Ov009_ResourceNodeCallback };
