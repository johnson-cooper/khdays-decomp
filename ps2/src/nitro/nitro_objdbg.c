/* Bring-up diagnostic: dump the game's object/task list (R3 on pad 1).
 *
 * Every live object (InstantiateClass/RunClassConstructor) is linked from gObjSystem[3] (next at
 * +0xc) and carries its class key (+0x10, +0x12), the state function the object manager calls
 * each frame (+0x14), its method (+0x18) and its work buffer (+0x20, size +0x24).  The state
 * function says what a task is doing - or waiting for - so a hang reads straight off the log
 * (symbolize the addresses with addr2line on the unstripped ELF).
 */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

#include <stdio.h>

extern int gObjSystem[];

void kh_debug_dump_objects(void)
{
    const u8 *node = (const u8 *)(uintptr_t)gObjSystem[3];
    int n = 0;
    KH_INFO("objs", "object list (current %p):", (void *)(uintptr_t)gObjSystem[1]);
    while (node && n < 200) {
        KH_INFO("objs", "  %p class %04x/%04x state %p method %p work %p+%x", (const void *)node,
                (unsigned)*(const u16 *)(node + 0x10), (unsigned)*(const u16 *)(node + 0x12),
                (void *)(uintptr_t)*(const u32 *)(node + 0x14), (void *)(uintptr_t)*(const u32 *)(node + 0x18),
                (void *)(uintptr_t)*(const u32 *)(node + 0x20), (unsigned)*(const u32 *)(node + 0x24));
        node = (const u8 *)(uintptr_t)*(const u32 *)(node + 0xc);
        n++;
    }
    KH_INFO("objs", "%d objects", n);
}

/* One-line form for the periodic log: every object's state function (symbolize with addr2line). */
void kh_debug_log_object_states(void)
{
    const u8 *node = (const u8 *)(uintptr_t)gObjSystem[3];
    char line[512];
    int len = 0, n = 0;
    while (node && n < 48 && len < (int)sizeof line - 12) {
        len += snprintf(line + len, sizeof line - len, " %x", (unsigned)*(const u32 *)(node + 0x14));
        node = (const u8 *)(uintptr_t)*(const u32 *)(node + 0xc);
        n++;
    }
    line[len] = 0;
    KH_INFO("objs", "states:%s", line);
}

/* Enemy spawns, logged through --wrap (ps2/tools/gen_ps2yaml.py): the spawner slot an area script
 * creates (Ov002_CreateSlotObjectAndStart: record entry, kind) and each enemy actor made from a
 * marker class (Ov107_SpawnEntityClass: class id). */
extern void *__real_Ov107_SpawnEntityClass(int id);
void *__wrap_Ov107_SpawnEntityClass(int id)
{
    void *a = __real_Ov107_SpawnEntityClass(id);
    KH_INFO("spawn", "enemy class %d -> actor %p", id, a);
    return a;
}

typedef struct { int x, y, z; } KhVec;
extern void __real_Ov002_CreateSlotObjectAndStart(int nEntry, int nKind, u32 nMask, int nMode, KhVec *pPos,
                                                   int a, int tag, int b, int c, KhVec *pAt, int am, int ap);
void __wrap_Ov002_CreateSlotObjectAndStart(int nEntry, int nKind, u32 nMask, int nMode, KhVec *pPos,
                                           int a, int tag, int b, int c, KhVec *pAt, int am, int ap)
{
    KH_INFO("spawn", "spawner slot: entry %d kind %d mask %x mode %d", nEntry, nKind, (unsigned)nMask, nMode);
    __real_Ov002_CreateSlotObjectAndStart(nEntry, nKind, nMask, nMode, pPos, a, tag, b, c, pAt, am, ap);
}

/* Script trace (ps2/overrides/engine/Game_RunActionScript.c): every command the interpreter
 * dispatches, as table.index, logged once per command position so waits do not repeat. */
void kh_trace_script_cmd(void *st, const char *cmd)
{
    static const void *last_st;
    static const char *last_cmd;
    if (st == last_st && cmd == last_cmd)
        return;
    last_st = st;
    last_cmd = cmd;
    KH_INFO("script", "%p %u.%u at %p", st, (unsigned)(u8)cmd[0], (unsigned)(u8)cmd[1], (const void *)cmd);
}

/* Field events (event slots registered by scripts, seat messages that queue an event id, the
 * dispatch of a queued event), logged through --wrap. */
extern void __real_Ov002_RegisterEventSlot(int nEventId, int nMatch, int bMuted, int nUnused, void *pSlot);
void __wrap_Ov002_RegisterEventSlot(int nEventId, int nMatch, int bMuted, int nUnused, void *pSlot)
{
    KH_INFO("event", "register slot: event %#x match %d muted %d", nEventId, nMatch, bMuted);
    __real_Ov002_RegisterEventSlot(nEventId, nMatch, bMuted, nUnused, pSlot);
}

extern void __real_Ov002_HandleSeatMessage(const u8 *pMsg);
void __wrap_Ov002_HandleSeatMessage(const u8 *pMsg)
{
    KH_INFO("event", "seat message: op %u seat %u value %d", pMsg[0], pMsg[1], *(const int *)(pMsg + 4));
    __real_Ov002_HandleSeatMessage(pMsg);
}

extern int __real_Ov002_PopAndDispatchEvent(int *pnEventId);
int __wrap_Ov002_PopAndDispatchEvent(int *pnEventId)
{
    int r = __real_Ov002_PopAndDispatchEvent(pnEventId);
    if (r != -1)
        KH_INFO("event", "dispatch: event %#x -> %d", *pnEventId, r);
    return r;
}
