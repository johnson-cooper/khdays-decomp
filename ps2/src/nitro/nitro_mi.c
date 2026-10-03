/* NitroSDK MI (memory interface) on the PS2.
 *
 * The CPU copy/fill routines are ARM assembly in the SDK (libs/nitro/mi/asm_stubs); these are
 * C versions with the SDK argument order (source first for copies, value first for the MIi_
 * clears) and the SDK's forward copy direction, which matters when ranges overlap.
 * The DMA variants are CPU copies: the destinations they had on the DS (VRAM, palette, OAM,
 * geometry FIFO) are PS2 renderer memory here, reached through the GX layer.
 */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

#include <string.h>


static void copy_fwd8(const void *src, void *dst, u32 size)
{
    const u8 *s = src;
    u8 *d = dst;
    if (d <= s || d >= s + size) {
        memmove(d, s, size);
        return;
    }
    while (size--)
        *d++ = *s++;          /* overlapping forward copy replicates, like the ARM loop */
}

/* DS VRAM.  Game code reaches the banks through the LCDC window (0x06800000.., rewritten to
 * kh_ds_vram) as well as through BG/OBJ pointers.  On the DS a CPU write only lands in a bank that
 * is mapped where the CPU can see it (LCDC, BG, OBJ): scene setups clear the whole LCDC window
 * (MIi_CpuClearFast(0, 0x06800000, 0xa4000)) while textures stay in banks mapped as texture
 * memory, and those keep their contents.  Bulk fills and copies into kh_ds_vram therefore go bank
 * by bank and skip banks the CPU could not write.  (Writing them all wiped the cutscene textures:
 * the models then drew black.) */
extern int kh_nitro_vram_bank_at(uint32_t ofs, uint32_t *end);
extern int kh_nitro_bank_cpu_visible(int b);

static inline int in_vram(const void *p)
{
    return (const u8 *)p >= kh_ds_vram && (const u8 *)p < kh_ds_vram + 0xa4000;
}

/* Length of the piece of [d, d + size) inside one bank, and whether the CPU may write it. */
static u32 vram_piece(const u8 *d, u32 size, int *writable)
{
    uint32_t ofs = (uint32_t)(d - kh_ds_vram), end;
    int b = kh_nitro_vram_bank_at(ofs, &end);
    *writable = kh_nitro_bank_cpu_visible(b);
    return end - ofs < size ? end - ofs : size;
}

#define VRAM_FILL(dst, size, FILL)                                                   \
    do {                                                                             \
        u8 *d_ = (u8 *)(dst);                                                        \
        u32 left_ = (size);                                                          \
        while (left_) {                                                              \
            int ok_;                                                                 \
            u32 n_ = vram_piece(d_, left_, &ok_);                                    \
            if (ok_) {                                                               \
                FILL(d_, n_);                                                        \
                kh_vram_mark((u32)(d_ - kh_ds_vram), n_);                            \
            }                                                                        \
            d_ += n_;                                                                \
            left_ -= n_;                                                             \
        }                                                                            \
    } while (0)

#define VRAM_COPY(src, dst, size, COPY)                                              \
    do {                                                                             \
        const u8 *s_ = (const u8 *)(src);                                            \
        u8 *d_ = (u8 *)(dst);                                                        \
        u32 left_ = (size);                                                          \
        while (left_) {                                                              \
            int ok_;                                                                 \
            u32 n_ = vram_piece(d_, left_, &ok_);                                    \
            if (ok_) {                                                               \
                COPY(s_, d_, n_);                                                    \
                kh_vram_mark((u32)(d_ - kh_ds_vram), n_);                            \
            }                                                                        \
            s_ += n_;                                                                \
            d_ += n_;                                                                \
            left_ -= n_;                                                             \
        }                                                                            \
    } while (0)

static void fill8_raw(void *dst, u8 data, u32 size) { memset(dst, data, size); }
#define FILL8(d, n) fill8_raw(d, data, n)
#define CLEAR8(d, n) fill8_raw(d, 0, n)

void MI_CpuFill8(void *dst, u8 data, u32 size)
{
    if (in_vram(dst)) { VRAM_FILL(dst, size, FILL8); return; }
    memset(dst, data, size);
}
void MI_CpuClear8(void *dst, u32 size)
{
    if (in_vram(dst)) { VRAM_FILL(dst, size, CLEAR8); return; }
    memset(dst, 0, size);
}
void MI_CpuCopy8(const void *src, void *dst, u32 size)
{
    if (in_vram(dst)) { VRAM_COPY(src, dst, size, copy_fwd8); return; }
    copy_fwd8(src, dst, size);
}

static void clear16_raw(u16 data, void *dst, u32 size);
#define FILL16(d, n) clear16_raw(data, d, n)
void MIi_CpuClear16(u16 data, void *dst, u32 size)
{
    if (in_vram(dst)) { VRAM_FILL(dst, size, FILL16); return; }
    clear16_raw(data, dst, size);
}

static void clear16_raw(u16 data, void *dst, u32 size)
{
    u16 *d = dst;
    u32 n = size / 2;
    while (n--)
        *d++ = data;
}

static void copy16_raw(const void *src, void *dst, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size)
{
    if (in_vram(dst)) { VRAM_COPY(src, dst, size, copy16_raw); return; }
    copy16_raw(src, dst, size);
}

static void copy16_raw(const void *src, void *dst, u32 size)
{
    const u16 *s = src;
    u16 *d = dst;
    u32 n = size / 2;
    if ((const u8 *)d <= (const u8 *)s || (const u8 *)d >= (const u8 *)s + size) {
        memmove(d, s, n * 2);
        return;
    }
    while (n--)
        *d++ = *s++;
}

