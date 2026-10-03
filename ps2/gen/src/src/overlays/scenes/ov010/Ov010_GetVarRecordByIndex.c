/* PS2: mechanically prepared copy of src/overlays/scenes/ov010/Ov010_GetVarRecordByIndex.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Returns the record with the given index from a list of length-prefixed variable-size records
 * (count at +4, first record at +8), or NULL when the index is out of range. */

void *Ov010_GetVarRecordByIndex(int *s, int idx) {
    unsigned int count = s[1];
    char *p = (char *)s[2];
    int i;
    if ((unsigned int)idx >= count) {
        return 0;
    }
    for (i = 0; i < idx; i++) {
        p += kh_read_s32_le_unaligned(p);
    }
    return p + 4;
}
