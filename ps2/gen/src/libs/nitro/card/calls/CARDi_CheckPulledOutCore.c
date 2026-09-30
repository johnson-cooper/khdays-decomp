/* PS2: mechanically prepared copy of libs/nitro/card/calls/CARDi_CheckPulledOutCore.c (ps2/tools/prep_sources.py). Do not edit. */
extern int OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(int state);
extern void CARDi_PulledOutCallback(int a, int b, int c);

/* Raises the "card pulled out" fault when the cartridge id no longer matches the one recorded at
 * boot. */
void CARDi_CheckPulledOutCore(int id) {
    volatile int stored;
    int enabled;
    int *src;
    if (*(unsigned short *)((unsigned int)kh_ds_hiram + 0x1fc10) == 0) {
        src = (int *)((unsigned int)kh_ds_hiram + 0x1f800);
    } else {
        src = (int *)((unsigned int)kh_ds_hiram + 0x1fc00);
    }
    stored = *src;
    if (id != stored) {
        enabled = OS_DisableInterrupts();
        CARDi_PulledOutCallback(0xe, 0x11, 0);
        OS_RestoreInterrupts(enabled);
    }
}
