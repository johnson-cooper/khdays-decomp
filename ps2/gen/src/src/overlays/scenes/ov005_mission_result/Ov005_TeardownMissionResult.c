/* PS2: mechanically prepared copy of src/overlays/scenes/ov005_mission_result/Ov005_TeardownMissionResult.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/engine.h"

extern void Ov005_RegisterAnimTables(void);
extern void func_02023ad0(int h);
extern void MI_CpuFill8(void *dst, int value, unsigned size);
extern int **data_ov005_0205b808;
extern int data_0204c32c;

/* Mission-result teardown: drops the three widgets, forces the capture bit in POWCNT, releases
 * the two node handles and clears the shared result block. */
void Ov005_TeardownMissionResult(void) {
    Ov005_RegisterAnimTables();
    ResSlot_Release_2(0x15);
    ResSlot_Release_2(0x1a);
    ResSlot_Release_2(0x19);
    *(volatile unsigned short *)((unsigned int)kh_ds_io + 0x304) |= 0x8000;
    func_02023ad0(**(int **)&data_ov005_0205b808);
    func_02023ad0((*(int **)&data_ov005_0205b808)[1]);
    MI_CpuFill8(&data_0204c32c, 0, 0xac);
}
