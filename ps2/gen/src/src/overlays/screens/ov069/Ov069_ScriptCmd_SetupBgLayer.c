/* PS2: mechanically prepared copy of src/overlays/screens/ov069/Ov069_ScriptCmd_SetupBgLayer.c (ps2/tools/prep_sources.py). Do not edit. */
/* Script opcode: hand a resolved target and an int operand to the ov002
 * dispatcher as request 0x9, then drop BG3 and BG1 to priority 0.
 *
 * Two codegen notes about the register pair. BG3CNT and BG1CNT are four bytes
 * apart, so the original loads one pointer and walks it back rather than
 * loading a second address; that is what keeps the literal pool to one word.
 * And the masked value is computed into a temporary before being stored: a
 * plain read-modify-write on the register keeps the pointer in the second
 * scratch register instead of the first and swaps the pair throughout.
 */
extern int ByteCode_ResolveOperand(void *vm, unsigned short *pc);
extern int ScriptVm_ReadOperandInt(void *vm, unsigned short *pc);
extern void Ov002_PostScoreRecord(int target, int value, int flags, int request);

int Ov069_ScriptCmd_SetupBgLayer(void *vm, unsigned short *pc)
{
    volatile unsigned short *reg = (volatile unsigned short *)((unsigned int)kh_ds_io + 0xe);
    int target;
    int value;
    unsigned short h;

    target = ByteCode_ResolveOperand(vm, pc);
    value = ScriptVm_ReadOperandInt(vm, pc + 4);
    Ov002_PostScoreRecord(target, value, 0, 0x9);

    h = *reg & ~3;
    *reg = h;
    reg -= 2;
    h = *reg & ~3;
    *reg = h;
    return 1;
}
