/* PS2: mechanically prepared copy of src/engine/SetMasterBrightnessSub.c (ps2/tools/prep_sources.py). Do not edit. */
extern unsigned short *GXx_SetMasterBrightness_(unsigned short *p, int v);
extern unsigned char data_027e0080;
extern unsigned char gMasterBrightness;

/* Sub-engine master brightness: apply now if DISPSTAT says we are in V-blank,
 * else stash it and flag a deferred update (data_027e0080 bit 1). */
void SetMasterBrightnessSub(int brightness) {
    (&gMasterBrightness)[1] = (unsigned char)brightness;
    {
        volatile unsigned short *reg_dispstat = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x4);
        if (*reg_dispstat & 1) {
            GXx_SetMasterBrightness_((unsigned short *)((unsigned int)kh_ds_io + 0x106c), brightness); /* MASTER_BRIGHT (sub) */
            data_027e0080 &= ~2;
        } else {
            data_027e0080 |= 2;
        }
    }
}
