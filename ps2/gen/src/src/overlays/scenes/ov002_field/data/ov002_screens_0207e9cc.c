/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/data/ov002_screens_0207e9cc.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov002 screen descriptors, 0x0207e9cc-0x0207e9f4.
 *
 * 2 records of the shape Ov002_OpenPanelScreen takes: a class id whose high half
 * selects the group and whose low half is the index, the pair of handlers, the work
 * size the screen needs, and a word that is zero in every record.
 */

typedef void (*Ov002ScreenFn)(void);

typedef struct {
    unsigned int nClassId;
    Ov002ScreenFn pfnOpen;
    Ov002ScreenFn pfnClose;
    int nWorkSize;
    int nReserved;
} Ov002ScreenDesc;

extern void Ov002_InitGaugeScene(void);
extern void Ov002_DestroyBufferSet(void);
extern void Ov002_SetupPanelPage(void);
extern void Ov002_DestroyEntryPool(void);

Ov002ScreenDesc data_ov002_0207e9cc __attribute__((aligned(__alignof__(Ov002ScreenDesc)))) = {
    0x0e0036, Ov002_InitGaugeScene, Ov002_DestroyBufferSet, 392, 0,
};

Ov002ScreenDesc data_ov002_0207e9e0 __attribute__((aligned(__alignof__(Ov002ScreenDesc)))) = {
    0x0e0034, Ov002_SetupPanelPage, Ov002_DestroyEntryPool, 32, 0,
};
