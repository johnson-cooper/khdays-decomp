/* PS2: mechanically prepared copy of src/overlays/players/ov087_player_roxas_dual_3/Ov087_PollHitAndFlagLanded.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Lands the character: steps its anchor to get the vertical offset, asks its landing callback
 * whether it touched down, and on landing sets the landed flags, shows its display node and flags
 * the local player. */

extern int Ov022_StepAnchorDelta(int self, void *out);
extern void SceneNode_Enable(int a);
extern int Session_GetLocalPlayerIndex(void);

typedef struct { unsigned char b0 : 1, b1 : 1; } Flags;

int Ov087_PollHitAndFlagLanded(int self) {
    int v[3];
    int r;
    Ov022_StepAnchorDelta(self, v);
    *(int *)(self + 0x58) = v[1];
    r = (*(int (**)(int))(self + 0x668))(self);
    ((Flags *)(self + 0x694))->b1 = (unsigned char)r;
    if (((Flags *)(self + 0x694))->b1) {
        int *p;
        *(kh_unaligned_u64 *)self |= 0x2000000000000ULL;
        p = *(int **)(self + 0x20);
        if ((*p & 0x20) == 0) {
            SceneNode_Enable((int)p + 4);
        }
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_u64 *)((char *)self + 0x464) |= 2;
        }
    }
    return 0;
}
