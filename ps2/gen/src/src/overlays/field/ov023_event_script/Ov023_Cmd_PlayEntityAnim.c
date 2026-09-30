/* PS2: mechanically prepared copy of src/overlays/field/ov023_event_script/Ov023_Cmd_PlayEntityAnim.c (ps2/tools/prep_sources.py). Do not edit. */
extern int ScriptVm_ReadOperandInt(int ctx, void *arg);
extern int ScriptVm_ReadOperandFx32(int ctx, void *arg);
extern int ScriptVm_ResolveActorIndex(int ctx, int arg);
extern void Slot48_StoreAtCurrentIndex(int ctx, int args);

extern long long kh_rt_s32_divmod(int a, int b);
extern int Ov023_TurnActorToward(int ctx, int entity, int frame);

/* Script command: plays the animation named by operand 1 on the entity from operand 0. */
int Ov023_Cmd_PlayEntityAnim(int ctx, int args) {
    int entity = ScriptVm_ReadOperandInt(ctx, (void *)args);
    int anim = ScriptVm_ReadOperandInt(ctx, (void *)(args + 8));
    int obj = ScriptVm_ResolveActorIndex(ctx, entity);
    unsigned short frame = (unsigned short)kh_rt_s32_divmod(anim << 16, 0x168);
    return Ov023_TurnActorToward(ctx, obj, frame);
}
