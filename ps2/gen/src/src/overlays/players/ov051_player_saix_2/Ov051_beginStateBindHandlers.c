/* PS2: mechanically prepared copy of src/overlays/players/ov051_player_saix_2/Ov051_beginStateBindHandlers.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Per-frame effect handler: rebinds the idle arm animation when the character is free to, then
 * updates and draws its effect block. */

extern int Ov022_IsState9Or6WithFlag200();
extern void BindAnimTrack();
extern void Ov051_UpdateGuardedSlots();
extern void Ov051_UpdateSlotsAndFlush();
extern void *data_ov051_020b7380;

void Ov051_beginStateBindHandlers(void *this)
{
    char *obj = (char *)*(int *)&data_ov051_020b7380;
    if ((*(kh_unaligned_u64 *)((char *)this + 0x464) & 0x10000ULL) == 0 &&
        Ov022_IsState9Or6WithFlag200((char *)this + 0x22f8) == 0 &&
        *(int *)((char *)this + 0x6bc) != 0x2e) {
        BindAnimTrack((char *)this + 0xf10, 1, (char *)this + 0xff0, 0);
    }
    Ov051_UpdateGuardedSlots(this, obj + 0x2c2c, *(short *)((char *)this + 0x2aba));
    Ov051_UpdateSlotsAndFlush(this, obj + 0x2c2c);
}
