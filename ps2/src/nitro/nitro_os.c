/* NitroSDK OS on the PS2: init, arenas, ticks, threads, thread queues, messages, mutexes,
 * alarms, locks, cache maintenance, owner info, termination.
 *
 * Threads.  The DS OS is a strict-priority scheduler without time slicing (the switcher is ARM
 * assembly), which is exactly the EE kernel's model, so every OSThread is an EE thread: DS
 * priority p (0 highest .. 31) runs at EE priority KH_EE_PRIO(p).  OSThreadQueue / OSMessageQueue
 * / OSMutex keep the SDK's own layout and bookkeeping (game code allocates them and some of it
 * reads their fields); only "block the current thread" and "make it runnable" are EE calls.
 * All queue manipulation happens with EE interrupts disabled, like the SDK does with IRQs.
 *
 * Per-thread PS2 state (EE thread id, join semaphore) lives in a side table keyed by the OSThread
 * pointer, never inside the game-allocated OSThread, whose size is the DS one.
 *
 * Alarms run their handlers on the game thread at the next VBlank or OS_Sleep past their fire
 * time (on the DS they run in the timer IRQ); the two users are time-outs, not audio timing.
 */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <kernel.h>
#include <delaythread.h>

typedef u64 OSTick;
typedef void *OSMessage;

/* ------------------------------------------------------------ SDK layouts (DS) */

typedef struct OSThread OSThread;
typedef struct { OSThread *head, *tail; } OSThreadQueue;
typedef struct { OSThread *prev, *next; } OSThreadLink;

typedef struct OSMutex OSMutex;
struct OSMutex {
    OSThreadQueue queue;
    OSThread *thread;
    s32 count;
    struct { OSMutex *next, *prev; } link;
};

typedef struct {
    OSThreadQueue queueSend;
    OSThreadQueue queueReceive;
    OSMessage *msgArray;
    s32 msgCount;
    s32 firstIndex;
    s32 usedCount;
} OSMessageQueue;

typedef void (*OSAlarmHandler)(void *);
typedef struct OSAlarm OSAlarm;
struct OSAlarm {
    OSAlarmHandler handler;
    void *arg;
    u32 tag;
    OSTick fire;
    OSAlarm *prev;
    OSAlarm *next;
    OSTick period;
    OSTick start;
};

enum { OS_THREAD_STATE_WAITING = 0, OS_THREAD_STATE_READY = 1, OS_THREAD_STATE_TERMINATED = 2 };
enum { OS_MESSAGE_NOBLOCK = 0, OS_MESSAGE_BLOCK = 1 };

/* ------------------------------------------------------------------- threads */

#define KH_EE_PRIO(dsprio) (16 + (int)(dsprio))
#define KH_MAX_THREADS 16

typedef struct KhThread {
    OSThread *os;          /* NULL = free slot; the main thread uses &g_main_os */
    int ee_id;
    int state;             /* OS_THREAD_STATE_* */
    u32 priority;
    void (*func)(void *);
    void *arg;
    int join_sema;
    OSThreadQueue *waiting_on;
    OSThread *qnext;       /* thread-queue link (the SDK keeps it inside OSThread) */
    OSThread *qprev;
} KhThread;

static KhThread g_threads[KH_MAX_THREADS];
static u8 g_main_os[0x100];      /* stand-in OSThread for the launcher (main) thread */
static int g_scheduler_disabled;

static KhThread *kt_of(OSThread *t)
{
    int i;
    for (i = 0; i < KH_MAX_THREADS; i++)
        if (g_threads[i].os == t)
            return &g_threads[i];
    return NULL;
}

static KhThread *kt_current(void)
{
    int id = GetThreadId();
    int i;
    for (i = 0; i < KH_MAX_THREADS; i++)
        if (g_threads[i].os && g_threads[i].ee_id == id)
            return &g_threads[i];
    return NULL;
}

