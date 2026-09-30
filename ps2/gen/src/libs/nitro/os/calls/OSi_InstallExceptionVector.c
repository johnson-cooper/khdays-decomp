/* PS2: mechanically prepared copy of libs/nitro/os/calls/OSi_InstallExceptionVector.c (ps2/tools/prep_sources.py). Do not edit. */
extern void OSi_ExceptionHandler(void);
extern char data_020445b4[];

void OSi_InstallExceptionVector(void)
{
    unsigned int v = *(volatile unsigned int *)((unsigned int)kh_ds_hiram + 0x1fd9c);
    if (v >= 0x02600000u && v < 0x02800000u) {
        *(unsigned int *)data_020445b4 = v;
    } else {
        *(unsigned int *)data_020445b4 = 0;
    }
    if (*(unsigned int *)data_020445b4 == 0) {
        *(volatile void **)((unsigned int)kh_ds_hiram + 0x1fd9c) = (void *)OSi_ExceptionHandler;
        {
            volatile char *base = (volatile char *)((unsigned int)kh_ds_hiram + 0x3000);
            *(volatile void **)(base + 0xFDC) = (void *)OSi_ExceptionHandler;
        }
    }
    *(unsigned int *)(data_020445b4 + 8) = 0;
}
