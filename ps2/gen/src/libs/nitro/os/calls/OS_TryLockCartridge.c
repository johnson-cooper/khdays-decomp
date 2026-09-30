/* PS2: mechanically prepared copy of libs/nitro/os/calls/OS_TryLockCartridge.c (ps2/tools/prep_sources.py). Do not edit. */
/* NitroSDK os (os_spinLock.c): OS_TryLockCartridge -- tries the cartridge lock word, allocating the bus. */
extern void *OSi_DoTryLockByWord();
extern void OSi_AllocateCartridgeBus(void);

void *OS_TryLockCartridge(int id) {
    return OSi_DoTryLockByWord(id, (void *)((unsigned int)kh_ds_hiram + 0x1ffe8), OSi_AllocateCartridgeBus, 1);
}
