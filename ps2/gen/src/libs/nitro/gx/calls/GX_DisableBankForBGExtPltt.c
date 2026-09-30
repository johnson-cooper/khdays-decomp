/* PS2: mechanically prepared copy of libs/nitro/gx/calls/GX_DisableBankForBGExtPltt.c (ps2/tools/prep_sources.py). Do not edit. */
/* NitroSDK gx (gx_vramcnt.c): GX_DisableBankForBGExtPltt -- disableBankForX_(&gGXState.vramCnt.bgExtPltt), after clearing the DISPCNT ext-palette enable (bit 30); identified by the state field it passes. */
extern void *disableBankForX_();
extern unsigned short data_020446e2;

void *GX_DisableBankForBGExtPltt(void) {
    volatile unsigned int *dispcnt = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x0);
    *dispcnt = *dispcnt & ~0x40000000;
    return disableBankForX_(&data_020446e2);
}
