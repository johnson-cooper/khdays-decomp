/* PS2: mechanically prepared copy of src/overlays/enemies/ov214_enemy_morning_star/Ov214_initEntrySlotAdvanceIndex.c (ps2/tools/prep_sources.py). Do not edit. */
/* Fills the next entry of the ring (+0x90, 0x38-byte entries) with the default sizes, the position
 * and a fixed rotation, then advances the ring index modulo its count. */

#include "game/engine.h"

struct v3 { int x, y, z; };

extern long long kh_rt_s32_divmod(int a, unsigned b);
extern int data_02042270[];

int Ov214_initEntrySlotAdvanceIndex(int *param_1, int *param_2) {
    int entry = *(int *)(*param_1 + 0x90) + param_1[1] * 0x38;
    long long r;
    *(int *)(*(int *)(*param_1 + 0x90) + param_1[1] * 0x38) = 0x1400;
    *(int *)(entry + 4) = 0x800;
    *(struct v3 *)(entry + 0x2c) = *(struct v3 *)param_2;
    QuatFromAxisAngle((void *)(entry + 8), data_02042270, 0x1922);
    r = kh_rt_s32_divmod(param_1[1] + 1, *(unsigned *)(*param_1 + 0x8c));
    param_1[1] = (int)(r >> 0x20);
    return (int)r;
}
