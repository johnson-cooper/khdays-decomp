/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/data/ov002_gauge_tables.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov002 gauge tables, 0x0207dd7c-0x0207deb8.
 *
 * A contiguous run of small tables behind the gauge row map and slot advance
 * (Ov002_BuildGaugeRowMap, Ov002_AdvanceGaugeSlot). Each is written in the width its
 * contents are in: word arrays where the values are small integers, byte arrays where
 * the words are packed bytes.
 */

#include "nitro/types.h"

const u8 data_ov002_0207dd7c[8] __attribute__((aligned(__alignof__(u8)))) = {
    1, 15, 15, 15, 15, 1, 0, 0,
};

const u8 data_ov002_0207dd84[8] __attribute__((aligned(__alignof__(u8)))) = {
    12, 12, 13, 13, 14, 14, 15, 0,
};

const u8 data_ov002_0207dd8c[8] __attribute__((aligned(__alignof__(u8)))) = {
    12, 12, 13, 13, 14, 14, 15, 0,
};

const u8 data_ov002_0207dd94[8] __attribute__((aligned(__alignof__(u8)))) = {
    6, 5, 4, 3, 11, 11, 11, 11,
};

const u16 data_ov002_0207dd9c[3] __attribute__((aligned(__alignof__(u16)))) = {
    257, 257, 257,
};

const u16 data_ov002_0207dda2[3] __attribute__((aligned(__alignof__(u16)))) = {
    256, 257, 1,
};

const int data_ov002_0207dda8[3] __attribute__((aligned(__alignof__(int)))) = {
    0, 2, 1,
};

const u8 data_ov002_0207ddb4[12] __attribute__((aligned(__alignof__(u8)))) = {
    6, 6, 5, 5, 4, 3, 15, 15, 15, 15, 15, 15,
};

const u8 data_ov002_0207ddc0[16] __attribute__((aligned(__alignof__(u8)))) = {
    6, 6, 5, 4, 3, 15, 15, 15, 15, 15, 11, 11, 12, 13, 14, 0,
};

const int data_ov002_0207ddd0[5] __attribute__((aligned(__alignof__(int)))) = {
    51, 52, 53, 54, 55,
};

const u8 data_ov002_0207dde4[24] __attribute__((aligned(__alignof__(u8)))) = {
    6, 6, 5, 5, 4, 3, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 13, 14, 10, 10, 9, 9, 8, 7,
};

const int data_ov002_0207ddfc[2] __attribute__((aligned(__alignof__(int)))) = {
    2048, 320,
};

const int data_ov002_0207de04[10] __attribute__((aligned(__alignof__(int)))) = {
    77, 2432, 64, 14, 2496, 64, 14, 2560, 64, 14,
};

const u8 data_ov002_0207de2c[12] __attribute__((aligned(__alignof__(u8)))) = {
    108, 0, 107, 0, 106, 0, 105, 0, 104, 0, 103, 0,
};

const u8 data_ov002_0207de38[24] __attribute__((aligned(__alignof__(u8)))) = {
    97, 0, 96, 0, 96, 0, 164, 0, 166, 0, 167, 0, 165, 0, 160, 0, 162, 0, 163, 0, 161, 0, 0, 0,
};

const int data_ov002_0207de50[4] __attribute__((aligned(__alignof__(int)))) = {
    3, 4, 5, 6,
};

const int data_ov002_0207de60[4] __attribute__((aligned(__alignof__(int)))) = {
    7, 8, 9, 10,
};

const int data_ov002_0207de70[4] __attribute__((aligned(__alignof__(int)))) = {
    3, 4, 5, 6,
};

const int data_ov002_0207de80[4] __attribute__((aligned(__alignof__(int)))) = {
    7, 8, 9, 10,
};

const int data_ov002_0207de90[10] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 15, 15, 21, 0, 0, 6, 0, 64,
};
