/* PS2: mechanically prepared copy of src/engine/Draw_ScaledValue.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Draw_ScaledValue - draw a value through Tilemap_FillRect using a cell width chosen by a flag.
 *
 * Picks the cell width from self[0x38] (0x40 wide when set, 0x20 narrow otherwise), divides
 * the value self[0x10] by that width via the 64-bit divide kh_rt_s32_divmod, and draws with
 * Tilemap_FillRect at (self[4], self[8]) passing the quotient. A negative line-height argument
 * (param_5) defaults to 0xf.
 *
 * param_5 is copied to a local so mwcc keeps it in a register (r4) instead of spilling the
 * modified parameter; the quotient is passed to the draw's u16 slot unmasked (no extra
 * lsl/lsr zero-extend).
 */

extern unsigned long long kh_rt_s32_divmod(unsigned int value, unsigned int divisor);
extern void Tilemap_FillRect(unsigned int a, int b, int c, int d, int e, int f, unsigned int g, int h);

void Draw_ScaledValue(void *pSelf, unsigned int param_2, int param_3, int param_4, int param_5)
{
    int self = (int)pSelf;
    unsigned int width;
    unsigned int q;
    int e = param_5;

    if (e < 0)
        e = 0xf;
    width = *(int *)(self + 0x38) != 0 ? 0x40 : 0x20;
    q = (unsigned int)kh_rt_s32_divmod(*(unsigned int *)(self + 0x10), width);
    Tilemap_FillRect(param_2, *(int *)(self + 4), *(int *)(self + 8), param_4, param_3, 0x40, q, e);
}
