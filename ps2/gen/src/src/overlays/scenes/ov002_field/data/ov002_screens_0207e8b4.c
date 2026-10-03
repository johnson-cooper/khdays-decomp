/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/data/ov002_screens_0207e8b4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov002 screen descriptors, 0x0207e8b4-0x0207e8dc.
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

extern void Ov002_CaptureRootHeap(void);
extern void Ov002_RootScreenCloseNoOp(void);
extern void Ov002_OpenPanelScreen(void);
extern void Ov002_ClosePanelScreen(void);

Ov002ScreenDesc data_ov002_0207e8b4 __attribute__((aligned(__alignof__(Ov002ScreenDesc)))) = {
    0x0e0000, Ov002_CaptureRootHeap, Ov002_RootScreenCloseNoOp, 8, 0,
};

Ov002ScreenDesc data_ov002_0207e8c8 __attribute__((aligned(__alignof__(Ov002ScreenDesc)))) = {
    0x0e0031, Ov002_OpenPanelScreen, Ov002_ClosePanelScreen, 708, 0,
};
