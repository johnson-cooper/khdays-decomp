/* PS2: mechanically prepared copy of src/overlays/enemies/ov243_enemy_cogsworth/Ov243_NextWaypoint.c (ps2/tools/prep_sources.py). Do not edit. */
/* Next-waypoint step of the ov243 enemy (and its byte-identical twins): with a +0x398 waypoint table and a
 * +0x39c count on the actor, the +0x24 index advances modulo the count and +0x1c points at the
 * 20-byte entry; then sub-state 3 is requested and the slot released. The remainder is the
 * high half of kh_rt_s32_divmod's 64-bit return; the index is stored and re-read (a local for the
 * remainder hoists the 0x14 above the table load). */
extern long long kh_rt_s32_divmod(int a, int b);
extern void SetIndexedSlot(int node, int slot, void *cb);

void Ov243_NextWaypoint(int node)
{
    int *state = *(int **)(node + 4);
    if (*(int *)(*state + 0x398) != 0 && *(int *)(*state + 0x39c) != 0) {
        state[9] = (int)(kh_rt_s32_divmod(state[9] + 1, *(int *)(*state + 0x39c)) >> 32);
        state[7] = *(int *)(*state + 0x398) + state[9] * 0x14;
    }
    *(unsigned char *)(*state + 0x1c7) = 3;
    SetIndexedSlot(node, *(signed char *)(node + 0x20), 0);
}
