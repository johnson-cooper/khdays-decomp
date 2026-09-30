/* PS2: mechanically prepared copy of libs/nitro/spi/calls/PM_Init.c (ps2/tools/prep_sources.py). Do not edit. */
extern void PXI_Init(void);
extern int PXI_IsCallbackReady(int fifoNo, int kind);
extern void PXI_SetFifoRecvCallback(int fifoNo, void (*cb)(int, unsigned int));
extern void OS_InitMutex(void *mutex);
extern void PMi_CommonCallback(int fifoNo, unsigned int data);

extern struct {
    unsigned short initialized;
    char _2[0xe];
    int field_10;
    char _14[0x8];
    int field_1c;
    int field_20;
} data_020463cc;

extern unsigned short data_02046410[5][4];
extern int data_020463f8;

void PM_Init(void)
{
    int i;

    if (data_020463cc.initialized != 0)
        return;

    data_020463cc.initialized = 1;
    data_020463cc.field_1c = 0;
    data_020463cc.field_20 = 0;
    PXI_Init();
    while (!PXI_IsCallbackReady(8, 1)) {
    }
    PXI_SetFifoRecvCallback(8, PMi_CommonCallback);
    for (i = 0; i < 5; i++) {
        data_02046410[i][0] = 0;
    }
    OS_InitMutex(&data_020463f8);
    data_020463cc.field_10 = *(int *)((unsigned int)kh_ds_hiram + 0x1fc3c);
}
