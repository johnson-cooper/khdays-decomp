/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/Ov000_ScrollListToRow.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov000_ScrollListToRow -- scroll the logo list to a clamped row, ov000.
 *
 * Only acts when the list has more than 8 rows. Clamps `row` into [0, rows-0xa], then converts it
 * to a pixel offset and applies it via Ov000_LayOutHandleRow:
 *
 *     offset = h[0x18] + row * ((0x14 - rowHeight) * 8) / (rows - 0xa)
 *
 * i.e. a scrollbar thumb: the row's position scaled across the free travel. The divisor is the
 * SAME `max` used for the clamp, which is why the ROM keeps it in its own register.
 *
 * ★ kh_rt_s32_divmod is the divide helper and takes TWO arguments (num, den), returning quotient in
 * r0 and remainder in r1 -- so it must be declared `long long`. This file declared it as
 * `int kh_rt_s32_divmod(int fixed)`, a one-argument fixed-point call that does not exist, and then
 * blamed the resulting diff on scheduling: "instruction-scheduling tie. The retail build keeps the
 * row max in a separate register and interleaves the pixel-offset math between the two clamps; our
 * mwcc serializes them." The "row max in a separate register" WAS the missing divisor. (2026-07-17)
 */
extern void *NNSi_FndGetCurrentRootHeap(void);
extern long long kh_rt_s32_divmod(int num, int den);
extern void Ov000_LayOutHandleRow(int offset);
void Ov000_ScrollListToRow(int row) {
    int *h = (int *)NNSi_FndGetCurrentRootHeap();
    int max;

    if (h[0x15] <= 8) {
        return;
    }
    h[0x16] = row;
    max = h[0x15] - 0xa;
    if (row < 0) {
        row = 0;
    }
    if (row > max) {
        row = max;
    }
    Ov000_LayOutHandleRow(h[0x18] + (int)kh_rt_s32_divmod(row * ((0x14 - h[0x17]) << 3), max));
}
