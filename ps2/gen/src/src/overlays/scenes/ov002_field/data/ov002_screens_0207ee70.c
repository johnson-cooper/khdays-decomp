/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/data/ov002_screens_0207ee70.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov002 screen descriptors, 0x0207ee70-0x0207ee98.
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

extern void Ov002_TouchPageCreate(void);
extern void Ov002_TeardownSession(void);
extern void Ov002_MapPageCreate(void);
extern void Ov002_TearDownSessionScene(void);

Ov002ScreenDesc data_ov002_0207ee70 __attribute__((aligned(__alignof__(Ov002ScreenDesc)))) = {
    0x0e0037, Ov002_TouchPageCreate, Ov002_TeardownSession, 44, 0,
};

Ov002ScreenDesc data_ov002_0207ee84 __attribute__((aligned(__alignof__(Ov002ScreenDesc)))) = {
    0x0e0039, Ov002_MapPageCreate, Ov002_TearDownSessionScene, 148, 0,
};
