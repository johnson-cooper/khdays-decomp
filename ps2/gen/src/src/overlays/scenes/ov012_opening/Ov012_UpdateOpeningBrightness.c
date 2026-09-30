/* PS2: mechanically prepared copy of src/overlays/scenes/ov012_opening/Ov012_UpdateOpeningBrightness.c (ps2/tools/prep_sources.py). Do not edit. */
/* Advances the opening brightness frame after MobiClip starts, applies the signed level to the sub
 * engine, and clamps the main engine to the timeline brightness limit. */

extern int Ov024_MobiClip_BufferedFrameCount(void);
extern void GXx_SetMasterBrightness_(volatile unsigned short *reg,
                                    int brightness);

#define REG_MASTER_BRIGHT_MAIN ((volatile unsigned short *)((unsigned int)kh_ds_io + 0x6c))
#define REG_MASTER_BRIGHT_SUB  ((volatile unsigned short *)((unsigned int)kh_ds_io + 0x106c))

void Ov012_UpdateOpeningBrightness(char *context) {
    char *base;
    int step;
    int level;

    base = context;
    context += 0x8000;
    step = *(int *)(context + 0xbe8);
    if (step < 0) {
        if (Ov024_MobiClip_BufferedFrameCount() <= 0) {
            return;
        }
        GXx_SetMasterBrightness_(REG_MASTER_BRIGHT_SUB, 0);
        return;
    }
    if (step >= 0x10) {
        return;
    }
    *(int *)(context + 0xbe8) = step + 1;
    if (*(int *)(context + 0xbe4) == 0) {
        level = -*(int *)(context + 0xbe8);
    } else {
        level = *(int *)(context + 0xbe8);
    }
    GXx_SetMasterBrightness_(REG_MASTER_BRIGHT_SUB, level);
    if (level >= *(int *)(base + 0x8bf4)) {
        level = *(int *)(base + 0x8bf4);
    }
    GXx_SetMasterBrightness_(REG_MASTER_BRIGHT_MAIN, level);
}
