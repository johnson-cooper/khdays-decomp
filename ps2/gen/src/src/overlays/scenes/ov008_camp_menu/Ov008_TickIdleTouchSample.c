/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_TickIdleTouchSample.c (ps2/tools/prep_sources.py). Do not edit. */
/* Idle-tick for a node: when it is not busy (+0x30 == 0) and fully idle (+0x24/+0x28/+0x2c all
 * zero), sample the touch state (Ov008_CopySourceBlock); if it is clear, advance the timed value
 * (kh_rt_s32_divmod over +0x18+1 and +0x1e78, a 64-bit result) and publish its high word.
 *
 * The old park note blamed "a guard-merge heuristic: build 139 folds the standalone
 * `if (+0x30 != 0) return;` into the following idle-AND conditional-compare chain". It is not a
 * heuristic and not the build. Two ordinary source bugs:
 *   - the touch record is `unsigned short`, not `short`: the ROM reads it with `ldrh`, and a
 *     signed array gives `ldrsh`;
 *   - the ROM has THREE separate predicated early returns, so all three guards must be written
 *     as early returns. Writing the idle test as a nested `if (a==0 && b==0 && c==0) { ... }`
 *     instead of `if (a!=0 || b!=0 || c!=0) return;` is what let mwcc fold the +0x30 guard into
 *     the same conditional-compare chain, costing 8 bytes. */
extern void Ov008_CopySourceBlock(void *out);
extern long long kh_rt_s32_divmod(int a, unsigned int b);
extern void Ov008_ShowGridPage(int ctx, int high, int flag);

void Ov008_TickIdleTouchSample(int param_1) {
    unsigned short local[4];                 /* the NitroSDK's TPData: x, y, touch, validity */
    long long r;
    if (*(int *)(param_1 + 0x30) != 0) {
        return;
    }
    if (*(int *)(param_1 + 0x24) != 0 || *(int *)(param_1 + 0x28) != 0 ||
        *(int *)(param_1 + 0x2c) != 0) {
        return;
    }
    Ov008_CopySourceBlock(local);
    if (local[2] != 0) {
        return;
    }
    r = kh_rt_s32_divmod(*(int *)(param_1 + 0x18) + 1, *(unsigned int *)(param_1 + 0x1e78));
    Ov008_ShowGridPage(param_1, (int)((unsigned long long)r >> 32), 1);
}
