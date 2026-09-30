/* PS2: mechanically prepared copy of src/overlays/system/ov105_wireless/Ov105_ClearSharedRequestBit.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov105_ClearSharedRequestBit -- clear the low request bit of the shared control word @0x027fff96,
 * ov105 (only if set). */
void Ov105_ClearSharedRequestBit(void) {
    unsigned short *p = (unsigned short *)((unsigned int)kh_ds_hiram + 0x1ff96);
    if (*p & 1) {
        *p &= ~1;
    }
}
