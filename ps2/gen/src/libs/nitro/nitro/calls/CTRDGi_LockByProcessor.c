/* PS2: mechanically prepared copy of libs/nitro/nitro/calls/CTRDGi_LockByProcessor.c (ps2/tools/prep_sources.py). Do not edit. */
extern int OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(int state);
extern unsigned short OS_ReadOwnerOfLockWord(void *lock);
extern int OS_TryLockCartridge(int owner);
extern void WaitByLoop(int count);

/* Spins until this processor owns the cartridge bus. `out` receives the saved interrupt state and
 * the "already locked by the other CPU" flag. */
void CTRDGi_LockByProcessor(int owner, char *out) {
    for (;;) {
        *(int *)(out + 4) = OS_DisableInterrupts();
        *(int *)out = OS_ReadOwnerOfLockWord((void *)((unsigned int)kh_ds_hiram + 0x1ffe8)) & 0x40;
        if (*(int *)out != 0) {
            return;
        }
        if (OS_TryLockCartridge(owner) == 0) {
            return;
        }
        OS_RestoreInterrupts(*(int *)(out + 4));
        WaitByLoop(1);
    }
}