OSThread *OS_GetCurrentThread(void)
{
    KhThread *k = kt_current();
    return k ? k->os : NULL;
}

void OS_InitThread(void)
{
    if (g_threads[0].os)
        return;
    g_threads[0].os = (OSThread *)g_main_os;
    g_threads[0].ee_id = GetThreadId();
    g_threads[0].state = OS_THREAD_STATE_READY;
    g_threads[0].priority = 16;
    ChangeThreadPriority(g_threads[0].ee_id, KH_EE_PRIO(16));
}

int OS_IsThreadAvailable(void) { return 1; }

static void thread_entry(void *arg)
{
    KhThread *k = arg;
    k->func(k->arg);
    /* returning from the thread function == OS_ExitThread */
    k->state = OS_THREAD_STATE_TERMINATED;
    SignalSema(k->join_sema);
    ExitDeleteThread();
}

void OS_CreateThread(OSThread *t, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 prio)
{
    ee_thread_t th;
    ee_sema_t sema;
    KhThread *k = kt_of(t);
    int i;

    if (!k) {
        for (i = 1; i < KH_MAX_THREADS && g_threads[i].os; i++)
            ;
        if (i == KH_MAX_THREADS)
            kh_panic("OS_CreateThread: more than %d threads", KH_MAX_THREADS);
        k = &g_threads[i];
    }
    memset(k, 0, sizeof *k);
    k->os = t;
    k->func = func;
    k->arg = arg;
    k->priority = prio;
    k->state = OS_THREAD_STATE_WAITING;   /* DS: created suspended until OS_WakeupThreadDirect */
    sema.init_count = 0;
    sema.max_count = 1;
    sema.option = 0;
    k->join_sema = CreateSema(&sema);

    memset(&th, 0, sizeof th);
    th.func = (void *)thread_entry;
    /* DS stacks are sized for ARM code (a few KiB); EE code saves 64-bit registers and uses larger
     * frames, so every game thread gets its own EE stack of 4x the DS size, at least 32 KiB.  The
     * game's own stack block is simply left unused. */
    (void)stack;
    {
        u32 sz = stackSize * 4 < 0x8000 ? 0x8000 : stackSize * 4;
        th.stack = kh_alloc(sz, 64, KH_LIFE_GLOBAL, KH_MEM_MISC);
        th.stack_size = (int)sz;
        if (!th.stack)
            kh_panic("OS_CreateThread: no memory for a %u KiB thread stack", (unsigned)(sz / 1024));
    }
    th.gp_reg = &_gp;
    th.initial_priority = KH_EE_PRIO(prio);
    k->ee_id = CreateThread(&th);
    if (k->ee_id < 0)
        kh_panic("OS_CreateThread: EE CreateThread failed (%d)", k->ee_id);
    KH_DBG("os", "thread %p created: ee id %d, DS prio %u", (void *)t, k->ee_id, (unsigned)prio);
}

void OS_WakeupThreadDirect(OSThread *t)
{
    KhThread *k = kt_of(t);
    if (!k)
        return;
    if (k->state == OS_THREAD_STATE_WAITING && k->waiting_on == NULL && k->func) {
        /* first wakeup of a created thread starts it */
        int started = 0;
        ee_thread_status_t st;
        if (ReferThreadStatus(k->ee_id, &st) >= 0 && st.status == THS_DORMANT) {
            k->state = OS_THREAD_STATE_READY;
            StartThread(k->ee_id, k);
            started = 1;
        }
        if (started)
            return;
    }
    k->state = OS_THREAD_STATE_READY;
    WakeupThread(k->ee_id);
}

/* --- thread queues (OS_SleepThread / OS_WakeupThread) --- */

static void queue_push(OSThreadQueue *q, KhThread *k)
{
    k->qnext = NULL;
    k->qprev = q->tail;
    if (q->tail)
        kt_of(q->tail)->qnext = k->os;
    else
        q->head = k->os;
    q->tail = k->os;
    k->waiting_on = q;
}

