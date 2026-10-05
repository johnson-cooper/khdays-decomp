/* PS2 override (ARM9 vs R5900 alignment): data_0204be1c is the 8-byte play-time start tick, but
 * the preserved DS .bss layout puts it at main .bss +0x7b7c - only 4-byte aligned.  GCC accesses a
 * global declared 'long long' with aligned 64-bit ld/sd, which raise an address error on the EE
 * (save crash: EE exception 4, BadV = &data_0204be1c, right after "card request complete").
 * prep_sources.py already makes the `*(long long *)&x` spellings safe (kh_unaligned_s64); this
 * spelling is not, so the access goes through kh_unaligned.h.  Same DS layout, same value.
 * Otherwise identical to the (prepared) original. */
#include "platform/kh_unaligned.h"
/* Commit the pending save: stamp the play-time counter, sign the work buffer with
 * SHA-1, write the magic, and queue the backup write for the chosen slot.
 *
 * Byte-identical triplet across ov008, ov009 and ov025 -- same code, differing only
 * in the two overlay-local callees and the CardTransferCtx global.
 *
 * Three orderings the ROM forces, all of them the "mwcc puts an operation where
 * the source forces it" rule:
 *   - elapsed is computed BEFORE the counter pointer is loaded;
 *   - the counter pointer is hoisted into a local so it is loaded once and kept
 *     in a callee-saved register ACROSS the divide call. Written as
 *     `*gGameState += f(...)` it reloads afterwards, which is +4 bytes;
 *   - MATH_CalcSHA1 runs before the 0xc8f592a6 magic is written, not after.
 *
 * kh_rt_ll_udiv_w_32 takes a long long in r0:r1 -- the divisor 0x1ff6210 against a
 * tick delta shifted left 6 is what turns elapsed ticks into the stored unit.
 */

#include "game/engine.h"

#pragma thumb on
typedef struct {
    unsigned char blockCounter;
    unsigned char slot;
    unsigned char pad02[2];
    int resultCode;
    void *thread;
} CardTransferCtx;

extern void CARD_UnlockBackup(int lockId);
extern void CardUnlockAfterKeyShare(int lockId);
extern int Ov009_EmitCommandAndStoreHandle(int a, void *buf, int size);
extern long long OS_GetTick(void);
extern int kh_rt_ll_udiv_w_32(long long value, int divisor, int c);
extern void MATH_CalcSHA1(void *digest, const void *data, int len);
extern void Ov009_EmitCommandVariantA(int a, int b, int c);

extern unsigned short data_0204be10;
extern unsigned char data_0204be1c[];
extern int *gGameState;
extern char *data_0204be14;
extern CardTransferCtx data_ov009_020563f8;

int Ov009_CommitSaveToSlot(int slot) {
    int probe;
    int *pCounter;
    long long elapsed;

    CARD_UnlockBackup(data_0204be10);
    Sleep_Block();
    if (Ov009_EmitCommandAndStoreHandle(0, &probe, 1) != 0) {
        CardUnlockAfterKeyShare(data_0204be10);
        Sleep_Unblock();
        return 0;
    }
    elapsed = OS_GetTick() - kh_read_s64_le_unaligned(data_0204be1c);
    pCounter = gGameState;
    *pCounter += kh_rt_ll_udiv_w_32(elapsed << 6, 0x1ff6210, 0);
    MATH_CalcSHA1(data_0204be14 + 4, data_0204be14 + 0x18, 0x1cac);
    *(int *)data_0204be14 = 0xc8f592a6;
    data_ov009_020563f8.blockCounter = 0;
    data_ov009_020563f8.slot = slot;
    Ov009_EmitCommandVariantA((data_ov009_020563f8.slot << 1) * 0x2018 + 0x20,
                        (int)data_0204be14, 0x2018);
    return 1;
}
