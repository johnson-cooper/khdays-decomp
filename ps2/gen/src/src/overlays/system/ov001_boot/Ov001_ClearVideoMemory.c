/* PS2: mechanically prepared copy of src/overlays/system/ov001_boot/Ov001_ClearVideoMemory.c (ps2/tools/prep_sources.py). Do not edit. */
/* Maps all VRAM to LCDC and clears it, then clears OAM (hidden) and both palettes. */

extern int GX_SetBankForLCDC();
extern int MIi_CpuClearFast();
extern int GX_DisableBankForLCDC();

void Ov001_ClearVideoMemory(void) {
    GX_SetBankForLCDC(0x1ff);
    MIi_CpuClearFast(0, (void *)((unsigned int)kh_ds_vram + 0x0), 0xa4000);
    GX_DisableBankForLCDC();
    MIi_CpuClearFast(0xc0, (void *)((unsigned int)kh_ds_oam + 0x0), 0x400);
    MIi_CpuClearFast(0xc0, (void *)((unsigned int)kh_ds_oam + 0x400), 0x400);
    MIi_CpuClearFast(0, (void *)((unsigned int)kh_ds_pal + 0x0), 0x400);
    MIi_CpuClearFast(0, (void *)((unsigned int)kh_ds_pal + 0x400), 0x400);
}
