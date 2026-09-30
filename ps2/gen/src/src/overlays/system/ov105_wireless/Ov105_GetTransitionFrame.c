/* PS2: mechanically prepared copy of src/overlays/system/ov105_wireless/Ov105_GetTransitionFrame.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov105_GetTransitionFrame -- read the current transition frame, ov105. Returns 0x8000 while
 * a transition is running (Ov105_IsDeviceReady != 0), else the shared frame word @0x027ffcfa. */
extern int Ov105_IsDeviceReady(void);
int Ov105_GetTransitionFrame(void) {
    if (Ov105_IsDeviceReady() != 0) {
        return 0x8000;
    }
    return *(unsigned short *)((unsigned int)kh_ds_hiram + 0x1fcfa);
}
