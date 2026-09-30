/* PS2: mechanically prepared copy of libs/nitro/gx/auto/G3X_GetMtxStackLevelPV.c (ps2/tools/prep_sources.py). Do not edit. */
/* Reads the matrix-stack level out of GXSTAT; -1 if the stack-error bit is set. */
int G3X_GetMtxStackLevelPV(int *level) {
    if ((*(volatile unsigned *)((unsigned int)kh_ds_io + 0x600) & 0x4000) != 0) {
        return -1;
    }
    *level = (*(volatile unsigned *)((unsigned int)kh_ds_io + 0x600) & 0x1f00) >> 8;
    return 0;
}
