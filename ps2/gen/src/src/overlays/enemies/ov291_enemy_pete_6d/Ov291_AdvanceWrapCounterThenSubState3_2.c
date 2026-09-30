/* PS2: mechanically prepared copy of src/overlays/enemies/ov291_enemy_pete_6d/Ov291_AdvanceWrapCounterThenSubState3_2.c (ps2/tools/prep_sources.py). Do not edit. */
extern unsigned long long kh_rt_s32_divmod(int a, unsigned int b);
extern void SetIndexedSlot(int obj, int a, int cb);

// Once the linked object is idle (*node[8]==0), advance the wrap counter
// (node[9] = (node[9]+1) mod the object's period) via the 64-bit divide helper,
// force sub-state 3 and advance.
void Ov291_AdvanceWrapCounterThenSubState3_2(int *this)
{
    int node = this[1];
    if (*(unsigned char *)(*(int *)(node + 0x20)) != 0) {
        return;
    }
    *(int *)(node + 0x24) =
        (int)(kh_rt_s32_divmod(*(int *)(node + 0x24) + 1,
                            *(unsigned int *)(*(int *)(*(int *)node + 0x3a0) + 8)) >> 0x20);
    *(signed char *)(*(int *)node + 0x1c7) = 3;
    SetIndexedSlot((int)this, *(signed char *)((int)this + 0x20), 0);
}
