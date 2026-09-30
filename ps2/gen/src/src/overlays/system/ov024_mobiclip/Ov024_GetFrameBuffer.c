/* PS2: mechanically prepared copy of src/overlays/system/ov024_mobiclip/Ov024_GetFrameBuffer.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov024_GetFrameBuffer -- resolve the main BG3 screen base, ov024. Returns the fixed
 * LCDC mapping (0x06400000) when `direct` is set, else the live G2 BG3 screen pointer. */
extern void *G2_GetBG3ScrPtr(void);
void *Ov024_GetFrameBuffer(int direct) {
    if (direct != 0) {
        return (void *)((unsigned int)kh_ds_vram_win(2) + 0x0);
    }
    return G2_GetBG3ScrPtr();
}
