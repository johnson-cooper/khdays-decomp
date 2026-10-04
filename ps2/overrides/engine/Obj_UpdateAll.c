/* PS2 replacement for Obj_UpdateAll (src/engine/Obj_UpdateAll.c): the object manager's per-frame
 * pass.  Identical logic, with one ABI detail made explicit: the DS loads the state function into
 * r0 and calls it (`ldr r0, [obj, #0x14]; blx r0`), so every state is entered with its own address
 * as the first argument.  Empty states are a bare `bx lr` there - "return the first argument" in
 * the decomp - and so hand back themselves, which the loop reads as "stay in this state".  Called
 * without that argument on the EE they returned whatever was left in v0 (the heap pointer from
 * Heap_SetCurrent), which became the next "state": the Mission Mode member screen jumped into
 * .data on its first frame. */

#include "game/engine.h"
#include "platform/kh_platform.h"

extern int gObjSystem[];
extern void kh_debug_mark(const char *stage, int a, int b);

typedef int (*ObjStateFn)(void *self);

void Obj_UpdateAll(int paused)
{
    int *obj;
    int next;

    gObjSystem[1] = gObjSystem[3];
    obj = (int *)gObjSystem[1];
    while (obj != 0) {
        switch (obj[5]) {
        case -2:
            next = obj[3];
            if (!(obj[0] & 1)) {
                next = Obj_Destroy(obj);
            }
            break;
        case -1:
            next = obj[3];
            break;
        default:
            if (paused == 0 || (obj[0] & 4)) {
                /*
                 * Keep the object being updated in a local and re-publish it after the callback.
                 * A number of scene helpers instantiate/destroy temporary objects and therefore
                 * touch gObjSystem[1].  The DS implementations restore it before returning, but
                 * one missed restore in a PS2 compatibility path made the callback result land in
                 * the wrong object.  The calendar then requested SCENE_FIELD but its own state was
                 * never latched to -2, leaving a live main loop parked forever on the DAY card.
                 *
                 * Using the saved object is equivalent to the DS path when the global is intact,
                 * and makes the object-manager invariant explicit on the EE.
                 */
                int *running = obj;
                int arena = Heap_SetCurrent(running[7]);
                ObjStateFn fn = (ObjStateFn)running[5];
                int cb = fn((void *)fn);

                Heap_SetCurrent(arena);
                gObjSystem[1] = (int)running;
                if (cb != 0) {
                    running[5] = cb;
                    if (cb == -2) {
                        kh_debug_mark("object state: dead", 
                                      *(unsigned short *)((char *)running + 0x10),
                                      *(unsigned short *)((char *)running + 0x12));
                    }
                }
            }
            next = obj[3];
            break;
        }
        gObjSystem[1] = next;
        obj = (int *)next;
    }
    gObjSystem[1] = 0;
    if (paused == 0) {
        gObjSystem[2]++;
    }
}
