/* PS2: mechanically prepared copy of libs/nitro/os/calls/OS_UnlockCartridge_0x02001704.c (ps2/tools/prep_sources.py). Do not edit. */
/* NitroSDK os (os_spinLock.c): OS_UnlockCartridge -- unlocks the cartridge lock word, freeing the bus. The address suffix is only there because a flat OS_UnlockCartridge.o would be the same file as the OS_UnLockCartridge.o alias on a case-insensitive file system. */
extern void *OSi_DoUnlockByWord();
extern void OSi_FreeCartridgeBus(void);

void *OS_UnlockCartridge_0x02001704(int id) {
    return OSi_DoUnlockByWord(id, (void *)((unsigned int)kh_ds_hiram + 0x1ffe8), OSi_FreeCartridgeBus, 1);
}
