/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_OpenTutorialPage.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov008_OpenTutorialPage -- Ov008_OpenTutorialPage: initialise page B as the
 * tutorial page.  Clears the page, copies the 16-byte cue request into its
 * header, loads the tutorial archive ("UI/btlttr/ttr_&.dat.z", 14 chunks) and
 * hands it to the ov025 list at +0x200 selecting the page's entry from the
 * page-index table; raises bit 2 of the page flags; then programs the sub
 * screen's BG1..BG3 (screen bases 0x18/0x19/0x16 with char base 8 on BG3,
 * priorities 3/2/1), clears the first BG3 character tile and shows only BG1..3
 * on the sub display.
 */

#include "nitro/types.h"

#define PAGE_B_SIZE 0x214

typedef struct Ov008CueRequest {
    int aWord[4];
} Ov008CueRequest;

typedef struct Ov008PageB {
    Ov008CueRequest header;   /* 0x000 */
    u8  pad_010[0x28 - 0x10];
    u32 nFlags;               /* 0x028 */
    u8  pad_02c[0x1fc - 0x2c];
    void *pArchive;           /* 0x1fc */
    u8  list[0x214 - 0x200];  /* 0x200: ov025 list */
} Ov008PageB;

typedef struct DisplayRegisters {
    volatile u32 dispcnt;
    u8 pad04[4];
    volatile u16 bg0cnt;
    volatile u16 bg1cnt;
    volatile u16 bg2cnt;
    volatile u16 bg3cnt;
} DisplayRegisters;

static volatile DisplayRegisters *const SUB_DISPLAY = (volatile DisplayRegisters *)((unsigned int)kh_ds_io + 0x1000);

extern char gOv008UiBtlttrTtrPath[];                                  /* "UI/btlttr/ttr_&.dat.z" */
extern const int data_ov008_02090a54[];                             /* page index -> tutorial entry */

extern Ov008PageB *Ov008_GetPageB(void);                        /* Ov008_GetPageB */
extern void MI_CpuFill8(void *pDst, int nValue, u32 nSize);
extern Ov008CueRequest *Ov008_GetCueRequest(void);                  /* Ov008_GetCueRequest */
extern void *Archive_LoadFile(const char *pPath, int nChunks);         /* Archive_LoadFile */
extern void Ov025_InitBlockCursor(void *pList, void *pArchive);
extern void Ov025_FindChunkById(void *pList, u32 nEntry);
extern void *G2S_GetBG3CharPtr(void);
extern void MIi_CpuClearFast(u32 nValue, void *pDst, u32 nSize);

void Ov008_OpenTutorialPage(void)
{
    Ov008PageB *pPage = Ov008_GetPageB();

    MI_CpuFill8(pPage, 0, PAGE_B_SIZE);
    pPage->header = *Ov008_GetCueRequest();
    pPage->pArchive = Archive_LoadFile(gOv008UiBtlttrTtrPath, 14);
    Ov025_InitBlockCursor(pPage->list, pPage->pArchive);
    Ov025_FindChunkById(pPage->list, (u16)data_ov008_02090a54[pPage->header.aWord[0]]);
    pPage->nFlags |= 4;
    SUB_DISPLAY->bg1cnt = (SUB_DISPLAY->bg1cnt & 0x43) | 0x1800;
    SUB_DISPLAY->bg2cnt = (SUB_DISPLAY->bg2cnt & 0x43) | 0x1900;
    SUB_DISPLAY->bg3cnt = (SUB_DISPLAY->bg3cnt & 0x43) | 0x1608;
    SUB_DISPLAY->bg1cnt = (SUB_DISPLAY->bg1cnt & ~3) | 3;
    SUB_DISPLAY->bg2cnt = (SUB_DISPLAY->bg2cnt & ~3) | 2;
    SUB_DISPLAY->bg3cnt = (SUB_DISPLAY->bg3cnt & ~3) | 1;
    MIi_CpuClearFast(0, G2S_GetBG3CharPtr(), 0x20);
    SUB_DISPLAY->dispcnt = (SUB_DISPLAY->dispcnt & ~0x1f00) | 0xe00;
}