static void queue_remove(OSThreadQueue *q, KhThread *k)
{
    if (k->qprev)
        kt_of(k->qprev)->qnext = k->qnext;
    else
        q->head = k->qnext;
    if (k->qnext)
        kt_of(k->qnext)->qprev = k->qprev;
    else
        q->tail = k->qprev;
    k->qnext = k->qprev = NULL;
    k->waiting_on = NULL;
}

void OS_InitThreadQueue(OSThreadQueue *q) { q->head = q->tail = NULL; }

void OS_SleepThread(OSThreadQueue *q)
{
    KhThread *k = kt_current();
    int old = DIntr();
    if (!k)
        kh_panic("OS_SleepThread from a non-OS thread");
    if (q)
        queue_push(q, k);
    k->state = OS_THREAD_STATE_WAITING;
    if (old)
        EIntr();
    SleepThread();
}

void OS_WakeupThread(OSThreadQueue *q)
{
    int old = DIntr();
    while (q->head) {
        KhThread *k = kt_of(q->head);
        queue_remove(q, k);
        k->state = OS_THREAD_STATE_READY;
        WakeupThread(k->ee_id);
    }
    if (old)
        EIntr();
}

void OS_RescheduleThread(void)
{
    KhThread *k = kt_current();
    if (g_scheduler_disabled || !k)
        return;
    RotateThreadReadyQueue(KH_EE_PRIO(k->priority));
}

void OS_YieldThread(void) { OS_RescheduleThread(); }
u32 OS_DisableScheduler(void) { return g_scheduler_disabled++; }
u32 OS_EnableScheduler(void) { return g_scheduler_disabled ? g_scheduler_disabled-- : 0; }

int OS_IsThreadTerminated(const OSThread *t)
{
    KhThread *k = kt_of((OSThread *)t);
    return !k || k->state == OS_THREAD_STATE_TERMINATED;
}

void OS_JoinThread(OSThread *t)
{
    KhThread *k = kt_of(t);
    if (!k || k->state == OS_THREAD_STATE_TERMINATED)
        return;
    WaitSema(k->join_sema);
    SignalSema(k->join_sema);
}

void OS_ExitThread(void)
{
    KhThread *k = kt_current();
    if (k) {
        k->state = OS_THREAD_STATE_TERMINATED;
        SignalSema(k->join_sema);
    }
    ExitDeleteThread();
}

void OS_DestroyThread(OSThread *t)
{
    KhThread *k = kt_of(t);
    if (!k)
        return;
    if (k == kt_current()) {
        OS_ExitThread();
        return;
    }
    TerminateThread(k->ee_id);
    DeleteThread(k->ee_id);
    k->state = OS_THREAD_STATE_TERMINATED;
    SignalSema(k->join_sema);
}

void OS_KillThread(OSThread *t, void *arg) { (void)arg; OS_DestroyThread(t); }

u32 OS_GetThreadPriority(const OSThread *t) { KhThread *k = kt_of((OSThread *)t); return k ? k->priority : 16; }

int OS_SetThreadPriority(OSThread *t, u32 prio)
{
    KhThread *k = kt_of(t);
    if (!k)
        return 0;
    k->priority = prio;
    ChangeThreadPriority(k->ee_id, KH_EE_PRIO(prio));
    return 1;
}

void OS_SetThreadDestructor(OSThread *t, void (*d)(void *)) { (void)t; (void)d; }

/* ------------------------------------------------------------------ messages */

void OS_InitMessageQueue(OSMessageQueue *mq, OSMessage *array, s32 count)
{
    OS_InitThreadQueue(&mq->queueSend);
    OS_InitThreadQueue(&mq->queueReceive);
    mq->msgArray = array;
    mq->msgCount = count;
    mq->firstIndex = 0;
    mq->usedCount = 0;
}

