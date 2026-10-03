/* PS2: mechanically prepared copy of src/overlays/enemies/ov252_enemy_ruler_of_the_sky/data/ov252_rodata_020d4258.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov252 .rodata head 0x020d4258-0x020d4334: the constructor's tables (the per-function templates
 * that follow are in ov252_templates_020d4334.c). Q12 fixed point for the bounds. */

/* Ov252_Construct (constructor): the +0x1fc bounds, min (-1.62, -4.92, -1.48) and max
 * (1.62, 2.47, 5.01). */
typedef struct { int min[3]; int max[3]; } Bounds;

/* Ov252_Construct (constructor): poses of the 49 hidden parts. */
typedef struct { int id[49]; } PartPoses;

const Bounds data_ov252_020d4258 __attribute__((aligned(__alignof__(Bounds)))) = { { -0x19ef, -0x4ec0, -0x17af }, { 0x19ef, 0x2798, 0x502f } };

const PartPoses data_ov252_020d4270 __attribute__((aligned(__alignof__(PartPoses)))) = { {
    0x127, 0x128, 0x12a, 0x12b, 0x12d, 0x12e, 0x130, 0x131, 0x132, 0x133, 0x134, 0x135,
    0x136, 0x137, 0x138, 0x138, 0x139, 0x13a, 0x13b, 0x129, 0x129, 0x129, 0x129, 0x129,
    0x129, 0x129, 0x129, 0x12c, 0x12c, 0x12c, 0x12c, 0x12c, 0x12c, 0x12c, 0x12c, 0x12c,
    0x12c, 0x12c, 0x12c, 0x12f, 0x12f, 0x12f, 0x12f, 0x12f, 0x12f, 0x12f, 0x12f, 0x12f,
    0x12f,
} };
