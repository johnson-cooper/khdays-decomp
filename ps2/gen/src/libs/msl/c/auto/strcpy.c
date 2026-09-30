/* PS2: mechanically prepared copy of libs/msl/c/auto/strcpy.c (ps2/tools/prep_sources.py). Do not edit. */
/* MSL strcpy (kept under its ROM symbol name, which the callers use): word-at-a-time copy when source and destination share their alignment, with the
 * usual zero-byte test ((w + 0xfefefeff) & ~w & 0x80808080); bytes otherwise. */
#define K1 0x80808080
#define K2 0xfefefeff

char *strcpy(char *dst, const char *src)
{
    unsigned char *destb, *fromb;
    unsigned long w, t, align;

    fromb = (unsigned char *)src;
    destb = (unsigned char *)dst;

    if ((align = ((int)fromb & 3)) != ((int)destb & 3)) {
        goto bytecopy;
    }

    if (align) {
        if ((*destb = *fromb) == 0)
            return dst;
        for (align = 3 - align; align; align--) {
            if ((*(++destb) = *(++fromb)) == 0)
                return dst;
        }
        ++destb;
        ++fromb;
    }

    w = *((int *)(fromb));

    t = w + K2;

    t &= ~w;
    t &= K1;
    if (t)
        goto bytecopy;
    (*(int * *)&(destb) -= 1);

    do {
        *((*(int * *)&(destb) += 1)) = w;
        w = *((*(int * *)&(fromb) += 1));

        t = w + K2;
        t &= ~w;
        t &= K1;
        if (t)
            goto adjust;
    } while (1);

adjust:
    (*(int * *)&(destb) += 1);
bytecopy:
    if ((*destb = *fromb) == 0)
        return dst;

    do {
        if ((*(++destb) = *(++fromb)) == 0)
            return dst;
    } while (1);

    return dst;
}
