/* PS2: mechanically prepared copy of libs/nitro/gx/calls/GX_ResetBankForBGExtPltt.c (ps2/tools/prep_sources.py). Do not edit. */
/* NitroSDK gx (gx_vramcnt.c): GX_ResetBankForBGExtPltt -- resetBankForX_(&gGXState.vramCnt.bgExtPltt), after clearing the DISPCNT ext-palette enable (bit 30); identified by the state field it passes. */
extern void *resetBankForX_();
extern unsigned short data_020446e2;

void *GX_ResetBankForBGExtPltt(void) {
    volatile unsigned int *dispcnt = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x0);
    *dispcnt = *dispcnt & ~0x40000000;
    return resetBankForX_(&data_020446e2);
}
