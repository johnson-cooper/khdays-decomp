/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_ArmDecoder.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Arm the decoder: raise bit 0 of the 64-bit state word at the root heap, install
 * the frame callback at +0x460, and hand back the entry point.
 *
 * The state word is 64-BIT -- that is why the high half is OR-ed with zero and
 * stored back, which reads as a no-op until you notice the pair. */
extern void *NNSi_FndGetCurrentRootHeap(void);
extern void func_ov022_020a37d0(void);
extern void Ov022_DecoderIdleStep(void);

void *Ov022_ArmDecoder(void) {
    char *heap = (char *)NNSi_FndGetCurrentRootHeap();

    *(kh_unaligned_u64 *)heap |= 1;
    *(void **)(heap + 0x460) = (void *)func_ov022_020a37d0;

    return (void *)Ov022_DecoderIdleStep;
}
