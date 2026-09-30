/* PS2: mechanically prepared copy of src/overlays/scenes/ov007_monologue/Ov007_TextWindowFadeIn.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/engine.h"

extern int NNSi_FndGetCurrentRootHeap(void);
extern void Text_UploadTileBuffer(void *p);
extern int Ov007_AdvanceTextLine(void);
extern unsigned short gPadPressed;
extern int Ov007_FadeOutStep(void);

/* Text window fade-in: on entry set the window colour to white (0x7fff) and
 * render it; each frame brighten the backdrop (grey RGB555 = frame in all
 * channels) until frame 0x20, then re-render dimmed and go to d0ec. A-press
 * (bit 1) forces the render + jump to d0ec; B-press (bit 8) -> fade-out d2d0. */
int Ov007_TextWindowFadeIn(void) {
    int root = NNSi_FndGetCurrentRootHeap();
    int ret = 0;
    int frame;

    if (*(int *)(root + 0x20) == 0) {
        *(volatile unsigned short *)((unsigned int)kh_ds_pal + 0x2) = 0x7fff;
        *(volatile unsigned short *)((unsigned int)kh_ds_pal + 0x4) = 0;
        Text_DrawDirectional(root + 0x30, 0x80, *(int *)(root + 0x70) * 0x12 + 0xe, 2, 0x14, root + 0x7c);
        Text_UploadTileBuffer((void *)(root + 0x30));
    }
    frame = *(int *)(root + 0x20) + 1;
    *(int *)(root + 0x20) = frame;
    if (frame >= 0x20) {
        Text_DrawDirectional(root + 0x30, 0x80, *(int *)(root + 0x70) * 0x12 + 0xe, 1, 0x14, root + 0x7c);
        Text_UploadTileBuffer((void *)(root + 0x30));
        *(int *)(root + 0x20) = 0;
        ret = (int)Ov007_AdvanceTextLine;
    } else {
        *(volatile unsigned short *)((unsigned int)kh_ds_pal + 0x4) = frame | frame * 0x20 | frame * 0x400;
    }
    if ((gPadPressed & 1) != 0) {
        if (*(int *)(root + 0x20) < 0x20) {
            Text_DrawDirectional(root + 0x30, 0x80, *(int *)(root + 0x70) * 0x12 + 0xe, 1, 0x14, root + 0x7c);
            Text_UploadTileBuffer((void *)(root + 0x30));
        }
        ret = (int)Ov007_AdvanceTextLine;
        *(int *)(root + 0x20) = 0xf;
    }
    if ((gPadPressed & 8) != 0) {
        *(int *)(root + 0x20) = 0;
        ret = (int)Ov007_FadeOutStep;
    }
    return ret;
}
