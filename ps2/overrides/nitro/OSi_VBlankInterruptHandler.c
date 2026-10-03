/* PS2 replacement for OSi_VBlankInterruptHandler (libs/nitro/os/calls/OSi_VBlankInterruptHandler.c).
 * Despite the SDK-style name this is the game's own VBlank IRQ handler (Boot_InitVBlank installs
 * it): it counts VBlanks, commits pending master-brightness changes and runs the named VBlank
 * task list (RegisterNamedTask) - e.g. ov002's BGUIVBFUNC, whose step nodes drive the field's
 * caption fade.  Identical logic; only the DTCM IRQ check-flag word (DTCM + 0x3ff8) becomes
 * OS_SetIrqCheckFlag, since there is no DTCM block on the PS2.  OS_WaitVBlankIntr calls it at
 * thread level once per VBlank (nitro_core.c run_vblank). */

extern unsigned short *GXx_SetMasterBrightness_(unsigned short *p, int v);
extern void OS_SetIrqCheckFlag(unsigned int mask);

extern unsigned char data_027e0080;
extern signed char gMasterBrightness;

struct VBlankNode {
    int pad0[5];
    void (*func)(void);
    struct VBlankNode *next;
};

struct VBlankCtx {
    unsigned int counter;
    struct VBlankNode *list;
};

extern struct VBlankCtx data_027e0088;

void OSi_VBlankInterruptHandler(void)
{
    struct VBlankCtx *ctx;
    struct VBlankNode *p;
    unsigned char flags;

    ctx = &data_027e0088;
    flags = data_027e0080;
    ctx->counter = ctx->counter + 1;

    if (flags & 1)
        GXx_SetMasterBrightness_((unsigned short *)((unsigned int)kh_ds_io + 0x6c), (&gMasterBrightness)[0]);
    if (flags & 2)
        GXx_SetMasterBrightness_((unsigned short *)((unsigned int)kh_ds_io + 0x106c), (&gMasterBrightness)[1]);
    p = ctx->list;
    data_027e0080 = 0;
    while (p) {
        (*p->func)();
        p = p->next;
    }
    OS_SetIrqCheckFlag(1);
}
