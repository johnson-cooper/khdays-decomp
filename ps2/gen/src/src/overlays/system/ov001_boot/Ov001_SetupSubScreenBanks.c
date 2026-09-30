/* PS2: mechanically prepared copy of src/overlays/system/ov001_boot/Ov001_SetupSubScreenBanks.c (ps2/tools/prep_sources.py). Do not edit. */
/* Maps the sub BG/OBJ VRAM banks and enables the sub engine's 1D OBJ mapping. */

extern int GX_SetBankForSubBG();
extern int GX_SetBankForSubOBJ();

void Ov001_SetupSubScreenBanks(void) {
    volatile unsigned int *p;

    GX_SetBankForSubBG(0x180);
    GX_SetBankForSubOBJ(8);
    p = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x1000);
    *p = (*p & 0xffcfffefu) | 0x10u | 0x200000u;
}
