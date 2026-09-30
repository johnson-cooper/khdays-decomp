/* PS2: mechanically prepared copy of src/overlays/scenes/ov005_mission_result/Ov005_ConfigureBackgroundPriorities.c (ps2/tools/prep_sources.py). Do not edit. */
/* Assign background priorities on the main and sub display engines. */

#include "nitro/types.h"

#define BG_CONTROL(address) (*(volatile u16 *)(address))
void Ov005_ConfigureBackgroundPriorities(void) {
    BG_CONTROL(((unsigned int)kh_ds_io + 0x100a))=(BG_CONTROL(((unsigned int)kh_ds_io + 0x100a))&~3)|3;
    BG_CONTROL(((unsigned int)kh_ds_io + 0x100e))=(BG_CONTROL(((unsigned int)kh_ds_io + 0x100e))&~3)|2;
    BG_CONTROL(((unsigned int)kh_ds_io + 0x100c))=BG_CONTROL(((unsigned int)kh_ds_io + 0x100c))&~3;
    BG_CONTROL(((unsigned int)kh_ds_io + 0xa))=(BG_CONTROL(((unsigned int)kh_ds_io + 0xa))&~3)|3;
    BG_CONTROL(((unsigned int)kh_ds_io + 0xe))=(BG_CONTROL(((unsigned int)kh_ds_io + 0xe))&~3)|2;
    BG_CONTROL(((unsigned int)kh_ds_io + 0xc))=BG_CONTROL(((unsigned int)kh_ds_io + 0xc))&~3;
}
