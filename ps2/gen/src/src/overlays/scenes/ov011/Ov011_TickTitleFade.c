/* PS2: mechanically prepared copy of src/overlays/scenes/ov011/Ov011_TickTitleFade.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov011_TickTitleFade -- Ov011_TickTitleFade (MATCHED, 596 B, 8 relocs).
 *
 * Per-frame title crossfade tick. Reads the fade-state byte at pScene+0x10945
 * (case 3 = both panes fading, case 4 = single pane, else no-op). A flag bit at
 * pScene+0x23abd selects which of the two pane-fade records (base pScene+0x1093c,
 * stride 0x10940) is primary. Each pane's alpha is interpolated from the global
 * frame timer between the pane's startTime/endTime via kh_rt_u32_divmod, clamped to
 * 0x10 once the timer passes endTime, then applied to both 2D engines' blend alpha
 * (G2x_SetBlendAlpha_ at 0x04000050 / 0x04001050). In case 3, once both panes reach
 * full (0x10) it clears the two +0xa flag bytes and advances both pane state bytes
 * (0x10945 / 0x21285) to 5.
 *
 * The crack: the function is declared to RETURN int, not void. Its caller
 * Ov011_RefreshCurrentPhase discards the result (declares `extern void ...`), but the
 * definition's int return reserves r0 across the whole body -- which is what makes
 * mwcc keep r0 out of the low-pressure switch-dispatch temp and the completion-block
 * temps (the ROM numbers them r1..r4, avoiding r0). As void, the body is otherwise
 * byte-identical but mwcc lets those scratch temps grab r0. The middle (both blend
 * calls, both interpolations) matches under either return type; only r0 avoidance in
 * the two low-pressure windows depends on the non-void return.
 */

#include "nitro/types.h"

typedef struct Ov011FadePane {
    u16 startTime;   /* +0x00 */
    u16 endTime;     /* +0x02 */
} Ov011FadePane;

typedef struct Ov011Globals {
    u32 nTimer;
    u8 *pScene;
} Ov011Globals;

extern Ov011Globals data_ov011_0205e960;
extern long long kh_rt_u32_divmod(int a, int b);
extern void G2x_SetBlendAlpha_(u32 reg, int a, int b, int c, int d);

int Ov011_TickTitleFade(void)
{
    int a0, a1;
    u8 *pScene = data_ov011_0205e960.pScene;
    u32 timer;
    unsigned notbit, bit;
    Ov011FadePane *pane;
    u16 start, end;
    int local_24[2];

    switch (pScene[0x10945]) {
    case 3:
        bit = (unsigned)(pScene[0x23abd] << 0x1f) >> 0x1f;
        notbit = bit ^ 1;

        pane = (Ov011FadePane *)(pScene + 0x1093c + notbit * 0x10940);
        end = pane->endTime;
        timer = data_ov011_0205e960.nTimer;
        if (end <= timer) {
            a0 = 0x10;
        } else {
            start = pane->startTime;
            a0 = (u16)kh_rt_u32_divmod((int)(timer - start) * 0x10, end - start);
        }

        pane = (Ov011FadePane *)(pScene + 0x1093c + bit * 0x10940);
        end = pane->endTime;
        if (end <= timer) {
            a1 = 0x10;
        } else {
            start = pane->startTime;
            a1 = (u16)kh_rt_u32_divmod((int)(timer - start) * 0x10, end - start);
        }

        local_24[0] = 0x10 - a1;
        local_24[1] = a0;
        G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x50), 4, 0x28, local_24[bit], local_24[notbit]);

        bit = (unsigned)(data_ov011_0205e960.pScene[0x23abd] << 0x1f) >> 0x1f;
        notbit = bit ^ 1;
        G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x1050), 4, 0x28, local_24[bit], local_24[notbit]);

        if (a0 == 0x10 && a1 == 0x10) {
            data_ov011_0205e960.pScene[0x10946] = 0;
            data_ov011_0205e960.pScene[0x21286] = 0;
            data_ov011_0205e960.pScene[0x10945] = 5;
            data_ov011_0205e960.pScene[0x21285] = 5;
        }
        break;

    case 4: {
        int alpha;

        bit = (unsigned)(pScene[0x23abd] << 0x1f) >> 0x1f;
        pane = (Ov011FadePane *)(pScene + 0x1093c + bit * 0x10940);
        end = pane->endTime;
        timer = data_ov011_0205e960.nTimer;
        if (end <= timer) {
            alpha = 0x10;
        } else {
            start = pane->startTime;
            alpha = (u16)kh_rt_u32_divmod((int)(timer - start) * 0x10, end - start);
        }

        local_24[0] = 0x10 - alpha;
        local_24[1] = 0;
        notbit = bit ^ 1;
        G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x50), 4, 0x28, local_24[bit], local_24[notbit]);

        bit = (unsigned)(data_ov011_0205e960.pScene[0x23abd] << 0x1f) >> 0x1f;
        notbit = bit ^ 1;
        G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x1050), 4, 0x28, local_24[bit], local_24[notbit]);
        break;
    }
    }
}