int OS_SendMessage(OSMessageQueue *mq, OSMessage msg, s32 flags)
{
    int old = DIntr();
    while (mq->msgCount <= mq->usedCount) {
        if (!(flags & OS_MESSAGE_BLOCK)) {
            if (old) EIntr();
            return 0;
        }
        if (old) EIntr();
        OS_SleepThread(&mq->queueSend);
        old = DIntr();
    }
    mq->msgArray[(mq->firstIndex + mq->usedCount) % mq->msgCount] = msg;
    mq->usedCount++;
    OS_WakeupThread(&mq->queueReceive);
    if (old) EIntr();
    return 1;
}

int OS_ReceiveMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags)
{
    int old = DIntr();
    while (mq->usedCount == 0) {
        if (!(flags & OS_MESSAGE_BLOCK)) {
            if (old) EIntr();
            return 0;
        }
        if (old) EIntr();
        OS_SleepThread(&mq->queueReceive);
        old = DIntr();
    }
    if (msg)
        *msg = mq->msgArray[mq->firstIndex];
    mq->firstIndex = (mq->firstIndex + 1) % mq->msgCount;
    mq->usedCount--;
    OS_WakeupThread(&mq->queueSend);
    if (old) EIntr();
    return 1;
}

int OS_JamMessage(OSMessageQueue *mq, OSMessage msg, s32 flags)
{
    int old = DIntr();
    while (mq->msgCount <= mq->usedCount) {
        if (!(flags & OS_MESSAGE_BLOCK)) {
            if (old) EIntr();
            return 0;
        }
        if (old) EIntr();
        OS_SleepThread(&mq->queueSend);
        old = DIntr();
    }
    mq->firstIndex = (mq->firstIndex + mq->msgCount - 1) % mq->msgCount;
    mq->msgArray[mq->firstIndex] = msg;
    mq->usedCount++;
    OS_WakeupThread(&mq->queueReceive);
    if (old) EIntr();
    return 1;
}

int OS_ReadMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags)
{
    (void)flags;
    if (!mq->usedCount)
        return 0;
    *msg = mq->msgArray[mq->firstIndex];
    return 1;
}

/* ------------------------------------------------------------------- mutexes */

void OS_InitMutex(OSMutex *m)
{
    OS_InitThreadQueue(&m->queue);
    m->thread = NULL;
    m->count = 0;
}

void OS_LockMutex(OSMutex *m)
{
    OSThread *cur = OS_GetCurrentThread();
    int old = DIntr();
    for (;;) {
        if (!m->thread) {
            m->thread = cur;
            m->count = 1;
            break;
        }
        if (m->thread == cur) {
            m->count++;
            break;
        }
        if (old) EIntr();
        OS_SleepThread(&m->queue);
        old = DIntr();
    }
    if (old) EIntr();
}

void OS_UnlockMutex(OSMutex *m)
{
    int old = DIntr();
    if (m->thread == OS_GetCurrentThread() && --m->count == 0) {
        m->thread = NULL;
        OS_WakeupThread(&m->queue);
    }
    if (old) EIntr();
}

int OS_TryLockMutex(OSMutex *m)
{
    OSThread *cur = OS_GetCurrentThread();
    int ok = 0, old = DIntr();
    if (!m->thread) {
        m->thread = cur;
        m->count = 1;
        ok = 1;
    } else if (m->thread == cur) {
        m->count++;
        ok = 1;
    }
    if (old) EIntr();
    return ok;
}

/* --------------------------------------------------------------------- ticks */

/* DS tick: 33.513982 MHz / 64.  From the EE bus clock (147.456 MHz) without overflow. */
OSTick OS_GetTick(void)
{
    const u64 num = 16756991ull;          /* 33513982 / 2 */
    const u64 den = 4718592000ull;        /* 64 * 147456000 / 2 */
    u64 t = kh_time_ticks();
    return (t / den) * num + ((t % den) * num) / den;
}

u16 OS_GetTickLo(void) { return (u16)OS_GetTick(); }
void OS_InitTick(void) { }
int OS_IsTickAvailable(void) { return 1; }
void OS_SetTick(OSTick t) { (void)t; }

