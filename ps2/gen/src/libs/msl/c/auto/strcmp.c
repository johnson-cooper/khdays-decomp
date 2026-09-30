/* PS2: mechanically prepared copy of libs/msl/c/auto/strcmp.c (ps2/tools/prep_sources.py). Do not edit. */
/* MSL strcmp (kept under its ROM symbol name, which the callers use): word-at-a-time compare when both
 * strings share their alignment, with the usual zero-byte test ((w + 0xfefefeff) & ~w & 0x80808080);
 * bytes otherwise, and bytes again to locate the difference once two words differ. */
#define K1 0x80808080
#define K2 0xfefefeff

int strcmp(const char *str1, const char *str2)
{
    unsigned char *left = (unsigned char *)str1;
    unsigned char *right = (unsigned char *)str2;
    unsigned long align, l1, r1, x;

    l1 = *left;
    r1 = *right;
    if (l1 - r1)
        return l1 - r1;

    if ((align = ((int)left & 3)) != ((int)right & 3))
        goto bytecopy;

    if (align) {
        if (l1 == 0)
            return 0;
        for (align = 3 - align; align; align--) {
            l1 = *(++left);
            r1 = *(++right);
            if (l1 - r1)
                return l1 - r1;
            if (l1 == 0)
                return 0;
        }
        left++;
        right++;
    }

    l1 = *(int *)left;
    r1 = *(int *)right;
    x = l1 + K2;
    if (x & ~l1 & K1)
        goto adjust;

    while (l1 == r1) {
        l1 = *((*(int * *)&(left) += 1));
        r1 = *((*(int * *)&(right) += 1));
        x = l1 + K2;
        if (x & K1)
            goto adjust;
    }

    --left;
    --right;
    goto bytecopy;

adjust:
    l1 = *left;
    r1 = *right;
    if (l1 - r1)
        return l1 - r1;

bytecopy:
    if (l1 == 0)
        return 0;

    do {
        l1 = *(++left);
        r1 = *(++right);
        if (l1 - r1)
            return l1 - r1;
    } while (l1 != 0);

    return 0;
}
