/* PS2: mechanically prepared copy of src/overlays/scenes/ov011/Ov011_StartTitleExit.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov011_StartTitleExit -- leave the title: scene mode 4, the exit window
 * opens at the current cursor timer and closes 30 frames later, both
 * engines switch to display mode 1 with BG1/BG3 (main) and BG1 (sub), and
 * the fade table entry 0 is kicked for 30 frames.
 */

/* Ov011ExitWindow */

#include "nitro/types.h"

struct ExitWindow {
    u16 nStart;                  /* 0x00 */
    u16 nEnd;                    /* 0x02 */
};

/* Ov011Scene */
struct Scene {
    u8 pad00000[4];
    int nMode;                   /* 0x00004 */
    u8 pad00008[0x2cf44];
    struct ExitWindow exit;      /* 0x2cf4c */
};

/* Ov011Globals */
struct Globals {
    int nCursor;                 /* 0x00 */
    struct Scene *pScene;        /* 0x04 */
};

#define REG_DISPCNT (*(volatile u32 *)((unsigned int)kh_ds_io + 0x0))
#define REG_DISPCNT_SUB (*(volatile u32 *)((unsigned int)kh_ds_io + 0x1000))
#define DISPCNT_BG_MASK 0x1f00
#define DISPCNT_MAIN_BGS 0x1500
#define DISPCNT_SUB_BGS 0x1100
#define MODE_EXIT 4
#define EXIT_FRAMES 0x1e

extern struct Globals data_ov011_0205e960;

extern void Table_TailCallWithEntry(int nEntry, int nFrames);                             /* Table_TailCallWithEntry */

void Ov011_StartTitleExit(void)
{
    struct ExitWindow *pExit;

    data_ov011_0205e960.pScene->nMode = MODE_EXIT;
    pExit = &data_ov011_0205e960.pScene->exit;
    pExit->nStart = data_ov011_0205e960.nCursor;
    pExit->nEnd = data_ov011_0205e960.nCursor + EXIT_FRAMES;
    REG_DISPCNT = (REG_DISPCNT & ~DISPCNT_BG_MASK) | DISPCNT_MAIN_BGS;
    REG_DISPCNT_SUB = (REG_DISPCNT_SUB & ~DISPCNT_BG_MASK) | DISPCNT_SUB_BGS;
    Table_TailCallWithEntry(0, EXIT_FRAMES);
}
