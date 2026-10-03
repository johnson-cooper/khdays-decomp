/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/data/ov002_tile_rows.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov002 tile rows, 0x0207ea14-0x0207eadc.
 *
 * Five forty-byte descriptors passed to Ov002_BuildTileRow. Only two fields move
 * between records: the count, and an offset that steps by 0xc0 each time. The rest
 * are the same constants in every row.
 */

typedef struct {
    int nUnused0;
    int nUnused1;
    int nStride;
    int nCount;
    int nOffset;
    int nEnd;
    int nFlags;
    int nStep;
    int nPad;
    int nLimit;
} Ov002TileRow;

int data_ov002_0207ea14[50] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 8, 0x18, 0xd0, -1, 0, 5, 0, 0x40,
    0, 0, 8, 0x18, 0x190, -1, 0, 5, 0, 0x40,
    0, 0, 8, 0x18, 0x250, -1, 0, 5, 0, 0x40,
    0, 0, 8, 0x18, 0x310, -1, 0, 5, 0, 0x40,
    0, 0, 8, 6, 0x3d0, -1, 0, 5, 0, 0x40,
};
