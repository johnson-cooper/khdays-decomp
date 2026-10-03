/* Explicit little-endian accesses for packed Nintendo DS resource data.
 *
 * The ARM946E-S can execute an unaligned LDR with ARMv5 rotation semantics.  Some game formats
 * deliberately put 32-bit fields at addresses that are only 16-bit aligned; an ordinary EE
 * `lw` raises AdEL instead.  Keep these bytewise so their generated R5900 code is safe for every
 * address. */
#ifndef KH_UNALIGNED_H
#define KH_UNALIGNED_H

#include <stdint.h>

static inline uint16_t kh_read_u16_le_unaligned(const void *ptr)
{
    const uint8_t *p = (const uint8_t *)ptr;
    return (uint16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
}

static inline int16_t kh_read_s16_le_unaligned(const void *ptr)
{
    return (int16_t)kh_read_u16_le_unaligned(ptr);
}

static inline uint32_t kh_read_u32_le_unaligned(const void *ptr)
{
    const uint8_t *p = (const uint8_t *)ptr;
    return (uint32_t)p[0]
         | ((uint32_t)p[1] << 8)
         | ((uint32_t)p[2] << 16)
         | ((uint32_t)p[3] << 24);
}

static inline int32_t kh_read_s32_le_unaligned(const void *ptr)
{
    return (int32_t)kh_read_u32_le_unaligned(ptr);
}

/* Alignment-1 lvalue wrappers for prepared decomp code.  Using these preserves ordinary
 * C lvalue semantics (reads, writes and compound assignments) while preventing GCC from
 * assuming an 8-byte-aligned address merely because the original source cast to u64/s64 *. */
typedef struct __attribute__((packed, may_alias)) {
    uint64_t value;
} kh_unaligned_u64_slot;

typedef struct __attribute__((packed, may_alias)) {
    int64_t value;
} kh_unaligned_s64_slot;

static inline uint64_t kh_read_u64_le_unaligned(const void *ptr)
{
    const uint8_t *p = (const uint8_t *)ptr;
    return (uint64_t)kh_read_u32_le_unaligned(p)
         | ((uint64_t)kh_read_u32_le_unaligned(p + 4) << 32);
}

static inline int64_t kh_read_s64_le_unaligned(const void *ptr)
{
    return (int64_t)kh_read_u64_le_unaligned(ptr);
}

static inline void kh_write_u64_le_unaligned(void *ptr, uint64_t value)
{
    uint8_t *p = (uint8_t *)ptr;
    p[0] = (uint8_t)(value >> 0);
    p[1] = (uint8_t)(value >> 8);
    p[2] = (uint8_t)(value >> 16);
    p[3] = (uint8_t)(value >> 24);
    p[4] = (uint8_t)(value >> 32);
    p[5] = (uint8_t)(value >> 40);
    p[6] = (uint8_t)(value >> 48);
    p[7] = (uint8_t)(value >> 56);
}

static inline void kh_write_s64_le_unaligned(void *ptr, int64_t value)
{
    kh_write_u64_le_unaligned(ptr, (uint64_t)value);
}

#endif /* KH_UNALIGNED_H */
