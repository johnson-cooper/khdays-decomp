/* PS2: mechanically prepared copy of src/engine/data/main_tables_02041e6c.c (ps2/tools/prep_sources.py). Do not edit. */
/* main .rodata 0x02041e6c-0x02041eac: two tables of the BG setup helpers. */

/* Per-BG screen-load command ids (main BG0-3 = 4..7, sub BG0-3 = 20..23), returned by
 * Gfx_GetBgUploadTarget for a BG index. */
const int data_02041e6c[8] __attribute__((aligned(__alignof__(int)))) = { 4, 5, 6, 7, 20, 21, 22, 23 };

/* BG3 as an extended (bitmap) BG, indexed by the current BG mode (DISPCNT & 7); entries >= 8 are
 * folded back into 0..7 (Bg_SetMainBg3ExtControl main / Bg_SetSubBg3ExtControl sub). */
const int data_02041e8c[8] __attribute__((aligned(__alignof__(int)))) = { 3, 3, 4, 4, 4, 5, 11, 11 };