static void clear32_raw(u32 data, void *dst, u32 size);
#define FILL32(d, n) clear32_raw(data, d, n)
void MIi_CpuClear32(u32 data, void *dst, u32 size)
{
    if (in_vram(dst)) { VRAM_FILL(dst, size, FILL32); return; }
    clear32_raw(data, dst, size);
}

static void clear32_raw(u32 data, void *dst, u32 size)
{
    u32 *d = dst;
    u32 n = size / 4;
    while (n--)
        *d++ = data;
}

static void copy32_raw(const void *src, void *dst, u32 size);
void MIi_CpuCopy32(const void *src, void *dst, u32 size)
{
    if (in_vram(dst)) { VRAM_COPY(src, dst, size, copy32_raw); return; }
    copy32_raw(src, dst, size);
}

static void copy32_raw(const void *src, void *dst, u32 size)
{
    const u32 *s = src;
    u32 *d = dst;
    u32 n = size / 4;
    if ((const u8 *)d <= (const u8 *)s || (const u8 *)d >= (const u8 *)s + size) {
        memmove(d, s, n * 4);
        return;
    }
    while (n--)
        *d++ = *s++;
}

void MIi_CpuClearFast(u32 data, void *dst, u32 size) { MIi_CpuClear32(data, dst, size); }
void MIi_CpuCopyFast(const void *src, void *dst, u32 size) { MIi_CpuCopy32(src, dst, size); }

/* Send to one fixed (register) address: on the DS only used for the geometry FIFO and command
 * ports, whose addresses game code has as kh_ds_io offsets after prep_sources.py R5. */
extern void kh_ge_port_write(u32 io_offset, const u32 *w, u32 nwords);
void MIi_CpuSend32(const void *src, volatile void *dst, u32 size)
{
    uintptr_t d = (uintptr_t)dst, io = (uintptr_t)kh_ds_io;
    if (d >= 0x04000000u && d < 0x04001100u)
        d = d - 0x04000000u + io;
    if (d >= io && d < io + 0x1100u) {
        kh_ge_port_write((u32)(d - io), (const u32 *)src, size / 4);
        return;
    }
    KH_WARN("mi", "MIi_CpuSend32 to unknown target %p", (void *)dst);
}

void MI_Copy16B(const void *src, void *dst) { memmove(dst, src, 16); }
void MI_Copy32B(const void *src, void *dst) { memmove(dst, src, 32); }
void MI_Copy36B(const void *src, void *dst) { memmove(dst, src, 36); }
void MI_Copy48B(const void *src, void *dst) { memmove(dst, src, 48); }
void MI_Copy64B(const void *src, void *dst) { memmove(dst, src, 64); }
void MI_Zero36B(void *dst) { memset(dst, 0, 36); }

u32 MI_SwapWord(u32 data, volatile u32 *dst)
{
    u32 old = *dst;
    *dst = data;
    return old;
}

/* DMA: synchronous CPU copies.  Async variants call their callback immediately. */
typedef void (*MIDmaCallback)(void *);

void MI_DmaCopy32(u32 ch, const void *src, void *dst, u32 size) { (void)ch; MIi_CpuCopy32(src, dst, size); }
void MI_DmaCopy16(u32 ch, const void *src, void *dst, u32 size) { (void)ch; MIi_CpuCopy16(src, dst, size); }
void MI_DmaFill32(u32 ch, void *dst, u32 data, u32 size) { (void)ch; MIi_CpuClear32(data, dst, size); }
void MI_DmaCopy32Async(u32 ch, const void *src, void *dst, u32 size, MIDmaCallback cb, void *arg)
{
    (void)ch;
    MIi_CpuCopy32(src, dst, size);
    if (cb)
        cb(arg);
}
void MI_DmaFill32Async(u32 ch, void *dst, u32 data, u32 size, MIDmaCallback cb, void *arg)
{
    (void)ch;
    MIi_CpuClear32(data, dst, size);
    if (cb)
        cb(arg);
}
void MI_WaitDma(u32 ch) { (void)ch; }
void MI_StopDma(u32 ch) { (void)ch; }
int MI_IsDmaBusy(u32 ch) { (void)ch; return 0; }
void MI_Init(void) { }
void MI_SetWramBank(int mode) { (void)mode; }
void MI_SetMainMemoryPriority(int p) { (void)p; }

/* LZ77 (BIOS format 0x10) decompression, used for loose files (the streaming context version
 * used by the file loader is the SDK's C, kept from libs/nitro/mi). */
void MI_UncompressLZ8(const void *srcp, void *destp)
{
    const u8 *s = srcp;
    u8 *d = destp;
    u32 size = (u32)s[1] | ((u32)s[2] << 8) | ((u32)s[3] << 16);
    u32 out = 0;
    s += 4;
    while (out < size) {
        u8 flags = *s++;
        int i;
        for (i = 0; i < 8 && out < size; i++, flags <<= 1) {
            if (flags & 0x80) {
                u32 n = (s[0] >> 4) + 3;
                u32 disp = (((u32)(s[0] & 0xf) << 8) | s[1]) + 1;
                s += 2;
                while (n-- && out < size) {
                    d[out] = d[out - disp];
                    out++;
                }
            } else {
                d[out++] = *s++;
            }
        }
    }
}

void MI_UncompressLZ16(const void *srcp, void *destp) { MI_UncompressLZ8(srcp, destp); }
