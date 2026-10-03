/* PS2: mechanically prepared copy of src/engine/data/main_tables_02041ecc.c (ps2/tools/prep_sources.py). Do not edit. */
/* main .rodata 0x02041ecc-0x02041fd4: BG-mode selection and GFX command tables of the BG setup
 * helpers (0x020243f4..0x02024b04).
 *
 * Each BG-mode table is indexed by the current BG mode (DISPCNT & 7, main or sub engine) and gives
 * the BG mode the helper switches to so that BG2/BG3 can take the requested layout;
 * DispMode_LookupWordAndDispatch (main) and SetSubEngineGraphicsModeFromTable (sub) fold entries >= 8 back into 0..7 before calling
 * the SetGraphicsMode routine. */

typedef void (*GfxCallback)(void);

extern void Bg_WriteMainBg0Cnt(void);

/* BG3 as an affine BG (BG3CNT with the wrap bit): Bg_SetMainBg3AffineControl / Bg_SetSubBg3AffineControl. */
const int data_02041ecc[8] __attribute__((aligned(__alignof__(int)))) = { 1, 1, 2, 1, 2, 9, 9, 9 };

/* BG3 as a 256-colour text BG (BG3CNT bit 7): Bg_SetMainBg3TextControl / Bg_SetSubBg3TextControl. */
const int data_02041eec[8] __attribute__((aligned(__alignof__(int)))) = { 0, 0, 8, 0, 8, 8, 8, 8 };

/* BG2 as an extended (bitmap) BG: Bg_SetMainBg2ExtControl / Bg_SetSubBg2ExtControl. */
const int data_02041f0c[8] __attribute__((aligned(__alignof__(int)))) = { 13, 13, 13, 5, 5, 5, 13, 13 };

/* BG2 as an affine BG: Bg_SetMainBg2AffineControl / Bg_SetSubBg2AffineControl. */
const int data_02041f2c[8] __attribute__((aligned(__alignof__(int)))) = { 10, 2, 2, 4, 4, 4, 12, 10 };

/* BG2 as a 256-colour text BG (BG2CNT bit 7): Bg_SetMainBg2TextControl / Bg_SetSubBg2TextControl; followed by the
 * per-BG character-load command ids (main BG0-3 = 8..11, sub BG0-3 = 24..27). */
const struct {
    int bg2TextMode[8];
    int charCmd[8];
} data_02041f4c __attribute__((aligned(4))) = {
    { 0, 1, 1, 3, 3, 3, 11, 8 },
    { 8, 9, 10, 11, 24, 25, 26, 27 },
};

/* Per-BG screen-load command ids (main BG0-3 = 4..7, sub BG0-3 = 20..23) queued by
 * Gfx_EnqueueTableCmdAt14. */
const int data_02041f8c[8] __attribute__((aligned(__alignof__(int)))) = { 4, 5, 6, 7, 20, 21, 22, 23 };

/* Per-BG character-load command ids queued by Gfx_EnqueueTableCmdAtC, then the handler slot that follows
 * the table (Bg_WriteMainBg0Cnt) and a terminating zero. */
const struct {
    int cmd[8];
    GfxCallback handler;
    int end;
} data_02041fac __attribute__((aligned(4))) = {
    { 8, 9, 10, 11, 24, 25, 26, 27 },
    Bg_WriteMainBg0Cnt,
    0,
};
