/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/data/ov002_screens_0207eb2c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov002 screen descriptors, 0x0207eb2c-0x0207eb40.
 *
 * 1 record of the shape Ov002_OpenPanelScreen takes: a class id whose high half
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

extern void Ov002_SetupPartyPanel(void);
extern void Ov002_SceneDestroy(void);

Ov002ScreenDesc data_ov002_0207eb2c __attribute__((aligned(__alignof__(Ov002ScreenDesc)))) = {
    0x0e0033, Ov002_SetupPartyPanel, Ov002_SceneDestroy, 2024, 0,
};
