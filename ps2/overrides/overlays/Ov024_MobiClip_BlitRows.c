/* PS2 override: MobiClip YCoCg -> RGB555 conversion (hand-written ARM on the DS, see
 * libs/mobiclip/video/asm_stubs/auto/Ov024_MobiClip_BlitRows.c for the original and its notes).
 *
 * The view is decoder +0x3c: luma plane, chroma plane (Co at +0, Cg at +0x80 of each 256-byte
 * row), destination, destination stride in bytes, width, height, and the 5-bit saturation ramp
 * (0x300 bytes, entry 0x100 = level 0).  Each chroma pair covers a 2x2 luma quad:
 *     G = ramp[Y + Cg]   R = ramp[Y + Co - Cg]   B = ramp[Y - Co - Cg]
 * and the quad's anti-diagonal pixels, (0, 1) and (1, 0), use Y - 4 (the ordered dither of the
 * original).  Pixels are written 0x8000 | R | G << 5 | B << 10.
 */
typedef unsigned char u8;
typedef unsigned short u16;

struct MobiClipBlitView {
    const u8 *pLuma;
    const u8 *pChroma;
    u8 *pDest;
    int nStride;
    int nWidth;
    int nHeight;
    const u8 *pRamp;
};

static inline u8 ramp(const u8 *t, int i)
{
    /* the ARM indexes the ramp unchecked; the extreme chroma pairs can step a few entries past
     * either end, where the neighbouring tables hold the saturated values anyway */
    if (i < 0)
        i = 0;
    else if (i > 0x2ff)
        i = 0x2ff;
    return t[i];
}

static inline u16 pixel(const u8 *t, int y, int g, int r, int b)
{
    return (u16)(0x8000 | ramp(t, r + y) | ramp(t, g + y) << 5 | ramp(t, b + y) << 10);
}

void Ov024_MobiClip_BlitRows(struct MobiClipBlitView *v)
{
    const u8 *t = v->pRamp;
    int w = v->nWidth, h = v->nHeight, x, y;

    for (y = 0; y + 1 < h; y += 2) {
        const u8 *l0 = v->pLuma + y * 256, *l1 = l0 + 256;
        const u8 *c = v->pChroma + (y >> 1) * 256;
        u16 *d0 = (u16 *)(v->pDest + y * v->nStride), *d1 = (u16 *)((u8 *)d0 + v->nStride);
        for (x = 0; x < w; x += 2) {
            int co = c[x >> 1] - 0x80, cg = c[(x >> 1) + 0x80] - 0x80;
            int g = 0x100 + cg, r = 0x100 + co - cg, b = r - 2 * co;
            d0[x] = pixel(t, l0[x], g, r, b);
            d0[x + 1] = pixel(t, l0[x + 1] - 4, g, r, b);
            d1[x] = pixel(t, l1[x] - 4, g, r, b);
            d1[x + 1] = pixel(t, l1[x + 1], g, r, b);
        }
    }
}
