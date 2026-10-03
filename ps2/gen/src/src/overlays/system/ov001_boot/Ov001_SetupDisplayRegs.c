/* PS2: mechanically prepared copy of src/overlays/system/ov001_boot/Ov001_SetupDisplayRegs.c (ps2/tools/prep_sources.py). Do not edit. */
/* Boot display setup: initialises the 3D engine, VRAM banks, graphics mode, backgrounds, blending,
 * viewport and the sub screen. */

extern void NNS_G3dInit(void);
extern void G3X_InitMtxStack(void);
extern void GX_SetBankForTex(int bank);
extern void GX_SetBankForTexPltt(int offset);
extern void GX_SetBankForBG(int bank);
extern void GX_SetBankForBGExtPltt(int bank);
extern void GX_SetGraphicsMode(int a, int b, int c);
extern void Ov001_SetupSubScreenBanks(void);
extern void DispCnt_ApplyPendingMode(void);

void Ov001_SetupDisplayRegs(void) {
    volatile unsigned int *reg_dispcnt = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x0);
    volatile unsigned short *reg_bg0cnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x8);
    volatile unsigned short *reg_bg1cnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0xa);
    volatile unsigned short *reg_bldcnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x50);
    volatile unsigned short *reg_disp3dcnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x60);
    volatile unsigned int *reg_swap_buffers = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x540);
    volatile unsigned int *reg_viewport = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x580);
    volatile unsigned int *reg_04001000 = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x1000);

    NNS_G3dInit();
    G3X_InitMtxStack();
    GX_SetBankForTex(7);
    GX_SetBankForTexPltt(0x60);
    GX_SetBankForBG(0x10);
    GX_SetBankForBGExtPltt(0);
    GX_SetGraphicsMode(1, 0, 1);

    *reg_dispcnt = (*reg_dispcnt & ~0x1f00) | 0x300;
    *reg_dispcnt &= ~0x7000000;
    *reg_dispcnt &= ~0x38000000;

    *reg_bg1cnt = (unsigned short)((*reg_bg1cnt & 0x43) | 0x400);
    *reg_bg0cnt = (unsigned short)((*reg_bg0cnt & ~3) | 1);
    *reg_bg1cnt = (unsigned short)(*reg_bg1cnt & ~3);
    *reg_disp3dcnt = (unsigned short)(*reg_disp3dcnt & ~0x3002);
    *reg_disp3dcnt = (unsigned short)((*reg_disp3dcnt & ~0x3000) | 0x10);
    *reg_bldcnt = 0;
    kh_ge_port_write1(0x540, (unsigned int)(2));
    *reg_disp3dcnt = (unsigned short)(*reg_disp3dcnt & 0xcffb);
    *reg_disp3dcnt = (unsigned short)((*reg_disp3dcnt & ~0x3000) | 8);
    kh_ge_port_write1(0x580, (unsigned int)(0xbfff0000));

    Ov001_SetupSubScreenBanks();
    DispCnt_ApplyPendingMode();
    *reg_04001000 |= 0x10000;
}
