/* PS2: mechanically prepared copy of libs/nns/g2d/calls/GetBGExtPlttSlot.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nnsys/g2d.h"

typedef void *OSMessage;

#define HW_MAIN_MEM 0x02000000

#define offsetof(type, member) ((u32)&(((type *)0)->member))

#define HW_IOREG ((unsigned int)kh_ds_io + 0x0)
#define HW_REG_BASE HW_IOREG        // alias
#define REG_BG0CNT_OFFSET 0x008
#define REG_BG1CNT_OFFSET 0x00a
#define REG_G2_BG0CNT_BGPLTTSLOT_MASK 0x2000
#define REG_DB_BG0CNT_OFFSET 0x1008
#define REG_DB_BG1CNT_OFFSET 0x100a

/* the function's addrTable[]: the BGnCNT register offsets, owned by g2d_screen_tables.c */
extern const u16 data_02041a04[8];

/* GetBGExtPlttSlot -- NitroSystem g2d_Screen.c: GetBGExtPlttSlot. */
NNSG2dBGExtPlttSlot GetBGExtPlttSlot (NNSG2dBGSelect bg)
{
    u32 addr;
    NNSG2dBGExtPlttSlot slot = (NNSG2dBGExtPlttSlot)bg;

    addr = data_02041a04[bg];

    if (addr != 0) {
        addr += HW_REG_BASE;

        if ((*(u16 *)addr & REG_G2_BG0CNT_BGPLTTSLOT_MASK) != 0) {
            slot += 2;
        }
    }

    return slot;
}
