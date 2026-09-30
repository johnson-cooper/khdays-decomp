/* PS2: mechanically prepared copy of src/engine/DispMode_LookupWordAndDispatch.c (ps2/tools/prep_sources.py). Do not edit. */
/* Reads the current display mode from REG_DISPCNT, looks the mode up in `tbl`, folds the
 * value into 0..7 and tail-calls GX_SetGraphicsMode with the DISPCNT bit-3 flag.
 *
 * The table load must be written inline as `tbl[*reg & 7]`: bound to a `mode` local first,
 * mwcc schedules the load after the boolean materialisation and colours the flag one
 * register away from the ROM's r0. */
extern void *GX_SetGraphicsMode();

void *DispMode_LookupWordAndDispatch(int *tbl) {
    volatile unsigned int *reg = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x0);
    int v = tbl[*reg & 7];
    int on = (*reg & 8) != 0;
    int b2 = on != 0;
    if (v >= 8) {
        v -= 8;
    }
    return GX_SetGraphicsMode(1, v, b2);
}
