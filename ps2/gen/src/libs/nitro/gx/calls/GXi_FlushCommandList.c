/* PS2: mechanically prepared copy of libs/nitro/gx/calls/GXi_FlushCommandList.c (ps2/tools/prep_sources.py). Do not edit. */
extern void MIi_CpuSend32(const void *src, volatile void *dst, unsigned int size);

extern volatile int data_027e0078;
extern int *data_027e0074;

void GXi_FlushCommandList(void) {
    while (data_027e0078) {
        ;
    }
    {
        int *list = data_027e0074;
        if (list[0] == 0) return;
        MIi_CpuSend32(list + 1, (volatile void *)((unsigned int)kh_ds_io + 0x400), (unsigned int)(list[0] << 2));
        data_027e0074[0] = 0;
    }
}
