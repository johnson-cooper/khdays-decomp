/* PS2: mechanically prepared copy of src/engine/Touch_GetRecentSamples.c (ps2/tools/prep_sources.py). Do not edit. */

/* TPData: x, y, touch, validity -- the touch-panel sample layout TP_GetCalibratedPoint
 * (TP_GetCalibratedPoint) converts. */

#include "nitro/types.h"

typedef struct {
    u16 field_00;
    u16 field_02;
    u16 field_04;
    u16 field_06;
} Rec;

extern int TP_GetLatestIndexInAuto(void);
extern void TP_GetCalibratedPoint(Rec *out, Rec *in);
extern void MI_CpuCopy8(const void *src, void *dst, unsigned int len);
extern Rec data_0204c1c4[];

/* Copies the four most recent auto-sampled touch points (ring of 5 at data_0204c1c4) into table,
 * calibrated; returns how many (0 while the calibration-disable bit of 0x027fffa8 is set). */
int Touch_GetRecentSamples(Rec *table) {
    int latest;
    int i;
    short count = 0;

    if (!((*(volatile u16 *)((unsigned int)kh_ds_hiram + 0x1ffa8) & 0x8000) >> 15)) {
        Rec buf;

        latest = TP_GetLatestIndexInAuto();
        i = 0;
        latest -= 3;

        for (; i < 4; i++) {
            int idx = latest + i;
            if (idx < 0) {
                idx += 5;
            }
            TP_GetCalibratedPoint(&buf, &data_0204c1c4[idx]);
            MI_CpuCopy8(&buf, &table[count++], 8);
        }
    }

    return count;
}
