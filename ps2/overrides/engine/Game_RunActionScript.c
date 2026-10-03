/* PS2 copy of Game_RunActionScript (src/engine/Game_RunActionScript.c) with a bring-up trace of
 * the script commands it dispatches (kh_trace_script_cmd).  The logic is unchanged; delete this
 * file to go back to the decomp's version. */

#include "nitro/types.h"
#include "platform/kh_prof.h"

/* bring-up trace: each new script command (table.opcode) once, ps2/src/nitro/nitro_objdbg.c */
#if KH_PS2_DEBUG
extern void kh_trace_script_cmd(void *st, const char *cmd);
#endif

typedef int (*Fn)(void *st, int arg);

extern int  Game_UnwindActionStack(void *st);
extern int  data_020425ec;
extern u8   data_0204be04;
extern void Game_UpdateObjectMotion(int a);
extern int  LoadGlobalIntAtC(void);
extern int  LoadGlobalU16At0(void);
extern void Camera_UpdateSoundListener(int a);

int Game_RunActionScript(char *st)
{
    char *cur;
    char *e;
    char *cmd;
    char *hb;
    int i;
    int r;

    cur = st + 4 + *(int *)(st + 0x124) * 0x48;
    if (*(int *)(st + 0x588) != 0 && *(int *)(st + 0x584) != 0) {
        Game_UnwindActionStack(st);
    }
    for (i = 0, e = cur; i < 5; i++, e += 8) {
        Fn f = *(Fn *)(e + 0x20);
        if (f != 0 && f(st, *(int *)(e + 0x24)) != 0) {
            *(int *)(e + 0x20) = 0;
            *(int *)(e + 0x24) = 0;
        }
    }
    {
        Fn af = *(Fn *)(cur + 0x18);
        if (af != 0) {
            r = af(st, *(int *)(cur + 0x1c));
            switch (r) {
            case 0:
                goto frame_tail;
            case 1:
                cmd = *(char **)(cur + 0x10);
                *(char **)(cur + 0x10) = cmd + ((*(u16 *)(cmd + 2) & 0x7ff) << 2);
                break;
            case 2:
                *(int *)(cur + 0x18) = 0;
                break;
            }
        }
    }

mainloop:
    e = st + 4 + *(int *)(st + 0x124) * 0x48;
    cmd = *(char **)(e + 0x10);
    {
        int h = (&data_020425ec)[*(u8 *)*(char **)(e + 0x10)];
        if (h != 0) {
            hb = (char *)h + *(u8 *)(cmd + 1) * 8;
        } else {
            *(int *)(e + 0x18) = 0;
            *(char **)(e + 0x10) = cmd + ((*(u16 *)(cmd + 2) & 0x7ff) << 2);
            goto mainloop;
        }
    }
#if KH_PS2_DEBUG
    kh_trace_script_cmd(st, cmd);
#endif
    if (((*(u16 *)(cmd + 2) >> 0xb) & 0x1f) != 0) {
        Fn hf = *(Fn *)hb;
        r = hf(st, (int)(cmd + 4));
        {
            u8 field = (u8)((*(u16 *)(cmd + 2) >> 0xb) & 0x1f);
            if (r == 0) {
                *(int *)(e + (field << 3) + 0x18) = *(int *)(hb + 4);
                *(int *)(e + (field << 3) + 0x1c) = *(int *)(e + 0x1c);
            } else {
                *(int *)(e + (field << 3) + 0x18) = 0;
                *(int *)(e + (field << 3) + 0x1c) = 0;
            }
        }
        *(int *)(e + 0x18) = 0;
        *(char **)(e + 0x10) = cmd + ((*(u16 *)(cmd + 2) & 0x7ff) << 2);
        goto mainloop;
    } else {
        Fn hf = *(Fn *)hb;
        if (hf == 0) goto mainloop;
        r = hf(st, (int)(cmd + 4));
        switch ((unsigned)r) {
        case 0:
            *(int *)(e + 0x18) = *(int *)(hb + 4);
            goto frame_tail;
        case 1:
            *(int *)(e + 0x18) = 0;
            *(char **)(e + 0x10) = cmd + ((*(u16 *)(cmd + 2) & 0x7ff) << 2);
            goto mainloop;
        case 2:
            *(int *)(e + 0x18) = 0;
            goto mainloop;
        case 3:
            return 0;
        case 4:
            if (*(int *)(st + 0x124) > 0) {
                *(int *)st = *(int *)st - *(int *)(e + 4);
                *(int *)(st + 0x124) = *(int *)(st + 0x124) - 1;
                {
                    char *pe = st + 4 + *(int *)(st + 0x124) * 0x48;
                    *(int *)(pe + 0x18) = 0;
                    *(char **)(pe + 0x10) = *(char **)(pe + 0x10)
                        + ((*(u16 *)(*(char **)(pe + 0x10) + 2) & 0x7ff) << 2);
                }
                goto mainloop;
            }
            *(int *)(st + 0x12c) = 2;
            return 0;
        case 5:
            goto mainloop;
        case 6:
            *(char **)(e + 0x10) = cmd + ((*(u16 *)(cmd + 2) & 0x7ff) << 2);
            goto frame_tail;
        default:
            goto mainloop;
        }
    }

frame_tail:
    {
        int arg = *(int *)(st + 0x128) + 0x30;
        int idx = *(u8 *)&data_0204be04;
        arg += (idx + (idx << 6)) << 2;
        Game_UpdateObjectMotion(arg);
    }
    if (LoadGlobalIntAtC() == 2 && (LoadGlobalU16At0() & 0x102) == 0) {
        int arg = *(int *)(st + 0x128) + 0x30;
        int idx = *(u8 *)&data_0204be04;
        arg += (idx + (idx << 6)) << 2;
        Camera_UpdateSoundListener(arg);
    }
    return 1;
}
