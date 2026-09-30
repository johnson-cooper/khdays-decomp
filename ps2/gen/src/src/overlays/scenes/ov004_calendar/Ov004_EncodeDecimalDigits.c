/* PS2: mechanically prepared copy of src/overlays/scenes/ov004_calendar/Ov004_EncodeDecimalDigits.c (ps2/tools/prep_sources.py). Do not edit. */
/* Signed division ABI: quotient in the low word, remainder in the high word. */
extern long long kh_rt_s32_divmod(int numerator, int denominator);

/* Encode up to three decimal digits, least-significant digit first.
 * A value of -1 suppresses a leading zero glyph.
 */
void Ov004_EncodeDecimalDigits(signed char *digits, int value)
{
    int divisor;
    int digitIndex;
    int suppressLeadingZeros = 1;
    int quotient;

    digits[0] = digits[1] = digits[2] = -1;
    for (divisor = 100, digitIndex = 2; divisor > 0; divisor /= 10, digitIndex--) {
        quotient = (int)kh_rt_s32_divmod(value, divisor);
        if (quotient > 0) {
            digits[digitIndex] = quotient;
            suppressLeadingZeros = 0;
            value = (int)(kh_rt_s32_divmod(value, divisor) >> 32);
        } else if (!suppressLeadingZeros || digitIndex == 0) {
            digits[digitIndex] = 0;
        }
    }
}

