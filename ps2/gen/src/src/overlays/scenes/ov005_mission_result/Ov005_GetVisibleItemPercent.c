/* PS2: mechanically prepared copy of src/overlays/scenes/ov005_mission_result/Ov005_GetVisibleItemPercent.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov005_GetVisibleItemPercent -- how many rows fit in the list window: 0x2bc divided by (row height at
 * +0x4c00 of the ov005 context, plus the 7-pixel gap), truncated to a signed byte.
 * kh_rt_s32_divmod is the runtime signed divide and returns quotient in r0 / remainder in r1, which
 * C sees as a packed `long long`; only the quotient is wanted here. */
extern long long kh_rt_s32_divmod(int a, int b);
extern int data_ov005_0205b80c;

int Ov005_GetVisibleItemPercent(void) {
    return (signed char)(int)kh_rt_s32_divmod(0x2bc,
        *(int *)(*(int *)&data_ov005_0205b80c + 0x4c00) + 7);
}
