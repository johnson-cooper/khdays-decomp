/* PS2: mechanically prepared copy of libs/nitro/gx/calls/G3X_ResetMtxStack_2.c (ps2/tools/prep_sources.py). Do not edit. */

#include "nitro/types.h"

#define reg_G3_GXSTAT        (*(REGType32v *)((unsigned int)kh_ds_io + 0x600))
#define reg_G3_MTX_MODE      (*(REGType32v *)((unsigned int)kh_ds_io + 0x440))
#define reg_G3_MTX_POP       (*(REGType32v *)((unsigned int)kh_ds_io + 0x448))
#define reg_G3_MTX_IDENTITY  (*(REGType32v *)((unsigned int)kh_ds_io + 0x454))

extern int G3X_GetMtxStackLevelPV(int *level);
extern int G3X_GetMtxStackLevelPJ(int *level);

void G3X_ResetMtxStack_2(void) {
    int pvLevel;
    int pjLevel;

    kh_ge_port_read(0x600) |= 0x8000;

    while (G3X_GetMtxStackLevelPV(&pvLevel) != 0) {}
    while (G3X_GetMtxStackLevelPJ(&pjLevel) != 0) {}

    kh_ge_port_write1(0x440, (unsigned int)(3));
    kh_ge_port_write1(0x454, (unsigned int)(0));
    kh_ge_port_write1(0x440, (unsigned int)(0));

    if (pjLevel != 0) {
        kh_ge_port_write1(0x448, (unsigned int)(pjLevel));
    }

    kh_ge_port_write1(0x440, (unsigned int)(2));
    kh_ge_port_write1(0x448, (unsigned int)(pvLevel));
    kh_ge_port_write1(0x454, (unsigned int)(0));
}
