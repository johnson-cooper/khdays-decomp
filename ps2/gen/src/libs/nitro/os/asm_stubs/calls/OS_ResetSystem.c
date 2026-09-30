/* PS2: mechanically prepared copy of libs/nitro/os/asm_stubs/calls/OS_ResetSystem.c (ps2/tools/prep_sources.py). Do not edit. */
/* NitroSDK OS_ResetSystem (ARM9): refuses on a multiboot child (OS_Terminate), locks the card ROM,
 * stops the four DMA channels, masks every interrupt but the PXI receive FIFO (OS_IE_FIFO_RECV)
 * and clears the pending requests, leaves `parameter` for the ARM7 at HW_RESET_PARAMETER_BUF
 * (0x027ffc20), sends it the PXI reset command (0x10), then moves the stack below the DTCM top and
 * runs OSi_FinalizeReset, which does not return. */
extern void OS_Terminate(void);
extern int OS_GetLockID(void);
extern void CARD_LockRom(unsigned short lockId);
extern void MI_StopDma(int channel);
extern void OS_SetIrqMask(unsigned int mask);
extern void OS_ResetRequestIrqMask(unsigned int mask);
extern void OSi_SendToPxi(unsigned int data);
extern void OSi_FinalizeReset(void);

static inline int MB_IsMultiBootChild(void)
{
    return *(volatile unsigned short *)((unsigned int)kh_ds_hiram + 0x1fc40) == 2;
}

void OS_ResetSystem(unsigned int parameter)
{
    unsigned short lockId;

    if (MB_IsMultiBootChild()) {
        OS_Terminate();
    }

    lockId = OS_GetLockID();
    CARD_LockRom(lockId);
    MI_StopDma(0);
    MI_StopDma(1);
    MI_StopDma(2);
    MI_StopDma(3);
    OS_SetIrqMask(0x00040000);
    OS_ResetRequestIrqMask(0xffffffff);
    *(volatile unsigned int *)((unsigned int)kh_ds_hiram + 0x1fc20) = parameter;
    OSi_SendToPxi(0x10);
    asm {
        ldr r0, =0x027e3f80
        ldr r1, =0x800
        sub r0, r0, r1
        mov sp, r0
        bl OSi_FinalizeReset
    }
}
