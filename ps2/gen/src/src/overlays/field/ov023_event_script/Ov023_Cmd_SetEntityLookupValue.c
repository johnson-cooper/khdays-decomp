/* PS2: mechanically prepared copy of src/overlays/field/ov023_event_script/Ov023_Cmd_SetEntityLookupValue.c (ps2/tools/prep_sources.py). Do not edit. */
extern int ScriptVm_ReadOperandInt(int ctx, void *arg);
extern int ScriptVm_ReadOperandFx32(int ctx, void *arg);
extern int ScriptVm_ResolveActorIndex(int ctx, int arg);
extern char *ArrayEntryPtrD0(int index);

extern long long kh_rt_s32_divmod(int a, int b);
extern void Ov023_SetScrollAndMarkDirty(void *entity, int value);

/* Script command: latches the looked-up value into the entity node's pending slot and marks it
 * dirty, then forwards it to the entity if one is spawned. */
int Ov023_Cmd_SetEntityLookupValue(int ctx, int args) {
    int entity = ScriptVm_ReadOperandInt(ctx, (void *)args);
    int name = ScriptVm_ReadOperandInt(ctx, (void *)(args + 8));
    int id = ScriptVm_ResolveActorIndex(ctx, entity);
    unsigned short value = (unsigned short)kh_rt_s32_divmod(name << 16, 0x168);
    char *node = ArrayEntryPtrD0((unsigned short)id);
    char *tbl;
    if ((*(int *)node & 0x20) == 0) {
        *(short *)(node + 0x80) = value;
        *(unsigned short *)(node + 4) |= 0x20;
    }
    tbl = *(char **)(*(char **)(ctx + 0x128) + 0x440);
    if (tbl != 0) {
        char *e = tbl + id * 0x1a64;
        if (*(int *)(e + 0x15e0) != 0) {
            Ov023_SetScrollAndMarkDirty(e, value);
        }
    }
    return 1;
}
