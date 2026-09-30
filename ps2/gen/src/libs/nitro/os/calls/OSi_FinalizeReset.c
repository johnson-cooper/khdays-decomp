/* PS2: mechanically prepared copy of libs/nitro/os/calls/OSi_FinalizeReset.c (ps2/tools/prep_sources.py). Do not edit. */
extern void Boot_ReloadModules(void);
extern void func_01ff8330(void);

extern volatile unsigned short data_02044694;

void OSi_FinalizeReset(void)
{
    while (data_02044694 == 0) { }
    *(volatile unsigned short *)((unsigned int)kh_ds_io + 0x208) = 0;
    Boot_ReloadModules();
    func_01ff8330();
}
