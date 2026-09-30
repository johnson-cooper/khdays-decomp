/* PS2: mechanically prepared copy of src/engine/SetSubEngineGraphicsModeFromTable.c (ps2/tools/prep_sources.py). Do not edit. */
extern void GXS_SetGraphicsMode(int mode);
/* Pick the sub-engine graphics mode for the current BG mode (DISPCNT_B & 7) from the table,
 * folding entries >= 8 back into range. */
void SetSubEngineGraphicsModeFromTable(int *table) {
    int mode = table[*(volatile unsigned int *)((unsigned int)kh_ds_io + 0x1000) & 7];
    if (mode >= 8) {
        mode -= 8;
    }
    GXS_SetGraphicsMode(mode);
}