void OS_Sleep(u32 ms)
{
    if (ms)
        DelayThread(ms * 1000);
    kh_nitro_run_alarms();
}

void OS_SpinWait(u32 cycles) { (void)cycles; }

/* -------------------------------------------------------------------- alarms */

static OSAlarm *g_alarms;
static int g_alarm_inited;

void OS_InitAlarm(void) { g_alarm_inited = 1; }
void OS_EndAlarm(void) { g_alarm_inited = 0; g_alarms = NULL; }
int OS_IsAlarmAvailable(void) { return g_alarm_inited; }

void OS_CreateAlarm(OSAlarm *a)
{
    a->handler = NULL;
    a->tag = 0;
}

static void alarm_insert(OSAlarm *a)
{
    a->prev = NULL;
    a->next = g_alarms;
    if (g_alarms)
        g_alarms->prev = a;
    g_alarms = a;
}

static void alarm_remove(OSAlarm *a)
{
    if (a->prev)
        a->prev->next = a->next;
    else if (g_alarms == a)
        g_alarms = a->next;
    if (a->next)
        a->next->prev = a->prev;
    a->prev = a->next = NULL;
}

void OS_SetAlarm(OSAlarm *a, OSTick tick, OSAlarmHandler handler, void *arg)
{
    int old = DIntr();
    a->handler = handler;
    a->arg = arg;
    a->period = 0;
    a->fire = OS_GetTick() + tick;
    alarm_insert(a);
    if (old) EIntr();
}

void OS_SetPeriodicAlarm(OSAlarm *a, OSTick start, OSTick period, OSAlarmHandler handler, void *arg)
{
    int old = DIntr();
    a->handler = handler;
    a->arg = arg;
    a->period = period;
    a->start = start;
    a->fire = start;
    alarm_insert(a);
    if (old) EIntr();
}

void OS_CancelAlarm(OSAlarm *a)
{
    int old = DIntr();
    if (a->handler) {
        alarm_remove(a);
        a->handler = NULL;
    }
    if (old) EIntr();
}

void OS_SetAlarmTag(OSAlarm *a, u32 tag) { a->tag = tag; }

void kh_nitro_run_alarms(void)
{
    OSTick now = OS_GetTick();
    OSAlarm *a = g_alarms, *next;
    for (; a; a = next) {
        next = a->next;
        if (a->handler && now >= a->fire) {
            OSAlarmHandler h = a->handler;
            if (a->period) {
                while (a->fire <= now)
                    a->fire += a->period;
            } else {
                alarm_remove(a);
                a->handler = NULL;
            }
            h(a->arg);
        }
    }
}

/* -------------------------------------------------------------------- arenas */

enum { OS_ARENA_MAIN = 0, OS_ARENA_MAIN_SUBPRIV, OS_ARENA_MAINEX, OS_ARENA_ITCM, OS_ARENA_DTCM,
       OS_ARENA_SHARED, OS_ARENA_WRAM_MAIN, OS_ARENA_WRAM_SUB, OS_ARENA_WRAM_SUBPRIV, OS_ARENA_MAX };

static u8 *g_arena_lo[OS_ARENA_MAX], *g_arena_hi[OS_ARENA_MAX];
static u8 *g_arena_init_lo[OS_ARENA_MAX], *g_arena_init_hi[OS_ARENA_MAX];

/* DS sizes of the small memories the game may take arenas from. */
static const u32 k_arena_size[OS_ARENA_MAX] = {
    [OS_ARENA_ITCM] = 0x8000, [OS_ARENA_DTCM] = 0x4000, [OS_ARENA_WRAM_MAIN] = 0x8000,
};

