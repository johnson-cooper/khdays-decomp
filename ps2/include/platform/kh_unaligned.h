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

#endif /* KH_UNALIGNED_H */
