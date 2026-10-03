/* PS2 override: alignment-safe Ov000_GetVarRecordByIndex with a guard.
 *
 * The title screen's message archive is a list of length-prefixed records (count at +4, first
 * record at +8).  Record sizes are even but need not be multiples of four.  The original ARMv5
 * LDR accepts those addresses; an EE lw raises Address Error Load, so every packed-data read here
 * must be bytewise.  A corrupt length is reported once and reads as an empty string.
 */
#include "platform/kh_platform.h"
#include "platform/kh_unaligned.h"

void *Ov000_GetVarRecordByIndex(int *s, int idx)
{
    static int reported;
    static unsigned int empty[2];          /* an empty record: text that ends at once */
    unsigned int count = s[1];
    char *p = (char *)s[2];
    int i;

    if ((unsigned int)idx >= count)
        return 0;
    {   /* (diagnostic) report a record walk over misaligned addresses once */
        static int checked;
        if (!checked) {
            char *q = p;
            int k, mis = 0;
            checked = 1;
            for (k = 0; k < (int)count && k < 64; k++) {
                int len;
                if ((unsigned int)q & 3)
                    mis++;
                len = kh_read_s32_le_unaligned(q);
                if (len <= 0 || len > 0x100000)
                    break;
                q += len;
            }
            KH_INFO("title", "message archive %p: %u records at %p, %d of the first %d "
                    "misaligned (safe reads)", (void *)s, count, (void *)p, mis, k);
        }
    }
    for (i = 0; i < idx; i++) {
        int len = kh_read_s32_le_unaligned(p);
        if (len <= 0 || len > 0x100000) {
            if (!reported) {
                const unsigned char *base = (const unsigned char *)s[2];
                reported = 1;
                KH_ERR("title", "message archive %p broken: record %d of %u has length %d (index %d); "
                       "first words %08x %08x %08x %08x %08x %08x", (void *)s, i, count, len, idx,
                       kh_read_u32_le_unaligned(base + 0), kh_read_u32_le_unaligned(base + 4),
                       kh_read_u32_le_unaligned(base + 8), kh_read_u32_le_unaligned(base + 12),
                       kh_read_u32_le_unaligned(base + 16), kh_read_u32_le_unaligned(base + 20));
                kh_log_flush();
            }
            return empty;
        }
        p += len;
    }
    return p + 4;
}