void OS_InitArena(void)
{
    size_t sz;
    int i;
    if (g_arena_lo[OS_ARENA_MAIN])
        return;
    g_arena_lo[OS_ARENA_MAIN] = kh_mem_game_arena(&sz);
    g_arena_hi[OS_ARENA_MAIN] = g_arena_lo[OS_ARENA_MAIN] + sz;
    for (i = 0; i < OS_ARENA_MAX; i++) {
        if (k_arena_size[i]) {
            g_arena_lo[i] = kh_alloc(k_arena_size[i], 64, KH_LIFE_GLOBAL, KH_MEM_GAME_HEAP);
            g_arena_hi[i] = g_arena_lo[i] + k_arena_size[i];
        }
        g_arena_init_lo[i] = g_arena_lo[i];
        g_arena_init_hi[i] = g_arena_hi[i];
    }
    KH_INFO("os", "main arena %p-%p (%u KiB)", (void *)g_arena_lo[0], (void *)g_arena_hi[0],
            (unsigned)((g_arena_hi[0] - g_arena_lo[0]) / 1024));
}

void OS_InitArenaEx(void) { }
void *OS_GetArenaLo(int id) { return g_arena_lo[id]; }
void *OS_GetArenaHi(int id) { return g_arena_hi[id]; }
void *OS_GetInitArenaLo(int id) { return g_arena_init_lo[id]; }
void *OS_GetInitArenaHi(int id) { return g_arena_init_hi[id]; }
void OS_SetArenaLo(int id, void *p) { g_arena_lo[id] = p; }
void OS_SetArenaHi(int id, void *p) { g_arena_hi[id] = p; }

void *OS_AllocFromArenaLo(int id, u32 size, u32 align)
{
    uintptr_t p = ((uintptr_t)g_arena_lo[id] + align - 1) & ~(uintptr_t)(align - 1);
    if (!g_arena_lo[id] || p + size > (uintptr_t)g_arena_hi[id]) {
        KH_ERR("os", "OS_AllocFromArenaLo(%d, 0x%x): arena exhausted", id, (unsigned)size);
        return NULL;
    }
    g_arena_lo[id] = (u8 *)(p + size);
    return (void *)p;
}

void *OS_AllocFromArenaHi(int id, u32 size, u32 align)
{
    uintptr_t p = ((uintptr_t)g_arena_hi[id] - size) & ~(uintptr_t)(align - 1);
    if (!g_arena_hi[id] || p < (uintptr_t)g_arena_lo[id]) {
        KH_ERR("os", "OS_AllocFromArenaHi(%d, 0x%x): arena exhausted", id, (unsigned)size);
        return NULL;
    }
    g_arena_hi[id] = (u8 *)p;
    return (void *)p;
}

/* ------------------------------------------------------------------ init/misc */

void OS_InitIrqTable(void) { }
void OS_InitLock(void) { }
void OS_InitReset(void) { }
void OS_InitVAlarm(void) { }

void OS_Init(void)
{
    OS_InitArena();
    OS_InitThread();
    OS_InitAlarm();
    KH_INFO("os", "OS_Init done");
}

/* Cache maintenance: the DS flushes before DMA to VRAM/sound; on the PS2 those transfers are
 * CPU copies into the renderer's own memory, and PS2 DMA paths flush for themselves. */
void DC_FlushRange(const void *p, u32 n) { (void)p; (void)n; }
void DC_StoreRange(const void *p, u32 n) { (void)p; (void)n; }
void DC_InvalidateRange(void *p, u32 n) { (void)p; (void)n; }
void DC_FlushAll(void) { }
void DC_StoreAll(void) { }
void DC_InvalidateAll(void) { }
void DC_WaitWriteBufferEmpty(void) { }
void IC_InvalidateRange(void *p, u32 n) { (void)p; (void)n; }
void IC_InvalidateAll(void) { }

