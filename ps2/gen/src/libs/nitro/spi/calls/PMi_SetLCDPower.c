/* PS2: mechanically prepared copy of libs/nitro/spi/calls/PMi_SetLCDPower.c (ps2/tools/prep_sources.py). Do not edit. */
extern int PMi_SetLED(int param_1);
extern int PMi_SetLEDAsync(int, void *, void *);
extern int PMi_SetAmp(int value);

extern struct { char _0[0x10]; int field_10; int field_14; } data_020463cc;

int PMi_SetLCDPower(int mode, int chan, int useTimeout, int useRoute)
{
    volatile unsigned short *reg_powcnt1 = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x304);

    if (mode != 0) {
        if (mode == 1) {
            if (useTimeout == 0) {
                if ((unsigned int)(*(int *)((unsigned int)kh_ds_hiram + 0x1fc3c) - data_020463cc.field_10) <= 7) {
                    return 0;
                }
            }

            if (chan != 0) {
                if (useRoute != 0) {
                    PMi_SetLED(chan);
                } else {
                    PMi_SetLEDAsync(chan, 0, 0);
                }
            }
            *reg_powcnt1 |= 1;
            PMi_SetAmp(data_020463cc.field_14);
        }
    } else {
        PMi_SetAmp(0);
        *reg_powcnt1 &= ~1;
        data_020463cc.field_10 = *(int *)((unsigned int)kh_ds_hiram + 0x1fc3c);
        if (chan != 0) {
            if (useRoute != 0) {
                PMi_SetLED(chan);
            } else {
                PMi_SetLEDAsync(chan, 0, 0);
            }
        }
    }
    return 1;
}
