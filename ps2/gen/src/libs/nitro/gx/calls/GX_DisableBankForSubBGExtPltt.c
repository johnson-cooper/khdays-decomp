/* PS2: mechanically prepared copy of libs/nitro/gx/calls/GX_DisableBankForSubBGExtPltt.c (ps2/tools/prep_sources.py). Do not edit. */
/* NitroSDK gx (gx_vramcnt.c): GX_DisableBankForSubBGExtPltt -- disableBankForX_(&gGXState.vramCnt.sub_bgExtPltt), after clearing the sub-DISPCNT ext-palette enable (bit 30); identified by the state field it passes. */
extern void *disableBankForX_();
extern unsigned short data_020446ea;

void *GX_DisableBankForSubBGExtPltt(void) {
    volatile unsigned int *dispcnt = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x1000);
    *dispcnt = *dispcnt & ~0x40000000u;
    return disableBankForX_(&data_020446ea);
}