/* ARM7/ARM9 bus locks: there is no second CPU contending for the card or cartridge. */
s32 OS_GetLockID(void) { static s32 id = 0x40; return id++; }
void OS_ReleaseLockID(u32 id) { (void)id; }
s32 OS_LockByWord(u32 id, void *lw, void (*ctrl)(void)) { (void)id; (void)lw; (void)ctrl; return 0; }
s32 OS_UnlockByWord(u32 id, void *lw, void (*ctrl)(void)) { (void)id; (void)lw; (void)ctrl; return 0; }
s32 OS_TryLockByWord(u32 id, void *lw, void (*ctrl)(void)) { (void)id; (void)lw; (void)ctrl; return 0; }
s32 OS_LockCard(u16 id) { (void)id; return 0; }
s32 OS_UnlockCard(u16 id) { (void)id; return 0; }
s32 OS_LockCartridge(u16 id) { (void)id; return 0; }
s32 OS_UnlockCartridge(u16 id) { (void)id; return 0; }
s32 OS_TryLockCartridge(u16 id) { (void)id; return 0; }

u32 OS_GetConsoleType(void) { return 0x82000000u; /* retail DS, 4 MiB, cartridge boot */ }

void OS_GetMacAddress(u8 *mac)
{
    static const u8 k_mac[6] = { 0x00, 0x09, 0xbf, 0x4b, 0x48, 0x44 };
    memcpy(mac, k_mac, 6);
}

void OS_GetLowEntropyData(u32 *buf)
{
    u64 t = kh_time_ticks();
    int i;
    for (i = 0; i < 8; i++) {
        t = t * 6364136223846793005ull + 1442695040888963407ull + kh_vblank_count();
        buf[i] = (u32)(t >> 32);
    }
}

/* OS owner info (the firmware user settings the ARM7 leaves at 0x027ffc80).  The first byte,
 * the console language, selects the game's text language: 1 EN, 2 FR, 3 DE, 4 IT, 5 ES. */
int kh_config_language = 1;

void Game_ReadLocalProfile(u8 *out)
{
    static const char k_name[] = "PS2";
    int i;
    memset(out, 0, 0x54);
    out[0] = (u8)kh_config_language;
    out[1] = 0;          /* favourite colour */
    out[2] = 1;          /* birthday month */
    out[3] = 1;          /* birthday day */
    for (i = 0; k_name[i]; i++)
        ((u16 *)(out + 4))[i] = (u16)k_name[i];
    *(u16 *)(out + 0x18) = 0;
    *(u16 *)(out + 0x1a) = (u16)(sizeof k_name - 1);
    *(u16 *)(out + 0x50) = 0;
    *(u16 *)(out + 0x52) = 0;
}

void OS_Terminate(void) { kh_panic("OS_Terminate (the game stopped itself)"); }
void OS_Halt(void) { kh_panic("OS_Halt"); }
void OS_ResetSystem(u32 param)
{
    /* The DS soft reset (L+R+Start+Select, or after a fatal card error) restarts the game. */
    kh_panic("OS_ResetSystem(%u): soft reset not implemented yet", (unsigned)param);
}

/* printf family: the SDK's formats are a subset of C's. */
int OS_VSNPrintf(char *dst, u32 len, const char *fmt, va_list ap) { return vsnprintf(dst, len, fmt, ap); }
int OS_VSPrintf(char *dst, const char *fmt, va_list ap) { return vsprintf(dst, fmt, ap); }
int OS_SNPrintf(char *dst, u32 len, const char *fmt, ...)
{
    va_list ap; int n;
    va_start(ap, fmt); n = vsnprintf(dst, len, fmt, ap); va_end(ap);
    return n;
}
int OS_SPrintf(char *dst, const char *fmt, ...)
{
    va_list ap; int n;
    va_start(ap, fmt); n = vsprintf(dst, fmt, ap); va_end(ap);
    return n;
}
void OS_Printf(const char *fmt, ...)
{
    char buf[256];
    va_list ap;
    va_start(ap, fmt); vsnprintf(buf, sizeof buf, fmt, ap); va_end(ap);
    kh_log(KH_LOG_INFO, "game", "%s", buf);
}
