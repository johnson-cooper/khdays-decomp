/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/data/ov002_help_entries.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov002 help entries, 0x0207e98c-0x0207e9c4.
 *
 * Two seven-word records, each holding four counters, a pointer to its row block and
 * the handler that draws it.
 */

typedef void (*Ov002HelpFn)(void);

typedef struct {
    int nIndex;
    int nId;
    int nRows;
    int nFlags;
    void *pRows;
    int nStep;
    Ov002HelpFn pfnDraw;
} Ov002HelpEntry;

extern int data_ov002_0207dd84;
extern int data_ov002_0207dd8c;
extern void Ov002_DrawGaugeTweenCell(void);
extern void Ov002_DrawLayoutRow(void);

Ov002HelpEntry data_ov002_0207e98c[2] __attribute__((aligned(__alignof__(Ov002HelpEntry)))) = {
    { 1, 77, 6, 0, &data_ov002_0207dd8c, 8, Ov002_DrawGaugeTweenCell },
    { 0, 46, 4, 2, &data_ov002_0207dd84, 2, Ov002_DrawLayoutRow },
};
