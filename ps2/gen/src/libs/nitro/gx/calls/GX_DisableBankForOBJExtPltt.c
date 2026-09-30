/* PS2: mechanically prepared copy of libs/nitro/gx/calls/GX_DisableBankForOBJExtPltt.c (ps2/tools/prep_sources.py). Do not edit. */
/* NitroSDK gx (gx_vramcnt.c): GX_DisableBankForOBJExtPltt -- disableBankForX_(&gGXState.vramCnt.objExtPltt), after clearing the DISPCNT ext-palette enable (bit 31); identified by the state field it passes. */
extern void *disableBankForX_();
extern unsigned short data_020446e4;

void *GX_DisableBankForOBJExtPltt(void) {
    volatile unsigned int *dispcnt = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x0);
    *dispcnt = *dispcnt & ~0x80000000;
    return disableBankForX_(&data_020446e4);
}
